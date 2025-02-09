/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** ItBox
*/

#pragma once

#include "AComponent.hpp"

#include <tuple>

class HitBoxComponent : public AComponent
{
public:
    HitBoxComponent(float w, float h);
    ~HitBoxComponent() = default;

    static idComponent getStaticId() { return idComponent::HitBox; }
    void setHitBoxComponent(float width, float height);
    float getWidth() const;
    float getHeight() const;

protected:
private:
    float width;
    float height;
};
