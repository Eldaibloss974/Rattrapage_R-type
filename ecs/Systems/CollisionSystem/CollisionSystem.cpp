/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** CollisionSystem.cpp
*/

#include "CollisionSystem.hpp"
#include <chrono>
#include <iostream>
#include <thread>

componentMask CollisionSystem::getRequiredComponents()
{
    componentMask mask;

    mask.set(int(idComponent::Position));
    mask.set(int(idComponent::HitBox));
    return mask;
}

bool CollisionSystem::isColliding(World &world, EntityID entityA, EntityID entityB)
{
    auto &positionA = world.getComponent<PositionComponent>(entityA);
    auto &colliderA = world.getComponent<HitBoxComponent>(entityA);
    auto &spriteA = world.getComponent<SpriteComponent>(entityA);

    auto &positionB = world.getComponent<PositionComponent>(entityB);
    auto &colliderB = world.getComponent<HitBoxComponent>(entityB);
    auto &spriteB = world.getComponent<SpriteComponent>(entityB);

    float x1 = positionA.getX();
    float y1 = positionA.getY();
    float w1 = colliderA.getWidth() * (spriteA.getWidth() / spriteA.getsourceRect().width);
    float h1 = colliderA.getHeight() * (spriteA.getHeight() / spriteA.getsourceRect().height);

    float x2 = positionB.getX();
    float y2 = positionB.getY();
    float w2 = colliderB.getWidth() * (spriteB.getWidth() / spriteB.getsourceRect().width);
    float h2 = colliderB.getHeight() * (spriteB.getHeight() / spriteB.getsourceRect().height);

    bool result = x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;

    // std::cout << "[CollisionSystem::isColliding] "
    //           << "EntityA: (x=" << x1 << ", y=" << y1 << ", w=" << w1 << ", h=" << h1 << "), "
    //           << "EntityB: (x=" << x2 << ", y=" << y2 << ", w=" << w2 << ", h=" << h2 << ") -> "
    //           << (result ? "Collision Detected" : "No Collision") << std::endl;

    return result;
}


void CollisionSystem::handleCollision(World &world, const EntityID entityA, const EntityID entityB)
{
    // std::cout << "[CollisionSystem::handleCollision] Collision detected between EntityA: " << entityA 
    //           << " and EntityB: " << entityB << std::endl;
    if (world.hasComponent<HealthComponent>(entityB)) {
        auto &health = world.getComponent<HealthComponent>(entityB);
        auto &position = world.getComponent<PositionComponent>(entityB, "Collision 1");
        auto &sprite = world.getComponent<SpriteComponent>(entityA);
        auto new_health = health.getHealth() - 10;
        // std::cout << "  [HealthComponent] EntityB Health reduced to: " << new_health << std::endl;

        if (world.hasComponent<ProjectileComponent>(entityA)) {
            world.createOnHitEffect(position.getX(), position.getY(), "../assets/sprites/projectiles1/explosion.gif", 4, 0.4f, 1.6f);
        }

        health.setHealth(new_health);
        if (health.getHealth() <= 0) {
            if (world.hasComponent<AiComponent>(entityB)) {
                if (world.hasComponent<ProjectileComponent>(entityA)) {
                    auto &pro = world.getComponent<ProjectileComponent>(entityA);
                    if (world.hasComponent<InputComponent>(pro.getOwner())) {
                        auto &score = world.getComponent<ScoreComponent>(pro.getOwner());
                        score.addScore(10);
                        world.newScoreId_.push_back(pro.getOwner());
                    }
                }
            }
            world.destroyEntity(entityB);
            world.deletedEntity_.push_back(entityB);
            // std::cout << "  [Entity Destroyed] EntityB: " << entityB << " has been destroyed (Health <= 0)." << std::endl;
        }
    }

    if (world.hasComponent<InputComponent>(entityA) && world.hasComponent<AiComponent>(entityB) && !world.isEntityDestroyed(entityA) && !world.isEntityDestroyed(entityB)) {
        world.destroyEntity(entityA);
        world.deletedEntity_.push_back(entityA);
    }

    if (world.hasComponent<InputComponent>(entityB) && world.hasComponent<AiComponent>(entityA) && !world.isEntityDestroyed(entityA) && !world.isEntityDestroyed(entityB)) {
        world.destroyEntity(entityB);
        world.deletedEntity_.push_back(entityB);
    }

    if (world.hasComponent<ProjectileComponent>(entityA)) {
        auto &pro = world.getComponent<ProjectileComponent>(entityA);
        if (world.hasComponent<InputComponent>(pro.getOwner())) {
            auto &score = world.getComponent<ScoreComponent>(pro.getOwner());
            score.addScore(1);
            world.newScoreId_.push_back(pro.getOwner());
        }
        world.destroyEntity(entityA);
        world.deletedEntity_.push_back(entityA);
        // std::cout << "  [Entity Destroyed] EntityA: " << entityA << " (Projectile) has been destroyed." << std::endl;
    }
}

void CollisionSystem::update(World &world, float deltaTime)
{
    const auto &entities = world.getEntitiesMatchingMask(getRequiredComponents());
    // std::cout << "[CollisionSystem::update] Total Entities to check: " << entities.size() << std::endl;

    for (size_t i = 0; i < entities.size(); ++i) {
        auto &entityA = entities[i];

        if (world.isEntityDestroyed(entityA)) {
            // std::cout << "  [Entity Skipped] EntityA: " << entityA << " is already destroyed." << std::endl;
            continue;
        }

        auto &positionA = world.getComponent<PositionComponent>(entityA, "Collision 2");
        auto &colliderA = world.getComponent<HitBoxComponent>(entityA);

        for (size_t j = i + 1; j < entities.size(); ++j) {
            auto &entityB = entities[j];


            if (world.isEntityDestroyed(entityB)) {
                // std::cout << "  [Entity Skipped] EntityB: " << entityB << " is already destroyed." << std::endl;
                continue;
            }

            if (world.hasComponent<ProjectileComponent>(entityA)) {
                auto &projectile = world.getComponent<ProjectileComponent>(entityA);
                if (world.hasComponent<AiComponent>(projectile.getOwner()) && world.hasComponent<AiComponent>(entityB)) {
                    continue;
                }
                if (world.hasComponent<ProjectileComponent>(entityB)) {
                    continue;
                }
                if (world.hasComponent<InputComponent>(projectile.getOwner()) && world.hasComponent<InputComponent>(entityB)) {
                    continue;
                }
            }

            if (world.hasComponent<AiComponent>(entityA) && world.hasComponent<AiComponent>(entityB)) {
                continue;
            }

            if (world.hasComponent<InputComponent>(entityA) && world.hasComponent<InputComponent>(entityB)) {
                continue;
            }

            auto &positionB = world.getComponent<PositionComponent>(entityB, "Collision 3");
            auto &colliderB = world.getComponent<HitBoxComponent>(entityB);

            // std::cout << "[CollisionSystem::update] Checking collision between EntityA: " << entityA
            //           << " and EntityB: " << entityB << std::endl;

            if (isColliding(world, entityA, entityB)) {
                handleCollision(world, entityA, entityB);
            }
        }
    }
    world.cleanupDestroyedEntities();
}
