/*
** EPITECH PROJECT, 2024
** Game
** File description:
** GameServer.hpp
*/

#pragma once

#include <vector>
#include <memory>
#include <boost/lockfree/queue.hpp>
#include "Server.hpp"
#include "Data.hpp"
#include <set>
#include "World.hpp"
#include "Room.hpp"
#include "WeaponSystem.hpp"
#include "MovementSystem.hpp"
#include "CollisionSystem.hpp"
#include "LifetimeSystem.hpp"

inline std::string endpointToString(const asio::ip::udp::endpoint &endpoint)
{
    return endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
}

class GameServer {
    private:
        Server server;
        Data data_;

        std::map<sf::Uint8, std::function<void(const asio::ip::udp::endpoint&, sf::Packet&)>> commandHandlers_;

    public:
        GameServer(const std::string &address = "0.0.0.0", unsigned short port = 2121);
        ~GameServer();
        void run();

        void setupCommandHandlers();

        void handleConnectPlayer(const asio::ip::udp::endpoint &client, sf::Packet &packet);
        void handleDisconnectPlayer(const asio::ip::udp::endpoint &client, sf::Packet &packet);
        void handleCreateRoom(const asio::ip::udp::endpoint &client, sf::Packet &packet);
        void handleSearchRoom(const asio::ip::udp::endpoint &client, sf::Packet &packet);
        void handleJoinRoom(const asio::ip::udp::endpoint &client, sf::Packet &packet);
        void handleRequestRoom(const asio::ip::udp::endpoint &client, sf::Packet &packet);
        void handleInput(const asio::ip::udp::endpoint &client, sf::Packet &packet);
        void handleQuitGame(const asio::ip::udp::endpoint &client, sf::Packet &packet);


        void broadcastUpdates();

        std::mutex roomsMutex_;
        std::unordered_map<std::string, Room> rooms_;

        //////// ECS ///////

        struct Joined {
            std::string playerId;
            bool use;
            asio::ip::udp::endpoint client;
            std::string roomName;
            std::string spritePath;
        };

        Joined joined_ = {"NULL", false, asio::ip::udp::endpoint(), "NULL"};

};
