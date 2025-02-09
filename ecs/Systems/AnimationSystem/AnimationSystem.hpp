/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** Animation.hpp
*/

#pragma once

#include "ISystem.hpp"
#include "World.hpp"

class AnimationSystem : public ISystem
{
    public:
        static componentMask getRequiredComponents();


        void update(World &world, float deltaTime) override
        {
            std::cout << "[AnimationSystem] Updating system with deltaTime: " << deltaTime << " seconds." << std::endl;

            const auto &entities = world.getEntitiesMatchingMask(getRequiredComponents());
            std::cout << "[AnimationSystem] Total entities with AnimationComponent and SpriteComponent: " << entities.size() << std::endl;

            for (auto entity : entities)
            {
                std::cout << "[AnimationSystem] Processing entity ID: " << entity << std::endl;

                auto &animation = world.getComponent<AnimationComponent>(entity);
                auto &sprite = world.getComponent<SpriteComponent>(entity);

                if (world.hasComponent<InputComponent>(entity)) {
                    auto &input = world.getComponent<InputComponent>(entity);

                    if (input.getActionUp()) {
                        animation.setFrameCount(5);
                        animation.setFrameLock(4);
                    } else {
                        animation.setFrameCount(1);
                        animation.setFrameLock(0);
                        animation.setCurrentFrame(0);
                    }

                    updateFrame(animation, deltaTime);
                } 
                else {
                    updateFrame(animation, deltaTime);
                }

                int currentFrame = animation.getCurrentFrame();
                float frameWidth = sprite.getWidth();
                float frameX = currentFrame * frameWidth;
                float frameY = sprite.getsourceRect().y;

                sprite.setSourceRect(frameX, frameY, frameWidth, sprite.getHeight());

                std::cout << "[AnimationSystem] Updated Entity ID: " << entity
                          << ", New Frame: " << animation.getCurrentFrame()
                          << ", Source Rect: (x=" << frameX << ", y=" << frameY
                          << ", width=" << frameWidth << ", height=" << sprite.getHeight() << ")" << std::endl;
            }
        }


    private:
        void updateFrame(AnimationComponent &animation, float deltaTime);

};
