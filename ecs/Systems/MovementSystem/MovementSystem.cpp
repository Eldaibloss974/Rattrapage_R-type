/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** MovementSystem.cpp
*/

#include "MovementSystem.hpp"
#include <cmath>

componentMask MovementSystem::getRequiredComponents()
{
    componentMask mask;

    mask.set(int(idComponent::Position));
    mask.set(int(idComponent::Velocity));
    return mask;
}

void MovementSystem::handleLevel(World &world, EntityID &entity)
{
    auto &position = world.getComponent<PositionComponent>(entity, "Movement 1");
    auto &velocity = world.getComponent<VelocityComponent>(entity);
    auto &sprite = world.getComponent<SpriteComponent>(entity);

    if (world.hasComponent<AiComponent>(entity)) {
        auto &ai = world.getComponent<AiComponent>(entity);
        std::string type = ai.getPath();

        float closestPlayerY = position.getY();
        float closestDistance = std::numeric_limits<float>::max();

        for (auto player : world.getEntities()) {
            if (!world.hasComponent<InputComponent>(player)) {
                continue;
            }
            if (world.isEntityValid(player) && world.hasComponent<PositionComponent>(player)) {
                const auto &playerPos = world.getComponent<PositionComponent>(player, "Movement 2");
                float distance = std::abs(playerPos.getX() - position.getX());
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestPlayerY = playerPos.getY();
                }
            }
        }
        if (type == "enemy_basic") {
            velocity.setX(-50.0f);
            velocity.setY(position.getY() < closestPlayerY ? 50.0f : (position.getY() > closestPlayerY ? -50.0f : 0.0f));
        } else if (type == "enemy_shooter") {
            velocity.setX(-50.0f);
            velocity.setY(position.getY() < closestPlayerY ? 50.0f : (position.getY() > closestPlayerY ? -50.0f : 0.0f));
        } else if (type == "enemy_fast_basic") {
            velocity.setX(-100.0f);
            velocity.setY(position.getY() < closestPlayerY ? 100.0f : (position.getY() > closestPlayerY ? -100.0f : 0.0f));
        } else if (type == "enemy_fast_shooter") {
            velocity.setX(-100.0f);
            velocity.setY(position.getY() < closestPlayerY ? 100.0f : (position.getY() > closestPlayerY ? -100.0f : 0.0f));
        }
    }
}

void MovementSystem::update(World &world, float deltaTime)
{
    const auto &entities = world.getEntitiesMatchingMask(getRequiredComponents());

    constexpr float WINDOW_WIDTH = 800.0f;
    constexpr float WINDOW_HEIGHT = 600.0f;

    for (auto entity : entities) {
        auto &position = world.getComponent<PositionComponent>(entity, "Movement 3");
        auto &velocity = world.getComponent<VelocityComponent>(entity);
        auto &sprite = world.getComponent<SpriteComponent>(entity);

        ////////////////////////////////////////////////////////////////////////

        handleLevel(world, entity);

        float newX = position.getX() + velocity.getX() * deltaTime;
        float newY = position.getY() + velocity.getY() * deltaTime;
        ////////////////////////////////////////////////////////////////////////

        if (world.hasComponent<InputComponent>(entity)) {
            if (newX < 0.0f) {
                newX = 0.0f;
                velocity.setX(0.0f);
            } else if (newX > WINDOW_WIDTH) {
                newX = WINDOW_WIDTH;
                velocity.setX(0.0f);
            }

            if (newY < 0.0f) {
                newY = 0.0f;
                velocity.setY(0.0f);
            } else if (newY > WINDOW_HEIGHT) {
                newY = WINDOW_HEIGHT;
                velocity.setY(0.0f);
            }
        }
        position.setPosition(newX, newY);
        if (world.hasComponent<InputComponent>(entity)) {
            velocity.setX(0.0f);
            velocity.setY(0.0f);
        }
        if (world.hasComponent<ProjectileComponent>(entity)) {
            if (newX >= WINDOW_WIDTH) {
                std::cout << "{DELETED PROJECTILE}: projectile deleted" << std::endl;
                world.destroyEntity(entity);
                world.deletedEntity_.push_back(entity);
                position.setPosition(newX, newY);
            }
            if (newX <= 0) {
                std::cout << "{DELETED PROJECTILE}: projectile deleted" << std::endl;
                world.destroyEntity(entity);
                world.deletedEntity_.push_back(entity);
                position.setPosition(newX, newY);
            }
        }
        if (world.hasComponent<AiComponent>(entity)) {
            if (newX <= 0) {
                std::cout << "{DELETED PROJECTILE}: projectile deleted" << std::endl;
                world.destroyEntity(entity);
                world.deletedEntity_.push_back(entity);
                position.setPosition(newX, newY);
            }
        }
        world.cleanupDestroyedEntities();
    }
}
