/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** WeaponSystem.hpp
*/

#pragma once

#include "ISystem.hpp"
#include "World.hpp"

class WeaponSystem : public ISystem
{
    private:
        void createProjectiles(World &world, const PositionComponent &position, const WeaponComponent &weapon, EntityID id);
    public:
        static componentMask getRequiredComponents();
        bool shouldRun(const World &world) const override { return true; }
        void update(World &world, float deltaTime) override {
            const auto &entities = world.getEntitiesMatchingMask(getRequiredComponents());
        
            // Loguer le nombre d'entités récupérées
            // std::cout << "WeaponSystem - Number of matching entities: " << entities.size() << std::endl;
        
            for (auto entity : entities) {
                // Log de l'entité en cours de traitement
                // std::cout << "Processing EntityID: " << entity << std::endl;
        
                auto &position = world.getComponent<PositionComponent>(entity, "Weapon 1");
                auto &weapon = world.getComponent<WeaponComponent>(entity);
        
                // Log des informations du PositionComponent
                // std::cout << "  Position: (x=" << position.getX() << ", y=" << position.getY() << ")" << std::endl;
        
                if (weapon.getcooldown() > 0.0f) {
                    float new_cd = weapon.getcooldown() - deltaTime;
                    weapon.setcooldown(new_cd);
        
                    // Log du cooldown réduit
                    // std::cout << "  Cooldown reduced to: " << new_cd << std::endl;
                }
        
                if (weapon.getisShooting() && weapon.getcooldown() <= 0.0f) {
                    // std::cout << "  Creating projectile for EntityID: " << entity << std::endl;
        
                    createProjectiles(world, position, weapon, entity);
        
                    weapon.setcooldown(weapon.getFireRate());
                    // std::cout << "  Cooldown reset to: " << weapon.getFireRate() << std::endl;
                }

                if (world.hasComponent<InputComponent>(entity)) {
                    weapon.setisShooting(false);
                }
                // std::cout << "  isShooting reset to false for EntityID: " << entity << std::endl;
            }
        }

};