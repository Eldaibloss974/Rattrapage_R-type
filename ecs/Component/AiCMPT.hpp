/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** AiComponent
*/

#pragma once

#include "AComponent.hpp"
#include <string>

class AiComponent : public AComponent
{
    public:
    AiComponent(std::string path, float interval, float cd, bool doLeft = false, bool doRight = false, bool doUp = false, bool doDown = false, bool doShoot = false);
    ~AiComponent() = default;

    static idComponent getStaticId() { return idComponent::AI; }

    void setAction(bool doLeft, bool doRight, bool doUp, bool doDown, bool doShoot);
    void setInterval(float new_interval);
    void setCooldown(float new_cd);

    bool getActionLeft() const;
    bool getActionRight() const;
    bool getActionUp() const;
    bool getActionDown() const;
    bool getActionShoot() const;
    float getCD() const;
    float getInterval() const;
    std::string getPath() const;


    protected:
    private:
        bool ActionLeft;
        bool ActionRight;
        bool ActionUp;
        bool ActionDown;
        bool ActionShoot;
        float _interval;
        float _cooldown = 0.0f;
        std::string scriptPath;
};
