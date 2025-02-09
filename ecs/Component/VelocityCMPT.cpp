/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** VelocityComponent
** VelocityComponent
*/

#include "VelocityCMPT.hpp"

VelocityComponent::VelocityComponent(float x, float y) : AComponent(idComponent::Velocity),  _x(x), _y(y)
{
}

float VelocityComponent::getX() const
{
    return _x;
}

float VelocityComponent::getY() const
{
    return _y;
}

void VelocityComponent::setX(float dx)
{
    _x = dx;
}

void VelocityComponent::setY(float dy)
{
    _y = dy;
}

void VelocityComponent::setVelocity(float x, float y)
{
    _x = x;
    _y = y;
}
