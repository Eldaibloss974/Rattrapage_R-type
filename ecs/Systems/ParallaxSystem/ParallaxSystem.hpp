/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** ParallaxSystem.hpp
*/

#pragma once
#include "ParallaxSystem.hpp"



class ParallaxSystem : public ISystem {
    public:
        componentMask getRequiredComponents() {
            componentMask mask;
            mask.set(ParallaxComponent::getStaticId());
            mask.set(PositionComponent::getStaticId());
            return mask;
        }

        void update(World &world, float deltaTime) {
            const auto &entities = world.getEntitiesMatchingMask(getRequiredComponents());

            float windowWidth = static_cast<float>(GetScreenWidth());

            for (auto entity : entities) {
                if (entity == 0 || entity == 1) {
                    auto &parallax = world.getComponent<ParallaxComponent>(entity);
                    auto &position = world.getComponent<PositionComponent>(entity, "Parallax 1");
                    auto &sprite = world.getComponent<SpriteComponent>(entity);

                    float textureWidth = sprite.getWidth();
                    float newX = position.getX() + parallax.getScrollSpeedX() * deltaTime;

                    if (newX + textureWidth <= 0.0f) {
                        int otherEntityID = (entity == 0) ? 1 : 0;
                        auto &otherPosition = world.getComponent<PositionComponent>(otherEntityID, "Parallax 2");

                        newX = otherPosition.getX() + textureWidth;
                    }

                    position.setPosition(newX, position.getY());
                } else {
                    auto &parallax = world.getComponent<ParallaxComponent>(entity);
                    auto &position = world.getComponent<PositionComponent>(entity, "Parallax 3");
                    
                    float newX = position.getX() + parallax.getScrollSpeedX() * deltaTime;
                    float newY = position.getY() + parallax.getScrollSpeedY() * deltaTime;
                    if (newX < -800.0f) {
                        newX += 1600.0f;
                    }
                    position.setPosition(newX, newY);
                }
            }
        }
};