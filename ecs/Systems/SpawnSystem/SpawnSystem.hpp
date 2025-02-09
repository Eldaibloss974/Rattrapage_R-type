/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** SpawnSystem.hpp
*/

#pragma once

#include "ISystem.hpp"
#include "World.hpp"

class SpawnSystem : public ISystem
{
    private:
        void spawnEntity(World &world, const SpawnerComponent &spawner, const PositionComponent &position);
    public:
        static componentMask getRequiredComponents();
        bool shouldRun(const World &world) const override { return true; }
        void update(World &world, float deltaTime) override {
            // std::cout << "[SpawnSystem] Updating system with deltaTime: " << deltaTime << " seconds." << std::endl;

            const auto &entities = world.getEntitiesMatchingMask(getRequiredComponents());
            // std::cout << "[SpawnSystem] Total matching entities: " << entities.size() << std::endl;

            for (auto entity : entities) {
                // std::cout << "[SpawnSystem] Processing entity ID: " << entity << std::endl;

                auto &spawner = world.getComponent<SpawnerComponent>(entity);
                auto &position = world.getComponent<PositionComponent>(entity, "Spawn 1");

                // std::cout << "[SpawnSystem] Entity ID: " << entity
                //           << ", Current Cooldown: " << spawner.getCD()
                //           << ", Spawn Interval: " << spawner.getInterval()
                //           << std::endl;

                // std::cout << "[SpawnSystem] Updated Cooldown: " << deltaTime << std::endl;
                spawner.setCooldown(spawner.getCD() - deltaTime);
                // std::cout << "[SpawnSystem] Updated Cooldown: " << spawner.getCD() << std::endl;

                if (spawner.getCD() <= 0.0f) {
                    spawnEntity(world, spawner, position);
                    spawner.setCooldown(spawner.getInterval());
                    // std::cout << "[SpawnSystem] Cooldown reset to: " << spawner.getInterval() << " seconds." << std::endl;
                }
            }
        }


};
