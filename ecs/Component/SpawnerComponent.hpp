/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** SpawnerComponent.hpp
*/

#pragma once

#include "AComponent.hpp"
#include <string>

class SpawnerComponent : public AComponent
{
    public:
        SpawnerComponent(std::string type, float interval, float cooldown, int level);
        ~SpawnerComponent() = default;

        static idComponent getStaticId() { return idComponent::Spawner; }

        void setInterval(float new_interval);
        void setCooldown(float new_cd);

        float getCD() const;
        float getInterval() const;
        std::string getType() const;

        int getLevel() const {
            return level_;
        }

        void setLevel(int &level) {
            level_ = level;
        }

    private:
        std::string EntityType;
        float _interval;
        float _cooldown = 0.0f;
        int level_ = 1;
};
