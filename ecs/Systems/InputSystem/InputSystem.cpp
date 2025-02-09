/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** InputSystem.cpp
*/

#include "InputSystem.hpp"
#include <chrono>
#include <iostream>
#include <thread>

componentMask InputSystem::getRequiredComponents()
{
    componentMask mask;

    mask.set(int(idComponent::Velocity));
    mask.set(int(idComponent::Input));
    return mask;
}

void InputSystem::resetShootingState(World &world, EntityID player)
{
    if (!world.hasComponent<WeaponComponent>(player))
    {
        return;
    }

    auto &weapon = world.getComponent<WeaponComponent>(player);
    weapon.setisShooting(false);
}

void InputSystem::handleShooting(World &world, EntityID player)
{
    if (!world.hasComponent<WeaponComponent>(player)) {
        return;
    }
    auto &weapon = world.getComponent<WeaponComponent>(player);
    weapon.setisShooting(true);

    // std::cout << "[InputSystem::handleShooting] isShooting set to true for EntityID: " << player << std::endl;

    // std::this_thread::sleep_for(std::chrono::seconds(1));
}

void InputSystem::update(World &world, float deltaTime)
{
    // const auto &entities = world.getEntitiesMatchingMask(getRequiredComponents());
    // int key = -1;

    // for (auto entity : entities) {
    //     auto &input = world.getComponent<InputComponent>(entity);
    //     auto &velocity = world.getComponent<VelocityComponent>(entity);

    //     input.setAction(false, false, false, false, false);

    //     if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
    //         input.setActionUp(true);
    //         key = 2;
    //     } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
    //         input.setActionDown(true);
    //         key = 3;
    //     } else {
    //         velocity.setY(0.0f);
    //     }

    //     if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
    //         input.setActionLeft(true);
    //         velocity.setX(-200.0f);
    //     } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
    //         input.setActionRight(true);
    //         key = 0;
    //     } else {
    //         velocity.setX(0.0f);
    //     }
    //     if (IsKeyPressed(KEY_SPACE)) {
    //         input.setActionShoot(true);
    //         key = 4;
    //     }
    //     if (key != -1) {
    //         sf::Packet packet;
    //         packet << SEND_INPUT << world.client_->playerId_ << key;
    //         world.client_->send_message(packetToString(packet));
    //     }
    // }
}
