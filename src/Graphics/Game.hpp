/*
** EPITECH PROJECT, 2024
** Game
** File description:
** Game.hpp
*/

#pragma once


#include <vector>
#include <memory>
#include <boost/lockfree/queue.hpp>
#include "../Network/Client.hpp"
#include "Menu/Menu.hpp"
#include "SoundManager.hpp"

class Game {
    private:
        Client client;
        boost::lockfree::queue<sf::Packet*> packetQueue_;
        bool isRunning_;
        void handlePacket();

    public:
        Game(const std::string &serverAddress = "127.0.0.1", unsigned short serverPort = 2121);
        ~Game();
        Menu menu_;
        void run();
};

