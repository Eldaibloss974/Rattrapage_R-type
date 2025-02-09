/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** WeaponComponent.hpp
*/

#pragma once

#include "AComponent.hpp"
#include <iostream>

enum class WeaponType {
    SINGLE_SHOT,
    MULTI_SHOT,
    CHARGED_SHOT,
};

class WeaponComponent : public AComponent
{
    public:
        WeaponComponent(
            bool isShooting, 
            float cd, 
            float fr, 
            int dmg, 
            int projectileCount, 
            WeaponType weaponType = WeaponType::SINGLE_SHOT
        ) 
        : AComponent(idComponent::Weapon),
          type(weaponType),
          _isShooting(isShooting), 
          cooldown(cd), 
          fireRate(fr),
          damage(dmg),
          _projectileCount(projectileCount)
        {}

        ~WeaponComponent() = default;

        static idComponent getStaticId() { return idComponent::Weapon; }

        void setFireRate(float FireRate) { fireRate = FireRate; }
        float getFireRate() const { return fireRate; }

        void setcooldown(float Cooldown) { cooldown = Cooldown; }
        float getcooldown() const { return cooldown; }

        void setisShooting(bool isShooting) { _isShooting = isShooting; }
        bool getisShooting() const { return _isShooting; }

        void setDamage(int Damage) { damage = Damage; }
        int getDamage() const { return damage; }

        void setprojectileCount(int ProjectileCount) { _projectileCount = ProjectileCount; }
        int getprojectileCount() const { return _projectileCount; }

        void setWeaponType(WeaponType WeaponType) { type = WeaponType; }
        WeaponType getWeaponType() const { return type; }
        
    private:
        WeaponType type;
        bool _isShooting;
        float cooldown;
        float fireRate;
        int damage;
        int _projectileCount;
};