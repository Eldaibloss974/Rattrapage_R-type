/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** VelocityComponent
*/

#pragma once

#include "AComponent.hpp"

#include "tuple"

class VelocityComponent : public AComponent
{
    public:
        VelocityComponent(float x, float y);
        ~VelocityComponent() = default;

        static idComponent getStaticId() { return idComponent::Velocity; }
        float getX() const;
        float getY() const;
        void setX(float dx);
        void setY(float dy);
        void setVelocity(float x, float y);

    protected:
    private:
        float _x;
        float _y;
};
