/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** SpawnSystem.cpp
*/

#include "SpawnSystem.hpp"

componentMask SpawnSystem::getRequiredComponents()
{
    componentMask mask;

    mask.set(int(idComponent::Position));
    mask.set(int(idComponent::Spawner));
    return mask;
}

void SpawnSystem::spawnEntity(World &world, const SpawnerComponent &spawner, const PositionComponent &position)
{
    int level = spawner.getLevel();

    switch (level) {
        case 1:
            world.createEnemy(
                position.getX(),
                position.getY(),
                "../assets/sprites/Minions/EplosiveMinions.gif",
                "enemy_basic"
            );
            break;
        case 2:
            world.createEnemy(
                position.getX(),
                position.getY(),
                "../assets/sprites/Minions/EplosiveMinions.gif",
                "enemy_shooter"
            );
            break;
        case 3:
            world.createEnemy(
                position.getX(),
                position.getY(),
                "../assets/sprites/Minions/EplosiveMinions.gif",
                "enemy_fast_basic"
            );
            break;
        case 4:
            world.createEnemy(
                position.getX(),
                position.getY(),
                "../assets/sprites/Minions/EplosiveMinions.gif",
                "enemy_fast_shooter"
            );
            break;
        case 5:
            world.createEnemy(
                position.getX(),
                position.getY(),
                "../assets/sprites/Minions/EplosiveMinions.gif",
                "enemy_fast_shooter"
            );
            break;
        case 6:
            world.createEnemy(
                position.getX(),
                position.getY(),
                "../assets/sprites/Minions/EplosiveMinions.gif",
                "enemy_fast_shooter"
            );
            break;
        case 7:
            world.createEnemy(
                position.getX(),
                position.getY(),
                "../assets/sprites/Minions/EplosiveMinions.gif",
                "enemy_fast_shooter"
            );
            break;
        case 8:
            world.createEnemy(
                position.getX(),
                position.getY(),
                "../assets/sprites/Minions/EplosiveMinions.gif",
                "enemy_fast_shooter"
            );
            break;
        default:
            std::cerr << "[SpawnSystem] Unknown level: " << level << std::endl;
            break;
    }
    // std::cout << "[SpawnSystem] Spawned entity for level " << level << " at (" << position.getX() << ", " << position.getY() << ")" << std::endl;
}
