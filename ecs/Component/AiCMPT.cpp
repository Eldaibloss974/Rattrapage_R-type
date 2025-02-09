/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** AiComponent
*/

#include "AiCMPT.hpp"

AiComponent::AiComponent(std::string path, float interval, float cd, bool doLeft, bool doRight, bool doUp, bool doDown, bool doShoot) : AComponent(idComponent::AI), scriptPath(path), _interval(interval), _cooldown(cd), ActionLeft(doLeft), ActionRight(doRight), ActionUp(doUp), ActionDown(doDown), ActionShoot(doShoot)
{
}

void AiComponent::setInterval(float new_interval)
{
    _interval = new_interval;
}

void AiComponent::setCooldown(float new_cd)
{
    _cooldown = new_cd;
}

void AiComponent::setAction(bool doLeft, bool doRight, bool doUp, bool doDown, bool doShoot)
{
    ActionLeft = doLeft;
    ActionRight = doRight;
    ActionUp = doUp;
    ActionDown = doDown;
    ActionShoot = doShoot;
}

std::string AiComponent::getPath() const
{
    return scriptPath;
}

bool AiComponent::getActionLeft() const
{
    return ActionLeft;
}

bool AiComponent::getActionRight() const
{
    return ActionRight;
}

bool AiComponent::getActionUp() const
{
    return ActionUp;
}

bool AiComponent::getActionDown() const
{
    return ActionDown;
}

bool AiComponent::getActionShoot() const
{
    return ActionShoot;
}

float AiComponent::getCD() const
{
    return _cooldown;
}

float AiComponent::getInterval() const
{
    return _interval;
}