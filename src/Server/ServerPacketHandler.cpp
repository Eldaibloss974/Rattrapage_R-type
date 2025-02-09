/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** ServerPacketHandler.cpp
*/

#include "GameServer.hpp"
#include "SpawnSystem.hpp"
#include "ParallaxSystem.hpp"
#include <iostream>


////////////////////////////////////////////////////////// HANDLE COMMAND /////////////////////////////////////////////////////////////////////////////////

void GameServer::handleConnectPlayer(const asio::ip::udp::endpoint &client, sf::Packet &packet)
{
    std::string playerId;
    packet >> playerId;

    std::cout << "<-----" << endpointToString(client) << ": send action CONNECT_PLAYER." << std::endl;
    if (playerId.empty() || playerId.size() > 25 || !std::all_of(playerId.begin(), playerId.end(), ::isalnum)) {
        sf::Packet responsePacket;
        responsePacket << NOT_ACCEPT_CONNECT_PLAYER;
        server.send_to_client(client, packetToString(responsePacket));
        std::cout << "----->" << "Server send to " << endpointToString(client) << " Invalid PlayerId in response avec CONNECT_PLAYER." << std::endl;
        return;
    }

    if (data_.ifPlayerExist(playerId)) {
        sf::Packet responsePacket;
        responsePacket << NOT_ACCEPT_CONNECT_PLAYER;
        server.send_to_client(client, packetToString(responsePacket));
        std::cout << "----->" << "Server send to " << endpointToString(client) << " PlayerId Already connected in response avec CONNECT_PLAYER." << std::endl;
        return;
    }

    if (data_.addPlayer(playerId, client)) {
        sf::Packet responsePacket;
        responsePacket << ACCEPT_CONNECT_PLAYER << playerId;
        server.send_to_client(client, packetToString(responsePacket));
        std::cout << "----->" << "Server send to " << endpointToString(client) << " PlayerId connected in response avec CONNECT_PLAYER." << std::endl;
    } else {
        sf::Packet responsePacket;
        responsePacket << NOT_ACCEPT_CONNECT_PLAYER;
        server.send_to_client(client, packetToString(responsePacket));
        std::cout << "----->" << "Server send to " << endpointToString(client) << " PlayerId Failed to add in response avec CONNECT_PLAYER." << std::endl;
    }
}

void GameServer::handleDisconnectPlayer(const asio::ip::udp::endpoint &client, sf::Packet &packet)
{
    std::string playerId;
    packet >> playerId;

    std::cout << "Packet quit has been received for player: " << playerId << std::endl;

    auto playerOpt = data_.getPlayerIdEcs(playerId);
    if (!playerOpt.has_value()) {
        std::cerr << "Player with ID " << playerId << " does not exist, ignoring QUIT_GAME.\n";
        return;
    }

    std::size_t entityId = playerOpt.value();

    auto roomIt = std::find_if(rooms_.begin(), rooms_.end(), [&](auto &pair) {
        return std::find_if(pair.second.getPlayers().begin(), pair.second.getPlayers().end(),
                            [&](const Data::Player& player) {
                                return player.id == playerId;
                            }) != pair.second.getPlayers().end();
    });

    if (roomIt == rooms_.end()) {
        std::cerr << "Failed to find the room for player " << playerId << ".\n";
        return;
    }

    roomIt->second.delPlayer(playerId);

    roomIt->second.getWorld().removeEntity(entityId);

    sf::Packet deletePacket;
    deletePacket << DELETE_ENTITY << static_cast<int>(entityId);
    for (const auto &player : roomIt->second.getPlayers()) {
        server.send_to_client(player.endpoint, packetToString(deletePacket));
    }

    std::cout << "Player with ID " << playerId << " and entity ID " << entityId << " has been removed from the room.\n";
}

void GameServer::handleCreateRoom(const asio::ip::udp::endpoint &client, sf::Packet &packet)
{
    std::string playerId, roomName;
    int level;
    World world;

    packet >> playerId >> roomName >> level;

    if (roomName.empty() || rooms_.find(roomName) != rooms_.end()) {
        sf::Packet responsePacket;
        responsePacket << NOT_ACCEPT_CREATE_ROOM << "Room name already exists or invalid.";
        server.send_to_client(client, packetToString(responsePacket));
        return;
    }

    if (level < 1 || level > 8) {
        sf::Packet responsePacket;
        responsePacket << NOT_ACCEPT_CREATE_ROOM << "Invalid level. Must be between 1 and 8.";
        server.send_to_client(client, packetToString(responsePacket));
        return;
    }

    std::lock_guard<std::mutex> lock(roomsMutex_);

    rooms_[roomName] = Room(roomName, level);
    rooms_[roomName].getWorld().addSystem(std::make_unique<ParallaxSystem>());
    rooms_[roomName].getWorld().addSystem(std::make_unique<WeaponSystem>());
    rooms_[roomName].getWorld().addSystem(std::make_unique<MovementSystem>());
    rooms_[roomName].getWorld().addSystem(std::make_unique<SpawnSystem>());
    rooms_[roomName].getWorld().addSystem(std::make_unique<LifetimeSystem>());

    rooms_[roomName].getWorld().addSystem(std::make_unique<CollisionSystem>());
    rooms_[roomName].getWorld().initialize();

    rooms_[roomName].getWorld().createSpawner("enemy_basic", 800.0f, 100.0f, 10.0f, 0.0f, level);

    sf::Packet responsePacket;
    responsePacket << ACCEPT_CREATE_ROOM << roomName << level;
    server.send_to_client(client, packetToString(responsePacket));

    std::cout << "Room created: " << roomName << " by " << playerId << " at level " << level << "." << std::endl;
}

void GameServer::handleSearchRoom(const asio::ip::udp::endpoint &client, sf::Packet &packet)
{
    sf::Packet responsePacket;
    responsePacket << RECEIVE_ROOMS;
    responsePacket << static_cast<sf::Uint32>(rooms_.size());

    std::lock_guard<std::mutex> lock(roomsMutex_);
    for (auto &room : rooms_) {
        responsePacket << room.second.getRoomName() << room.second.getLevel() << static_cast<sf::Uint32>(room.second.getPlayers().size());
    }
    server.send_to_client(client, packetToString(responsePacket));
}

void GameServer::handleJoinRoom(const asio::ip::udp::endpoint &client, sf::Packet &packet)
{
    std::string roomName;
    std::string clientId;
    std::string spritePath;
    packet >> clientId >> roomName >> spritePath;
    std::cout << "[DEBUG] Room name extracted from packet: " << roomName << std::endl;

    std::lock_guard<std::mutex> lock(roomsMutex_);
    auto itRoom = rooms_.find(roomName);
    joined_.use = false;
    sf::Packet responsePacket;
    if (itRoom != rooms_.end()) {
        auto playerIdEcsOpt = data_.getPlayerIdEcs(clientId);
        if (playerIdEcsOpt) {
            itRoom->second.addPlayer(clientId, client, *playerIdEcsOpt);
            responsePacket << JOIN_ROOM_SUCCESS << roomName;

            std::cout << "[INFO] PLAYER ADDED IN ROOM: " << roomName << " WITH ID: " << clientId << std::endl;
        } else {
            responsePacket << JOIN_ROOM_FAILURE << "Player ID not found.";
            std::cout << "[ERROR] Player " << clientId << " has no valid ECS ID." << std::endl;
        }

        joined_.playerId = clientId;
        joined_.client = client;
        joined_.roomName = roomName;
        joined_.use = true;
        joined_.spritePath = spritePath;
    } else {
        std::cout << "[WARNING] Room not found: " << roomName 
                  << ". Current rooms available: " << std::endl;
        for (const auto &room : rooms_) {
            std::cout << "  - " << room.first << std::endl;
        }
        responsePacket << JOIN_ROOM_FAILURE << "Room not found.";
    }
    std::string responseString = packetToString(responsePacket);
    // std::cout << "[DEBUG] Sending response to client: " << responseString << std::endl;
    server.send_to_client(client, responseString);
}

void GameServer::handleRequestRoom(const asio::ip::udp::endpoint &client, sf::Packet &packet)
{
    std::lock_guard<std::mutex> lock(roomsMutex_);

    sf::Packet responsePacket;
    responsePacket << RECEIVE_ROOMS;
    responsePacket << static_cast<sf::Uint32>(rooms_.size());
    for (auto &room : rooms_) {
        responsePacket << room.second.getRoomName() << room.second.getLevel() << static_cast<sf::Uint32>(room.second.getPlayers().size());
    }
    server.send_to_client(client, packetToString(responsePacket));
}

void GameServer::handleInput(const asio::ip::udp::endpoint &client, sf::Packet &packet)
{
    std::string playerId;
    int key;

    packet >> playerId >> key;

    auto idEcsOpt = data_.getPlayerIdEcs(playerId);
    if (!idEcsOpt) {
        std::cerr << "Error: Player ID " << playerId << " does not exist." << std::endl;
        return;
    }

    std::size_t idEcs = *idEcsOpt;

    std::lock_guard<std::mutex> lock(roomsMutex_);
    auto roomIt = std::find_if(rooms_.begin(), rooms_.end(), [&](auto &pair) {
        return std::find_if(pair.second.getPlayers().begin(), pair.second.getPlayers().end(),
                            [&](const Data::Player &player) {
                                return player.id == playerId; // Vérifie si le joueur avec playerId existe dans la liste
                            }) != pair.second.getPlayers().end();
    });


    if (roomIt == rooms_.end()) {
        std::cerr << "Failed to find the room for player " << playerId << "." << std::endl;
        return;
    }

    auto &world = roomIt->second.getWorld();

    if (!world.hasComponent<InputComponent>(idEcs) || !world.hasComponent<VelocityComponent>(idEcs)) {
        std::cerr << "[ERROR] Player " << playerId << " is missing required components." << std::endl;
        return;
    }

    auto &input = world.getComponent<InputComponent>(idEcs);
    auto &velocity = world.getComponent<VelocityComponent>(idEcs);

    input.setAction(false, false, false, false, false);
    velocity.setX(0.0f);
    velocity.setY(0.0f);

    switch (key) {
    case 0: // Droite
        input.setActionRight(true);
        velocity.setX(200.0f);
        break;
    case 1: // Gauche
        input.setActionLeft(true);
        velocity.setX(-200.0f);
        break;
    case 2: // Haut
        input.setActionUp(true);
        velocity.setY(-200.0f);
        break;
    case 3: // Bas
        input.setActionDown(true);
        velocity.setY(200.0f);
        break;
    case 4: // Tir
        input.setActionShoot(true);
        if (world.hasComponent<WeaponComponent>(idEcs)) {
            auto &weapon = world.getComponent<WeaponComponent>(idEcs);
            weapon.setisShooting(true);
        }
        break;
    default:
        std::cerr << "[WARNING] Invalid key received: " << key << std::endl;
        return;
    }

    // std::cout << "[INFO] Processed input for player " << playerId << " with key " << key << std::endl;
}


void GameServer::handleQuitGame(const asio::ip::udp::endpoint &client, sf::Packet &packet)
{
    std::string playerId;
    packet >> playerId;

    std::cout << "Packet quit has been received.\n";

    std::lock_guard<std::mutex> lock(roomsMutex_);
    auto playerOpt = data_.getPlayerIdEcs(playerId);
    if (!playerOpt.has_value()) {
        std::cerr << "Player with ID " << playerId << " does not exist, ignoring QUIT_GAME.\n";
        return;
    }

    std::size_t entityId = playerOpt.value();

    auto roomIt = std::find_if(rooms_.begin(), rooms_.end(), [&](auto &pair) {
        return std::find_if(pair.second.getPlayers().begin(), pair.second.getPlayers().end(),
                            [&](Data::Player &player) {
                                return player.id == playerId;
                            }) != pair.second.getPlayers().end();
    });

    if (roomIt == rooms_.end()) {
        std::cerr << "Failed to find the room for player " << playerId << ".\n";
        return;
    }

    if (!data_.removePlayer(playerId)) {
        std::cerr << "Failed to remove player with ID " << playerId << ".\n";
        return;
    }

    sf::Packet deletePacket;
    deletePacket << DELETE_ENTITY << static_cast<int>(entityId);

    for (const auto &player : roomIt->second.getPlayers()) {
        server.send_to_client(player.endpoint, packetToString(deletePacket));
    }

    // SUPPRIMER DE WOLRD LE PLAYER ET SUPPRIMER DE LA ROOM LE PLAYER
    roomIt->second.getWorld().removeEntity(entityId);
    roomIt->second.delPlayer(playerId);

    std::cout << "Player with ID " << playerId << " and entity ID " << entityId << " has been removed.\n";
}
