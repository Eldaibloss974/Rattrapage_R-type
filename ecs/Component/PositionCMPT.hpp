/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** PositionComponent
*/

#pragma once

#include "AComponent.hpp"
#include <tuple>

class PositionComponent : public AComponent
{
public:
    PositionComponent(float x, float y);
    ~PositionComponent() = default;

    float getX() const;
    float getY() const;
    void setX(float x) {
        _x = x;
    }
    void setY(float x) {
        _y = x;
    }
    void setPosition(float x, float y);
    static idComponent getStaticId() { return idComponent::Position; }


protected:
private:
    float _x;
    float _y;
};
