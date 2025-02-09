/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** FireRateComponent
*/

#pragma once

#include "AComponent.hpp"

class FireRateComponent : public AComponent
{
public:
    FireRateComponent(float firerate);
    ~FireRateComponent() = default;

    static idComponent getStaticId() { return idComponent::Firerate; }
    void setFireRate(float FireRate);
    float getFireRate() const;

protected:
private:
    float rps;
};
