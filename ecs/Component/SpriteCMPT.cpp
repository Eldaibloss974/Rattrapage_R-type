/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** SpriteComponent
*/

#include "SpriteCMPT.hpp"
#include <iostream>

SpriteComponent::SpriteComponent(std::string path, float x, float y, float w, float h): AComponent(idComponent::Sprite), SpritePath(path), sourceRect{x, y, w, h}, _x(x), _y(y), width(w), height(h)
{
    // std::cout << "[SpriteComponent] Created with path: " << path 
    //               << ", Source Rect: (" << x << ", " << y << ", " << w << ", " << h << ")" << std::endl;
}


std::string SpriteComponent::getSpritePath() const
{
    return SpritePath;
}

Rectangle SpriteComponent::getsourceRect() const
{
    return sourceRect;
}

float SpriteComponent::getWidth() const
{
    return width;
}

float SpriteComponent::getHeight() const
{
    return height;
}

void SpriteComponent::setSprite(std::string spritePath, float w, float h)
{
    SpritePath = spritePath;
    width = w;
    height = h;
}

void SpriteComponent::setSourceRect(float x, float y, float w, float h)
{
    sourceRect = {x,y,w,h};
}