/*
** EPITECH PROJECT, 2024
** Game
** File description:
** main_server.cpp
*/

#include "Network/Server.hpp"
#include "Server/GameServer.hpp"
#include <iostream>
#include <csignal>

int main(int argc, char *argv[])
{
    try {
        
        std::string serverAddress = "0.0.0.0";
        unsigned short serverPort = (argc >= 2) ? static_cast<unsigned short>(std::stoi(argv[1])) : 8080;
        std::cout << "Starting server on " << serverAddress << ":" << serverPort << std::endl;
        GameServer gameServer(serverAddress, serverPort);
        gameServer.run();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
