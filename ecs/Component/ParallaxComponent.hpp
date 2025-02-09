/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** ParallaxComponent.hpp
*/

#pragma once

#include "AComponent.hpp"

class ParallaxComponent : public AComponent {
    public:
        ParallaxComponent(float scrollSpeedX, float scrollSpeedY)
            : AComponent(idComponent::Parallax), _scrollSpeedX(scrollSpeedX), _scrollSpeedY(scrollSpeedY) {}

        static idComponent getStaticId() { return idComponent::Parallax; }

        float getScrollSpeedX() const { return _scrollSpeedX; }
        float getScrollSpeedY() const { return _scrollSpeedY; }

        void setScrollSpeedX(float speed) { _scrollSpeedX = speed; }
        void setScrollSpeedY(float speed) { _scrollSpeedY = speed; }

    private:
        float _scrollSpeedX;
        float _scrollSpeedY;
};
