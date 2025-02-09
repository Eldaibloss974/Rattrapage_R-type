/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** ProjectileComponent.hpp
*/

#pragma once

#include "AComponent.hpp"
#include "World.hpp"
#include <stdexcept>
#include "EntityID.hpp"

class ProjectileComponent : public AComponent {
    public:
        ProjectileComponent(EntityID owner, int damage, float lifetime)
        : AComponent(idComponent::Projectile), owner(owner), damage(damage), lifetime(lifetime) {}

        EntityID getOwner() const {
            return owner;
        }

        static idComponent getStaticId() { return idComponent::Projectile; }

        int getDamage() const {
            return damage;
        }

        void setDamage(int newDamage) {
            if (newDamage < 0) {
                throw std::invalid_argument("Damage cannot be negative");
            }
            damage = newDamage;
        }

        float getLifetime() const {
            return lifetime;
        }

        void setLifetime(float newLifetime) {
            if (newLifetime < 0.0f) {
                throw std::invalid_argument("Lifetime cannot be negative");
            }
            lifetime = newLifetime;
        }
    private:
        EntityID owner;
        int damage;
        float lifetime;
};

