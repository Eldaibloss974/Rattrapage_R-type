/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** LifetimeComponent
*/

#include "LifetimeCMPT.hpp"

LifetimeComponent::LifetimeComponent(float lifetime) : AComponent(idComponent::Lifetime), LifeTime(lifetime)
{
}

float LifetimeComponent::getLifeTime() const
{
    return LifeTime;
}

void LifetimeComponent::setLifeTime(float lifeTime)
{
    LifeTime = lifeTime;
}
