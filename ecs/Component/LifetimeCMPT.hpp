/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** LifetimeComponent
*/

#pragma once

#include "AComponent.hpp"

class LifetimeComponent : public AComponent
{
public:
    LifetimeComponent(float lifetime);
    ~LifetimeComponent() = default;

    static idComponent getStaticId() { return idComponent::Lifetime; }
    float getLifeTime() const;
    void setLifeTime(float lifeTime);

protected:
private:
    float LifeTime;
};
