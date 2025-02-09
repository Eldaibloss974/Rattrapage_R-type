/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** CollisionSystem.hpp
*/

#pragma once

#include "ISystem.hpp"
#include "World.hpp"

class CollisionSystem : public ISystem
{
public:
    static componentMask getRequiredComponents();
    bool shouldRun(const World &world) const override { return true; }
    void update(World &world, float deltaTime) override;
    bool isColliding(World &world, EntityID entityA, EntityID entityB);
    void handleCollision(World &world, const EntityID entityA, const EntityID entityB);
};
