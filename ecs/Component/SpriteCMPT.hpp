/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** SpriteComponent
** SpriteComponent
*/

#pragma once

#include "AComponent.hpp"
#include "raylib.h"

#include <tuple>
#include <string>

class SpriteComponent : public AComponent
{
public:
    SpriteComponent(std::string path, float x, float y, float width, float height);
    ~SpriteComponent() = default;

    static idComponent getStaticId() { return idComponent::Sprite; }
    std::string getSpritePath() const;
    Rectangle getsourceRect() const;
    float getWidth() const;
    float getHeight() const;
    void setSprite(std::string spritePath, float width, float height);
    void setSourceRect(float x, float y, float w, float h);

protected:
private:
    std::string SpritePath;
    Rectangle sourceRect;
    float _x;
    float _y;
    float width;
    float height;
};
