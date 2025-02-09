/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** InputSystem.hpp
*/

#pragma once

#include "World.hpp"
#include "ISystem.hpp"

class InputSystem : public ISystem
{
    public:
        static componentMask getRequiredComponents();
        bool shouldRun(const World &world) const override { return true; }
        void update(World &world, float deltaTime) override;
        void handleShooting(World &world, EntityID player);
        void resetShootingState(World &world, EntityID player);
};