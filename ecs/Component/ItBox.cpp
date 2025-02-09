/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** ItBox
*/

#include "ItBox.hpp"

HitBoxComponent::HitBoxComponent(float w, float h) : AComponent(idComponent::HitBox), width(w), height(h)
{
}

void HitBoxComponent::setHitBoxComponent(float w, float h)
{
    width = w;
    height = h;
}

float HitBoxComponent::getWidth() const
{
    return width;
}

float HitBoxComponent::getHeight() const
{
    return height;
}

