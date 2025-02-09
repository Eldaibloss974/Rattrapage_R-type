/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** SpawnerComponent.cpp
*/

#include "SpawnerComponent.hpp"


SpawnerComponent::SpawnerComponent(std::string type, float interval, float cd, int level) : AComponent(idComponent::AI), EntityType(type), _interval(interval), _cooldown(cd), level_(level)
{

}

void SpawnerComponent::setInterval(float new_interval)
{
    _interval = new_interval;
}
void SpawnerComponent::setCooldown(float new_cd)
{
    _cooldown = new_cd;
}

float SpawnerComponent::getCD() const
{
    return _cooldown;
}

float SpawnerComponent::getInterval() const
{
    return _interval;
}

std::string SpawnerComponent::getType() const
{
    return EntityType;
}