/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** RendererSystem.hpp
*/

#pragma once

#include "ISystem.hpp"
#include "TextureManager.hpp"
#include "World.hpp"
#include <iostream>

class RendererSystem : public ISystem {
private:
    TextureManager textureManager;

public:
    RendererSystem();
    static componentMask getRequiredComponents();
    bool shouldRun(const World &world) const override { return true; }
    void update(World &world, float deltaTime) override {
        std::cout << "[RendererSystem] Updating system with deltaTime: " << deltaTime << " seconds." << std::endl;

        const auto &entities = world.getEntitiesMatchingMask(getRequiredComponents());
        std::cout << "[RendererSystem] Total entities to render: " << entities.size() << std::endl;

        float windowWidth = static_cast<float>(GetScreenWidth());
        float windowHeight = static_cast<float>(GetScreenHeight());
        std::cout << "[RendererSystem] Window dimensions: " << windowWidth << "x" << windowHeight << std::endl;

        for (auto it = entities.rbegin(); it != entities.rend(); ++it) {
            auto entity = *it;
            auto &sprite = world.getComponent<SpriteComponent>(entity);

            std::cout << "[RendererSystem] Rendering entity ID: " << entity 
                      << ", Sprite Path: " << sprite.getSpritePath() << std::endl;

            try {
                Texture2D texture = textureManager.getTexture(sprite.getSpritePath());
                auto &position = world.getComponent<PositionComponent>(entity, "Render 1");

                float scaleX = windowWidth / sprite.getWidth();
                float scaleY = windowHeight / sprite.getHeight();

                std::cout << "  [RendererSystem] Entity ID: width " << scaleX << " height" << scaleY << std::endl;

                
                if (world.hasComponent<ParallaxComponent>(entity) && (entity == 0 || entity == 1)) {
                    float x1 = position.getX();

                    Vector2 screenPosition1 = {x1, position.getY()};

                    DrawTextureEx(texture, screenPosition1, 0.0f, std::max(scaleX, scaleY), WHITE);

                    std::cout << "[RendererSystem] Rendered far-layer (ID 0) at positions: "
                              << "(" << screenPosition1.x << ", " << screenPosition1.y << ")" << std::endl;
                }
            } catch (const std::exception &e) {
                std::cerr << "[RendererSystem] Error loading or drawing texture for entity: " << entity
                          << ", Error: " << e.what() << std::endl;
            }
        }
        for (auto it = entities.rbegin(); it != entities.rend(); ++it) {
            auto entity = *it;
            auto &position = world.getComponent<PositionComponent>(entity, "Render 2");
            auto &sprite = world.getComponent<SpriteComponent>(entity);

            if (world.hasComponent<ParallaxComponent>(entity) && (entity == 0 || entity == 1)) {
                continue;
            } else if (world.hasComponent<ParallaxComponent>(entity) && (entity != 0 || entity != 1)) {
                Texture2D texture = textureManager.getTexture(sprite.getSpritePath());
                float scaleX = windowWidth / sprite.getWidth();
                float scaleY = windowHeight / sprite.getHeight();
                std::cout << "  [RendererSystem] Entity ID: " << entity << " has ParallaxComponent. Rendering full-screen." << std::endl;
                Vector2 screenPosition = {position.getX(), position.getY()};
                DrawTextureEx(texture, screenPosition, 0.0f, std::max(scaleX, scaleY), WHITE);
                std::cout << "  [RendererSystem] Rendered full-screen texture for entity ID: " << entity << "." << std::endl;
                continue;
            }
            try {
                Texture2D texture = textureManager.getTexture(sprite.getSpritePath());
                DrawTextureRec(texture, sprite.getsourceRect(), {position.getX(), position.getY()}, WHITE);
            } catch (const std::exception &e) {
                std::cerr << "[RendererSystem] Error loading or drawing texture for entity: " << entity
                        << ", Error: " << e.what() << std::endl;
            }
        }

        std::cout << "[RendererSystem] Rendering completed for this frame." << std::endl;
    }
};