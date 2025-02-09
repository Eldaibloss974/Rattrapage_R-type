/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** AnimationSystem.cpp
*/

#include "AnimationSystem.hpp"


componentMask AnimationSystem::getRequiredComponents()
{
    componentMask mask;
    mask.set(AnimationComponent::getStaticId());
    mask.set(SpriteComponent::getStaticId());
    return mask;
}


void AnimationSystem::updateFrame(AnimationComponent &animation, float deltaTime)
{
    animation.addElapsedTime(deltaTime);

    std::cout << "[AnimationSystem::updateFrame] Elapsed Time: " << animation.getElapsedTime()
              << "/" << animation.getFrameTime() << ", Current Frame: " << animation.getCurrentFrame()
              << ", FrameLock: " << animation.getFrameLock() << std::endl;

    if (animation.getElapsedTime() >= animation.getFrameTime()) {
        animation.resetElapsedTime();

        int currentFrame = animation.getCurrentFrame();
        int frameCount = animation.getFrameCount();
        int frameLock = animation.getFrameLock();

        if (currentFrame == frameCount - 1) {
            animation.setCurrentFrame(frameLock);
            std::cout << "[AnimationSystem::updateFrame] Animation looped to FrameLock: " << frameLock << std::endl;
        } else {
            animation.setCurrentFrame(currentFrame + 1);
            std::cout << "[AnimationSystem::updateFrame] Frame updated to: " << currentFrame + 1 << std::endl;
        }
    }
}

