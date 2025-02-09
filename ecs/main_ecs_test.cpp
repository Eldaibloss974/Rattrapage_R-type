/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** main.cpp
*/

#include "World.hpp"
#include <iostream>
#include "TextureManager.hpp"
#include "raylib.h"

int main()
{
    const float fixedDeltaTime = 1.0f / 60.0f;
    float accumulator = 0.5f;
    InitWindow(800, 600, "R-Type");
    std::cout << "window is initialized" << std::endl;
    SetTargetFPS(60);

    World world;
    world.initialize();
    std::cout << "world is initialized" << std::endl;
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        accumulator += deltaTime;

        std::cout << "in the loop accumulator : " << accumulator << " fixed delta time: " << fixedDeltaTime << std::endl;
        while(accumulator >= fixedDeltaTime) {
            std::cout << "in the system loop" << std::endl;
            world.updateSystems(deltaTime);
            accumulator -= fixedDeltaTime;
        }
    }
    CloseWindow();
}
