/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** AnimationComponent.hpp
*/

#pragma once

#include "AComponent.hpp"

class AnimationComponent : public AComponent
{
public:
    AnimationComponent(int frameCount, float frameTime, int frameLock = 0)
        : AComponent(idComponent::Animation),
          _frameCount(frameCount),
          _frameTime(frameTime),
          _currentFrame(0),
          _elapsedTime(0.0f),
          _frameLock(frameLock) {}

    static idComponent getStaticId() { return idComponent::Animation; }

    int getFrameCount() const { return _frameCount; }
    float getFrameTime() const { return _frameTime; }
    int getCurrentFrame() const { return _currentFrame; }
    float getElapsedTime() const { return _elapsedTime; }
    int getFrameLock() const { return _frameLock; }

    void addElapsedTime(float deltaTime) { _elapsedTime += deltaTime; }
    void resetElapsedTime() { _elapsedTime = 0.0f; }
    void setFrameCount(int frame_c) { _frameCount = frame_c; }
    void setFrameTime(float frame_t) { _frameTime = frame_t; }
    void setCurrentFrame(int frame) { _currentFrame = frame; }
    void setFrameLock(int frameLock) { _frameLock = frameLock; }

private:
    int _frameCount;
    float _frameTime;
    int _currentFrame;
    float _elapsedTime;
    int _frameLock;
};
