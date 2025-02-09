/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** FireRateComponent
*/

#include "FireRateCMPT.hpp"

FireRateComponent::FireRateComponent(float firerate) : AComponent(idComponent::Firerate), rps(firerate)
{
}

void FireRateComponent::setFireRate(float FireRate)
{
    rps = FireRate;
}

float FireRateComponent::getFireRate() const
{
    return rps;
}
