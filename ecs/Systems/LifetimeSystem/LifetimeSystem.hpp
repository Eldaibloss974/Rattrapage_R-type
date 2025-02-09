/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** LifetimeSystem.hpp
*/

#pragma once

#include "ISystem.hpp"
#include "World.hpp"


class LifetimeSystem : public ISystem
{
    public:
        static componentMask getRequiredComponents()
        {
            componentMask mask;
            mask.set(LifetimeComponent::getStaticId());
            return mask;
        }

        void update(World &world, float deltaTime) override
        {
        
            const auto &entities = world.getEntitiesMatchingMask(getRequiredComponents());
        
            for (auto entity : entities) {
                auto &lifetime = world.getComponent<LifetimeComponent>(entity);
        
                float oldLifetime = lifetime.getLifeTime();
                float newLifetime = oldLifetime - deltaTime;
        
                lifetime.setLifeTime(newLifetime);
        
                if (newLifetime <= 0.0f) {
                    world.destroyEntity(entity);
                    world.deletedEntity_.push_back(entity);
                }
            }
            world.cleanupDestroyedEntities();        
        }

};