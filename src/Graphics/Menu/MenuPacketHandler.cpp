/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** MenuPacketHandler.cpp
*/

#include "Menu.hpp"
#include <list>
#include <string>

void Menu::initializePacketHandlers()
{
    packetHandlers_ = {
        {ACCEPT_CONNECT_PLAYER, [this](sf::Packet &packet) { handleAcceptConnectPlayer(packet); }},
        {ACCEPT_DISCONNECT_PLAYER, [this](sf::Packet &packet) { handleAcceptDisconnectPlayer(packet); }},
        {NOT_ACCEPT_CONNECT_PLAYER, [this](sf::Packet &packet) { handleNotAcceptConnectPlayer(packet); }},
        {ACCEPT_CREATE_ROOM, [this](sf::Packet &packet) { handleAcceptCreateRoom(packet); }},
        {NOT_ACCEPT_CREATE_ROOM, [this](sf::Packet &packet) { handleNotAcceptCreateRoom(packet); }},
        {RECEIVE_ROOMS, [this](sf::Packet &packet) { handleReceiveRooms(packet); }},
        {JOIN_ROOM_SUCCESS, [this](sf::Packet &packet) { handleJoinRoomSuccess(packet); }},
        {JOIN_ROOM_FAILURE, [this](sf::Packet &packet) { handleJoinRoomFailure(packet); }},
        {CREATE_ENTITY, [this](sf::Packet &packet) {handleCreateEntity(packet);}},
        {POSITION_ENTITY, [this](sf::Packet &packet) {handlePositionEntity(packet);}},
        {DELETE_ENTITY, [this](sf::Packet &packet) {handleDeleteEntity(packet);}},
        {GAME_OVER, [this](sf::Packet &packet) {handleGameOver(packet);}},
        {SCORE_PLAYER, [this](sf::Packet &packet) { handleScorePlayer(packet); }}
    };
}

void Menu::processPacket(sf::Packet &packet)
{
    sf::Uint8 action;
    packet >> action;

    auto it = packetHandlers_.find(action);
    if (it != packetHandlers_.end()) {
        it->second(packet);
    } else {
        std::cout << "Unhandled packet action: " << static_cast<int>(action) << std::endl;
    }
}

void Menu::handlePacket(boost::lockfree::queue<sf::Packet*> &packetQueue_, bool &isRunning)
{
    //// GAME ////
    isRunning_ = isRunning;
    packetQueue_.consume_all([&](sf::Packet *packet) {
        processPacket(*packet);
        isRunning = isRunning_;
        delete packet;
    });
}

/////////////////////////////// COMMANDS /////////////////////////////////////////////////////////

void Menu::handleAcceptConnectPlayer(sf::Packet &packet)
{
    if (isInGame) {
        return;
    }

    packet >> clientId_;
    connected_ = true;
    showLoginScreen_ = false;
    errorMessage_.clear();
    std::cout << "Connected as " << clientId_ << "." << std::endl;
    client_.playerId_ = clientId_;
}

void Menu::handleNotAcceptConnectPlayer(sf::Packet &packet)
{
    if (isInGame) {
        return;
    }
    errorMessage_ = "Connection refused: invalid ID or already connected.";
    std::cout << "Connection refused for ID: " << clientId_ << std::endl;
}

void Menu::handleAcceptDisconnectPlayer(sf::Packet &packet)
{
    isRunning_ = false;
    connected_ = false;
    std::cout << "Disconnected successfully." << std::endl;
}

void Menu::handleAcceptCreateRoom(sf::Packet &packet)
{
    if (isInGame) {
        return;
    }

    std::string roomName;
    int level = 0;

    packet >> roomName >> level;
    std::cout << "Room created: " << roomName << " at level " << level << "." << std::endl;
    showCreateRoomScreen_ = false;
    errorMessage_.clear();
    roomName_.clear();
    roomLevel_ = 1;
    std::cout << "Returning to main menu after successful room creation." << std::endl;
}

void Menu::handleNotAcceptCreateRoom(sf::Packet &packet)
{
    if (isInGame) {
        return;
    }

    std::string errorMsg;
    packet >> errorMsg;
    errorMessage_ = errorMsg;
    std::cout << "Failed to create room: " << errorMsg << std::endl; 
}

void Menu::handleReceiveRooms(sf::Packet &packet)
{
    if (isInGame) {
        return;
    }

    int roomCount;
    packet >> roomCount;
    rooms_.clear();
    for (int i = 0; i < roomCount; ++i) {
        Room room;
        packet >> room.name >> room.level >> room.players;
        rooms_.push_back(room);
        std::cout << "Room " << room.name << " Level " << room.level << " Players " << room.players << std::endl;
    }
    errorMessage_.clear();
    std::cout << "Rooms received: " << roomCount << " rooms." << std::endl;
}

void Menu::handleJoinRoomSuccess(sf::Packet &packet)
{
    if (isInGame) {
        return;
    }

    std::string roomName;
    packet >> roomName;
    std::cout << "Successfully joined room: " << roomName << std::endl;
    isInGame = true;
}

void Menu::handleJoinRoomFailure(sf::Packet &packet)
{
    if (isInGame) {
        return;
    }

    std::string errorMsg;
    packet >> errorMsg;
    errorMessage_ = errorMsg;
    std::cout << "Failed to join room: " << errorMsg << std::endl;
}

void Menu::handleCreateEntity(sf::Packet &packet)
{
    std::string type;

    if (!isInGame) {
        return;
    }
    float posX, posY;
    std::string spritePath;
    std::size_t id;
    int id32;
    std::string script;
    packet >> id32 >> posX >> posY >> spritePath >> type >> script;
    id = id32;

    if (type == "PLAYER") {
        // std::cout << "{PLAYER} create with id: " << id << " and PATH = " << spritePath << std::endl;
        world_.world_.createPlayerCli(spritePath, WeaponType::SINGLE_SHOT, id);
    } else if (type == "BACKGROUND") {
        // std::cout << "{BACKGROUND} create with id: " << id << " and PATH = " << spritePath << std::endl;
        world_.world_.createParallaxLayerCli(spritePath, posX, posY, 0.0f, 0.0f, 272.0f, 160.0f, id);
    } else if (type == "PROJECTILE") {
        std::cout << "{PROJECTILE} create with id: " << id << " and PATH = " << spritePath << std::endl;
        world_.world_.createProjectileCli(posX, posY, 0, 0.0f, 0, spritePath, 2, 0.2f, 1, id);
        world_.getSoundManager().playSound("../assets/laser.mp3");
    } else if (type == "ENEMY") {
        // std::cout << "{ENEMY} create with id: " << id << " and PATH = " << spritePath << std::endl;
        world_.world_.createEnemyCli(posX, posY, spritePath, id, 4, 0.2f, 0, script);
    } else if (type == "EFFECT") {
        world_.world_.createOnHitEffectCli(posX, posY, spritePath, 4, 0.2f, 1.6f, 4, id);
        world_.getSoundManager().playSound("../assets/medium-explosion.mp3");

    } else {
        std::cout << "[WARNING] Unknown entity type: " << type << std::endl;
    }
    // world_.world_.printAllEntitiesDetails();
}


void Menu::handlePositionEntity(sf::Packet &packet)
{
    if (!isInGame) {
        return;
    }

    while (!packet.endOfPacket()) {
        int entityId;
        float x, y;

        if (!(packet >> entityId >> x >> y)) {
            std::cerr << "[ERROR] Failed to read entity data from packet." << std::endl;
            return;
        }

        if (!world_.world_.doesEntityExist(entityId)) {
            std::cerr << "[WARNING] Entity " << entityId << " does not exist in the world." << std::endl;
            continue;
        }

        if (world_.world_.hasComponent<PositionComponent>(entityId)) {
            auto &position = world_.world_.getComponent<PositionComponent>(entityId);
            position.setPosition(x, y);
            std::cout << "[INFO] Updated entity ID " << entityId 
                      << " position to (" << x << ", " << y << ")" << std::endl;
        } else {
            std::cerr << "[WARNING] Entity " << entityId << " is missing a PositionComponent." << std::endl;
        }
    }
}

void Menu::handleDeleteEntity(sf::Packet &packet)
{
    int entityId;

    if (!isInGame) {
        return;
    }
    packet >> entityId;
    std::cout << "REMOVING ENTITY " << entityId << std::endl;
    world_.world_.removeEntity(entityId);
}

void Menu::handleGameOver(sf::Packet & packet)
{
    world_.resetWorld();
}

void Menu::handleScorePlayer(sf::Packet &packet)
{
    if (!isInGame) {
        return;
    }
    int entityId;
    int score;

    if (!(packet >> entityId >> score)) {
        std::cerr << "[ERROR] Failed to read SCORE_PLAYER packet." << std::endl;
        return;
    }

    if (!world_.world_.doesEntityExist(entityId)) {
        std::cerr << "[WARNING] Entity " << entityId << " does not exist in the world." << std::endl;
        return;
    }

    if (score > world_.score_) {
        world_.additional = score - world_.score_;
        world_.score_ = score;
        world_.scoreChanged = true;
    }

    if (world_.world_.hasComponent<ScoreComponent>(entityId)) {
        auto &scoreComponent = world_.world_.getComponent<ScoreComponent>(entityId);
        scoreComponent.setScore(score);
        world_.score_ = score;
        std::cout << "[INFO] Updated score for entity ID " << entityId 
                  << " to " << scoreComponent.getScore() << std::endl;
    } else {
        std::cerr << "[WARNING] Entity " << entityId << " is missing a ScoreComponent." << std::endl;
    }
}
