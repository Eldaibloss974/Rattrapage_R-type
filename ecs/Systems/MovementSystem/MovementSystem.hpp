/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** MovementSystem.hpp
*/

#pragma once

#include "ISystem.hpp"
#include "World.hpp"

class MovementSystem : public ISystem
{
    public:
        static componentMask getRequiredComponents();
        bool shouldRun(const World &world) const override { return true; }
        void update(World &world, float deltaTime) override;

        void handleLevel(World &world, EntityID &entity);
};