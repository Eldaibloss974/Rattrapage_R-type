/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** HealthComponent
*/

#pragma once

#include "AComponent.hpp"

class HealthComponent : public AComponent
{
public:
    HealthComponent(int Health);
    ~HealthComponent() = default;

    static idComponent getStaticId() { return idComponent::Health; }
    void setHealth(unsigned int health);
    void modifyHealth(int health);
    unsigned int getHealth() const;

protected:
private:
    unsigned int Health;
};
