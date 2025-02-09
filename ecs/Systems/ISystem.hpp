/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** ISystem.hpp
*/

#pragma once

class World;

class ISystem {
    public:
        virtual ~ISystem() = default;
        virtual void update(World& world, float deltaTime) = 0;
        virtual bool shouldRun(const World& world) const { return true; }
};
