/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** InitialisedWorld.cpp
*/

#include "InitialisedWorld.hpp"
#include "RendererSystem.hpp"
#include "InputSystem.hpp"
#include "AnimationSystem.hpp"
#include "ParallaxSystem.hpp"
#include "SpawnerComponent.hpp"

InitialiseWorld::InitialiseWorld(Client &client) : client_(client)
{
    soundManager_.loadBackgroundMusic("../assets/03 Counterattack '91.mp3");
    soundManager_.playBackgroundMusic(0.5f);
    world_.addSystem(std::make_unique<InputSystem>());
    world_.addSystem(std::make_unique<AnimationSystem>());
    world_.addSystem(std::make_unique<RendererSystem>());
}

InitialiseWorld::~InitialiseWorld()
{
}

void InitialiseWorld::updates(float &deltaTime)
{
    soundManager_.updateBackgroundMusic();
    updateInput(deltaTime);
    world_.updateSystems(deltaTime);

    updateScoreHUD();
}

void InitialiseWorld::updateScoreHUD()
{
    if (scoreChanged) {
        showAdditionalEffect = true;
        effectTimer = 0.0f;
        scoreChanged = false;
    }

    std::string scoreString = "Score: " + std::to_string(score_);
    DrawText(scoreString.c_str(), 10, 10, 20, WHITE);
    
    if (showAdditionalEffect) {
        effectTimer += GetFrameTime();

        if (effectTimer <= effectDuration) {
            Vector2 effectCurrentPos = effectPosition;
            effectCurrentPos.y -= 50.0f * (effectTimer / effectDuration); // Décalage vers le haut
            std::string additionalString = "+" + std::to_string(additional);
            DrawText(additionalString.c_str(), effectCurrentPos.x, effectCurrentPos.y, 20, RED);
        } else {
            showAdditionalEffect = false;
        }
    }
}


void InitialiseWorld::updateInput(float &deltaTime)
{
    componentMask mask;

    mask.set(int(idComponent::Velocity));
    mask.set(int(idComponent::Input));

    const auto &entities = world_.getEntitiesMatchingMask(mask);
    int key = -1;

    if (IsKeyPressed(KEY_ESCAPE)) {
        resetWorld();
    }

    for (auto entity : entities) {
        auto &input = world_.getComponent<InputComponent>(entity);
        auto &velocity = world_.getComponent<VelocityComponent>(entity);

        auto &anim = world_.getComponent<AnimationComponent>(entity);

        input.setAction(false, false, false, false, false);

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            input.setActionUp(true);
            
            key = 2;
        } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            input.setActionDown(true);
            key = 3;
        } else {
            velocity.setY(0.0f);
        }

        if (IsKeyDown(KEY_Q) || IsKeyDown(KEY_LEFT)) {
            input.setActionLeft(true);
            velocity.setX(-200.0f);
            key = 1;
        } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            input.setActionRight(true);
            key = 0;
        } else {
            velocity.setX(0.0f);
        }
        if (IsKeyPressed(KEY_SPACE)) {
            input.setActionShoot(true);
            key = 4;
        }
        if (key != -1) {
            sf::Packet packet;
            packet << SEND_INPUT << client_.playerId_ << key;
            client_.send_message(packetToString(packet));
        }
    }
}

void InitialiseWorld::resetWorld()
{
    world_ = World();

    world_.addSystem(std::make_unique<InputSystem>());
    world_.addSystem(std::make_unique<ParallaxSystem>());
    world_.addSystem(std::make_unique<AnimationSystem>());
    world_.addSystem(std::make_unique<RendererSystem>());

    sf::Packet packetQuit;

    packetQuit << QUIT_GAME << client_.playerId_;
    client_.send_message(packetToString(packetQuit));
    client_.returnToMenu_ = true;
    std::cout << "World has been reset!" << std::endl;
}

void InitialiseWorld::handlePacket(boost::lockfree::queue<sf::Packet *> &packetQueue_)
{
}

void InitialiseWorld::draw()
{
}
