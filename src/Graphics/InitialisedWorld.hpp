/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** InitialisedWorld.hpp
*/

#pragma once

#include "../Network/Client.hpp"
#include <iostream>
#include <unordered_map>
#include <string>
#include "raylib.h"
#include "World.hpp"
#include <boost/lockfree/queue.hpp>
#include "SoundManager.hpp"

class InitialiseWorld {
public:
    InitialiseWorld(Client &client);
    ~InitialiseWorld();

    SoundManager &getSoundManager() { return soundManager_; }

    void updates(float &deltaTime);
    void handlePacket(boost::lockfree::queue<sf::Packet*> &packetQueue_);
    void draw();
    void updateInput(float &deltaTime);
    void resetWorld();
    void updateScoreHUD();

    World world_;
    int score_ = 0;
    bool scoreChanged = false;
    int additional = 0;

private:
    SoundManager soundManager_;
    Client &client_;

    bool showAdditionalEffect = false;
    float effectDuration = 1.5f;
    float effectTimer = 0.0f;
    Vector2 effectPosition = { 100.0f, 100.0f };
};
