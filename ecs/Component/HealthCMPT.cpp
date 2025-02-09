/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** HealthComponent
*/

#include "HealthCMPT.hpp"

HealthComponent::HealthComponent(int health) : AComponent(idComponent::Health), Health(health)
{
}

void HealthComponent::setHealth(unsigned int health)
{
    Health = health;
}

void HealthComponent::modifyHealth(int health)
{
    Health += health;
}

unsigned int HealthComponent::getHealth() const
{
    return Health;
}