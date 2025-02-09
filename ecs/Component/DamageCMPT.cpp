/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** DamageComponent
*/

#include "DamageCMPT.hpp"

DamageComponent::DamageComponent(int dmg) : AComponent(idComponent::Damage), dmg(dmg) 
{
}

void DamageComponent::setDamage(unsigned int damage)
{
    dmg = damage;
}

unsigned int DamageComponent::getDamage() const
{
    return dmg;
}
