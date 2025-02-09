/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** Game.cpp
*/

#include "Game.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <utility>
#include "raylib.h"

Game::Game(const std::string &serverAddress, unsigned short serverPort)
    : packetQueue_(20000), isRunning_(true), menu_(client)
{
    srand(static_cast<unsigned>(time(nullptr)));
    client.set_on_message([this](const std::string &message) {
        auto *packet = new sf::Packet(stringToPacket(message));
        this->packetQueue_.push(packet);
    });
    client.set_on_disconnect([this]() {
        std::cout << "Disconnected from server." << std::endl;
        isRunning_ = false;
    });

    try {
        client.start(serverAddress, serverPort);
    } catch (const std::exception &e) {
        isRunning_ = false;
        throw std::runtime_error("Failed to connect server: " + std::string(e.what()));
    }
}

Game::~Game()
{
    client.stop();
}

void Game::run()
{
    const float fixedDeltaTime = 1.0f / 60.0f;
    float accumulator = 0.0f;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "R-Type");
    SetTargetFPS(60);

    try {
        while (isRunning_) {
            float deltaTime = GetFrameTime();
            if (WindowShouldClose()) {
                menu_.disconnectClient();
                break;
            }
            if (client.returnToMenu_) {
                break;
            }
            accumulator += deltaTime;
            int screenWidth = GetScreenWidth();
            int screenHeight = GetScreenHeight();
            if (screenWidth < 400 || screenHeight < 300) {
                SetWindowSize(400, 300);
            }
            BeginDrawing();
            ClearBackground(BLACK);
            while (accumulator >= fixedDeltaTime) {
                accumulator -= fixedDeltaTime;
                menu_.handleInput(fixedDeltaTime);
                menu_.update();
            }
            menu_.draw();
            EndDrawing();
            handlePacket();
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught!" << std::endl;
    }
    CloseWindow();
    std::cout << "Game closed cleanly." << std::endl;
}


void Game::handlePacket()
{
    menu_.handlePacket(packetQueue_, isRunning_);
}
