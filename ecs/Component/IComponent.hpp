/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** IComponent
*/

#pragma once

#include <cstddef>

enum idComponent
{
    Position = 0,
    Velocity = 1,
    Sprite = 2,
    Health = 3,
    Lifetime = 4,
    AI = 5,
    Weapon = 6,
    HitBox = 7,
    Damage = 8,
    Firerate = 9,
    Projectile = 10,
    Input = 11,
    Spawner = 12,
    Animation = 13,
    Parallax = 14,
    Score = 16,
};

class IComponent
{
    public:
        virtual ~IComponent() = default;
    
        virtual idComponent getId() const = 0;
    
    protected:
    private:
};
