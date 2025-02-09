/*
** EPITECH PROJECT, 2024
** Game
** File description:
** GameServer.cpp
*/

#include <iostream>
#include "GameServer.hpp"
#include "Entity.hpp"
#include <chrono>

#include "GameServer.hpp"
#include <iostream>
#include <thread>

GameServer::GameServer(const std::string &address, unsigned short port)
{
    setupCommandHandlers();

    server.set_on_message([this](const asio::ip::udp::endpoint &client, const std::string &message) {
        try {
            sf::Packet packet = stringToPacket(message);
            sf::Uint8 action;
            packet >> action;

            auto it = commandHandlers_.find(action);
            if (it != commandHandlers_.end()) {
                it->second(client, packet);
            } else {
                std::cerr << "Unknown action: " << static_cast<int>(action) << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Error handling message: " << e.what() << std::endl;
        }
    });

    server.set_on_connect([this](const asio::ip::udp::endpoint &client) {
        std::cout << "Client connected: " << client.address().to_string() << ":" << client.port() << std::endl;
    });

    server.set_on_disconnect([this](const asio::ip::udp::endpoint &client) {
        std::cout << "Client disconnected: " << client.address().to_string() << ":" << client.port() << std::endl;
    });

    try {
        server.start(address, port);
    } catch (const std::exception &e) {
        throw std::runtime_error("Failed to start server: " + std::string(e.what()));
    }

    std::thread(&GameServer::broadcastUpdates, this).detach();
}

GameServer::~GameServer() {}


void GameServer::run()
{
    using clock = std::chrono::steady_clock;
    const float fixedDeltaTime = 1.0f / 60.0f;
    auto lastTime = clock::now();
    float accumulator = 0.5f;

    while (true) {
        try {
            auto currentTime = clock::now();
            std::chrono::duration<float> elapsedTime = currentTime - lastTime;
            lastTime = currentTime;
            float deltaTime = elapsedTime.count();
            accumulator += deltaTime;

            std::unordered_map<std::string, std::reference_wrapper<Room>> localRooms;

            {
                std::lock_guard<std::mutex> lock(roomsMutex_);
                for (auto &roomPair : rooms_) {
                    localRooms.emplace(roomPair.first, std::ref(roomPair.second));
                }
            }

        while (accumulator >= fixedDeltaTime) {
            for (auto &[roomName, roomRef] : localRooms) {
                Room &room = roomRef.get();
                auto &world = room.getWorld();
                auto &players = room.getPlayers();

                try {
                    world.updateSystems(fixedDeltaTime);
                } catch (const std::exception &e) {
                    std::cerr << "[ERROR] updateSystems failed for room '" << roomName
                            << "': " << e.what() << std::endl;
                    for (const auto &entity : world.getEntities()) {
                        // std::cout << "[DEBUG] Entity ID: " << entity
                        //           << ", Components: " << std::endl;
                    }
                    continue;
                }

                // Traitement des nouvelles entités
                for (const auto &newEntity : world.newEntity_) {
                    if (world.hasComponent<PositionComponent>(newEntity.id)) {
                        const auto &position = world.getComponent<PositionComponent>(newEntity.id);
                        const auto &sprite = world.getComponent<SpriteComponent>(newEntity.id);

                        sf::Packet packet;
                        packet << CREATE_ENTITY;
                        packet << static_cast<int>(newEntity.id);
                        packet << position.getX() << position.getY();
                        packet << sprite.getSpritePath();
                        packet << newEntity.type;

                        for (const auto &clientId : players) {
                            auto it = std::find_if(data_.players_.begin(), data_.players_.end(),
                                [&](const Data::Player &player) { return player.id == clientId.id; });
                            if (it != data_.players_.end()) {
                                server.send_to_client(it->endpoint, packetToString(packet));
                            }
                        }
                    }
                }

                // Traitement des entités supprimées
                for (EntityID &entity : world.deletedEntity_) {
                    sf::Packet deletePacket;
                    deletePacket << DELETE_ENTITY << static_cast<int>(entity);

                    for (const auto &clientId : players) {
                        auto it = std::find_if(data_.players_.begin(), data_.players_.end(),
                            [&](const Data::Player &player) { return player.id == clientId.id; });

                        if (it != data_.players_.end()) {
                            if (it->idEcs == entity) {
                                sf::Packet specialPacket;
                                specialPacket << GAME_OVER << static_cast<int>(entity);
                                std::cout << "PACKET GAME OVER ENVOYE" << std::endl;
                                server.send_to_client(it->endpoint, packetToString(specialPacket));
                            } else {
                                server.send_to_client(it->endpoint, packetToString(deletePacket));
                            }
                        }
                    }
                }

                // Traitement des entités new score
                for (EntityID &entity : world.newScoreId_) {
                    auto &scoreComponent = world.getComponent<ScoreComponent>(entity);

                    for (const auto &clientId : players) {
                        auto it = std::find_if(data_.players_.begin(), data_.players_.end(),
                            [&](const Data::Player &player) { return player.id == clientId.id; });

                        if (it != data_.players_.end()) {
                            if (it->idEcs == entity) {
                                sf::Packet specialPacket;
                                specialPacket << SCORE_PLAYER << static_cast<int>(entity) << scoreComponent.getScore();
                                server.send_to_client(it->endpoint, packetToString(specialPacket));
                            }
                        }
                    }
                }

                world.newEntity_.clear();
                world.deletedEntity_.clear();
                world.newScoreId_.clear();
            }
            accumulator -= fixedDeltaTime;
        }

        if (joined_.use) {
            joined_.use = false;
            std::cout << "[INFO] Processing joined player: " << joined_.playerId << std::endl;

            auto itRoom = std::find_if(rooms_.begin(), rooms_.end(), [&](auto &room) {
                auto &players = room.second.getPlayers();
                return std::any_of(players.begin(), players.end(), [&](Data::Player &player) {
                    return player.id == joined_.playerId;
                });
            });

            if (itRoom == rooms_.end()) {
                std::cerr << "[ERROR] No room found for player " << joined_.playerId << "." << std::endl;
                continue;
            }

            Room &room = itRoom->second;
            auto &world = room.getWorld();

            try {
                std::size_t idEcs_;
                world.createPlayer(joined_.spritePath, WeaponType::SINGLE_SHOT, idEcs_);
                data_.updatePlayerIdEcs(joined_.playerId, idEcs_);

                auto itPlayer = std::find_if(data_.players_.begin(), data_.players_.end(),
                                            [&](const Data::Player &player) { return player.id == joined_.playerId; });
                if (itPlayer == data_.players_.end()) {
                    std::cerr << "[ERROR] Player ID " << joined_.playerId << " not found in players_." << std::endl;
                    continue;
                }

                const auto &newPlayerEndpoint = itPlayer->endpoint;

                for (const auto &entity : world.getEntities()) {
                    if (world.hasComponent<PositionComponent>(entity) && world.hasComponent<SpriteComponent>(entity)) {
                        const auto &position = world.getComponent<PositionComponent>(entity);
                        const auto &sprite = world.getComponent<SpriteComponent>(entity);

                        sf::Packet existingEntityPacket;
                        std::string entityType;

                        if (sprite.getSpritePath() == "../assets/sprites/green_player.gif" ||
                            sprite.getSpritePath() == "../assets/sprites/red_player.gif" ||
                            sprite.getSpritePath() == "../assets/sprites/purple_player.gif" ||
                            sprite.getSpritePath() == "../assets/sprites/blue_player.gif" ||
                            sprite.getSpritePath() == "../assets/sprites/blue_player2.gif") {
                            entityType = "PLAYER";
                        } else if (world.hasComponent<ParallaxComponent>(entity)) {
                            entityType = "BACKGROUND";
                        } else if (sprite.getSpritePath() == "../assets/sprites/projectiles1/projectile_niv1.png") {
                            entityType = "PROJECTILE";
                        } else if (sprite.getSpritePath() == "../assets/sprites/Minions/EplosiveMinions.gif") {
                            entityType = "ENEMY";
                        } else {
                            entityType = "UNKNOWN";
                        }

                        existingEntityPacket << CREATE_ENTITY
                                            << static_cast<int>(entity)
                                            << position.getX()
                                            << position.getY()
                                            << sprite.getSpritePath()
                                            << entityType;

                        server.send_to_client(newPlayerEndpoint, packetToString(existingEntityPacket));
                        std::cout << "[INFO] Sent existing entity " << entity << " to player " << joined_.playerId << std::endl;
                    }
                }
            } catch (const std::exception &e) {
                std::cerr << "[ERROR] Exception while processing joined player: " << e.what() << std::endl;
            }
        }

        } catch (const std::exception &e) {
            std::cerr << "[ERROR] Exception in main loop: " << e.what() << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

////////////////////////////////////////////////////////// SETUP COMMAND /////////////////////////////////////////////////////////////////////////////////

void GameServer::setupCommandHandlers()
{
    commandHandlers_[CONNECT_PLAYER] = std::bind(&GameServer::handleConnectPlayer, this, std::placeholders::_1, std::placeholders::_2);
    commandHandlers_[DISCONNECT_PLAYER] = std::bind(&GameServer::handleDisconnectPlayer, this, std::placeholders::_1, std::placeholders::_2);
    commandHandlers_[CREATE_ROOM] = std::bind(&GameServer::handleCreateRoom, this, std::placeholders::_1, std::placeholders::_2);
    commandHandlers_[SEARCH_ROOM] = std::bind(&GameServer::handleSearchRoom, this, std::placeholders::_1, std::placeholders::_2);
    commandHandlers_[JOIN_ROOM] = std::bind(&GameServer::handleJoinRoom, this, std::placeholders::_1, std::placeholders::_2);
    commandHandlers_[REQUEST_ROOMS] = std::bind(&GameServer::handleRequestRoom, this, std::placeholders::_1, std::placeholders::_2);
    commandHandlers_[SEND_INPUT] = std::bind(&GameServer::handleInput, this, std::placeholders::_1, std::placeholders::_2);
    commandHandlers_[QUIT_GAME] = std::bind(&GameServer::handleQuitGame, this, std::placeholders::_1, std::placeholders::_2);
}

///////////////////////////////////////////////////////////// PERIODIC UPDATE //////////////////////////////////////////////////////////////////////////////

void GameServer::broadcastUpdates() {
    auto lastBroadcastTime = std::chrono::steady_clock::now();
    const float broadcastInterval = 1.0f / 80.0f;
    const int maxEntitiesPerPacket = 1;

    while (true) {
        std::unordered_map<std::string, std::reference_wrapper<Room>> localRooms;
        {
            std::lock_guard<std::mutex> lock(roomsMutex_);
            for (auto &roomPair : rooms_) {
                localRooms.emplace(roomPair.first, std::ref(roomPair.second));
            }
        }

        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - lastBroadcastTime;

        if (elapsedTime.count() >= broadcastInterval) {
            lastBroadcastTime = currentTime;
            for (auto &[roomName, roomRef] : localRooms) {
                Room &room = roomRef.get();
                auto &world = room.getWorld();
                auto &players = room.getPlayers();

                sf::Packet groupedPacket;
                int entityCount = 0;

                for (const auto &entityId : world.getEntities()) {
                    if (!world.isEntityValid(entityId)) {
                        continue;
                    }
                    if (world.hasComponent<PositionComponent>(entityId) &&
                        world.hasComponent<SpriteComponent>(entityId)) {
                        
                        if (entityCount == 0) {
                            groupedPacket.clear();
                            groupedPacket << POSITION_ENTITY;
                        }
                        const auto &position = world.getComponent<PositionComponent>(entityId);
                        groupedPacket << static_cast<int>(entityId) 
                                      << position.getX() 
                                      << position.getY();
                        if (position.getX() > 900 || position.getX() < 0) {
                            continue;
                        }
                        entityCount++;

                        if (entityCount >= maxEntitiesPerPacket) {
                            groupedPacket << entityCount;
                            for (auto &player : players) {
                                std::cout << "Player endpoint = " << endpointToString(player.endpoint) << std::endl;
                                server.send_to_client(player.endpoint, packetToString(groupedPacket));
                            }
                            entityCount = 0;
                        }
                    }
                }
                if (entityCount > 0) {
                    groupedPacket << entityCount;
                    for (auto &player : players) {
                        server.send_to_client(player.endpoint, packetToString(groupedPacket));
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
