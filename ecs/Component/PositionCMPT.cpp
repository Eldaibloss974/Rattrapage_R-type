/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** PositionComponent
*/

#include "PositionCMPT.hpp"

PositionComponent::PositionComponent(float x, float y) : AComponent(idComponent::Position), _x(x), _y(y)
{
}

float PositionComponent::getX() const
{
    return _x;
}

float PositionComponent::getY() const
{
    return _y;
}

void PositionComponent::setPosition(float x, float y)
{
    _x = x;
    _y = y;
}
