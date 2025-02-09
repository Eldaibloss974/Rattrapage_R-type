/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** DamageComponent
*/

#pragma once

#include "AComponent.hpp"

class DamageComponent : public AComponent
{
public:
    DamageComponent(int dmg);
    ~DamageComponent() = default;

    static idComponent getStaticId() { return idComponent::Damage; }
    void setDamage(unsigned int damage);
    unsigned int getDamage() const;

protected:
private:
    unsigned int dmg;
};
