/*
** EPITECH PROJECT, 2024
** Game
** File description:
** main_client.cpp
*/

#include "Network/Client.hpp"
#include "Graphics/Game.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <stdexcept>

int main(int argc, char *argv[])
{
    try {
        std::string serverAddress = (argc >= 2) ? argv[1] : "127.0.0.1";
        unsigned short serverPort = (argc >= 3) ? static_cast<unsigned short>(std::stoi(argv[2])) : 8080;

        std::cout << "Connecting to server at " << serverAddress << ":" << serverPort << std::endl;

        Game game(serverAddress, serverPort);
        game.run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
