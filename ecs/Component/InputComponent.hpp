/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** InputComponent.hpp
*/

#pragma once

#include "AComponent.hpp"
#include <string>

class InputComponent : AComponent {
    public:
        InputComponent(bool doLeft = false, bool doRight = false, bool doUp = false, bool doDown = false, bool doShoot = false) : AComponent(idComponent::Input), ActionLeft(doLeft), ActionRight(doRight), ActionUp(doUp), ActionDown(doDown), ActionShoot(doShoot) {}
        ~InputComponent() = default;

        static idComponent getStaticId() { return idComponent::Input; }
        void setAction(bool doLeft, bool doRight, bool doUp, bool doDown, bool doShoot)
        {
            ActionLeft = doLeft;
            ActionRight = doRight;
            ActionDown = doDown;
            ActionUp = doUp;
            ActionShoot = doShoot;
        }
        bool getActionLeft() const { return ActionLeft; }
        bool getActionRight() const { return ActionRight; }
        bool getActionUp() const { return ActionUp; }
        bool getActionDown() const { return ActionDown; }
        bool getActionShoot() const { return ActionShoot; }
        void setActionLeft(bool action) { ActionLeft = action; }
        void setActionRight(bool action) { ActionRight = action; }
        void setActionUp(bool action) { ActionUp = action; }
        void setActionDown(bool action) { ActionDown = action; }
        void setActionShoot(bool action) { ActionShoot = action; }

    protected:
    private:
        bool ActionLeft;
        bool ActionRight;
        bool ActionUp;
        bool ActionDown;
        bool ActionShoot;
};