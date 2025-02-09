/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** WeaponSystem.cpp
*/

#include "WeaponSystem.hpp"

componentMask WeaponSystem::getRequiredComponents()
{
    componentMask mask;

    mask.set(int(idComponent::Position));
    mask.set(int(idComponent::Weapon));
    return mask;
}

void WeaponSystem::createProjectiles(World &world, const PositionComponent &position, const WeaponComponent &weapon, EntityID id)
{
    for (int i = 0; i < weapon.getprojectileCount(); ++i) {
        float offsetX = i * 15.0f - (weapon.getprojectileCount() - 1) * 7.5f;
        EntityID projectile = world.createEntity();

        if (world.hasComponent<AiComponent>(id)) {
            world.addComponent<PositionComponent>(projectile, {position.getX() + offsetX, position.getY()});
            world.addComponent<VelocityComponent>(projectile, {-200.0f, 0.0f});
            world.addComponent<ProjectileComponent>(projectile, {id, weapon.getDamage(), 3.0f});
            world.addComponent<HitBoxComponent>(projectile, {10, 10});
            world.addComponent<SpriteComponent>(projectile, {"../assets/sprites/projectiles1/projectile_niv1.png", 0.0f, 0.0f, 18.5f, 18.0f});
        } else {
            world.addComponent<PositionComponent>(projectile, {position.getX() + offsetX, position.getY()});
            world.addComponent<VelocityComponent>(projectile, {200.0f, 0.0f});
            world.addComponent<ProjectileComponent>(projectile, {id, weapon.getDamage(), 3.0f});
            world.addComponent<HitBoxComponent>(projectile, {10, 10});
            world.addComponent<SpriteComponent>(projectile, {"../assets/sprites/projectiles1/projectile_niv1.png", 0.0f, 0.0f, 18.5f, 18.0f});
        }

        World::NewEntity entity_ = {projectile, "PROJECTILE", "../assets/sprites/projectiles1/projectile_niv1.png"};
        world.newEntity_.push_back(entity_);
    }
    std::cout << "Created " << weapon.getprojectileCount() << " projectiles for entity " << id << std::endl;
}
