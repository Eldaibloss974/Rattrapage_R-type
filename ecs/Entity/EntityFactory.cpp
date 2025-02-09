/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** EntityFactory.cpp
*/

#include "EntityFactory.hpp"
#include "ProjectileComponent.hpp"

Entity EntityFactory::createPlayer(World& world, std::string spritePath, WeaponType weapon) {
    Entity player(world.createEntity());

    std::cout << "player has Id " << player.getId() << std::endl;
    world.addComponent<PositionComponent>(player.getId(), {100.0f, 200.0f});
    world.addComponent<VelocityComponent>(player.getId(), {0.0f, 0.0f});
    world.addComponent<HealthComponent>(player.getId(), {100});
    world.addComponent<InputComponent>(player.getId(), {});
    world.addComponent<WeaponComponent>(player.getId(), {false, 0.0f, 0.0f, 10, 1, weapon});
    world.addComponent<HitBoxComponent>(player.getId(), {50.0f, 50.0f});
    world.addComponent<SpriteComponent>(player.getId(), {spritePath, 0.0f, 0.0f, 33.2f, 19.0f});
    world.addComponent<ScoreComponent>(player.getId(), {0});

    World::NewEntity entity_ = {player.getId(), "PLAYER", spritePath};
    world.newEntity_.push_back(entity_);
    return player;
}

Entity EntityFactory::createPlayerCli(World& world, std::string spritePath, WeaponType weapon, std::size_t &id) {
    Entity player(world.createEntityCli(id));

    std::cout << "player has Id " << player.getId() << std::endl;
    world.addComponent<PositionComponent>(player.getId(), {100.0f, 200.0f});
    world.addComponent<VelocityComponent>(player.getId(), {0.0f, 0.0f});
    world.addComponent<HealthComponent>(player.getId(), {100});
    world.addComponent<InputComponent>(player.getId(), {});
    world.addComponent<WeaponComponent>(player.getId(), {false, 0.0f, 0.0f, 10, 1, weapon});
    world.addComponent<HitBoxComponent>(player.getId(), {50.0f, 50.0f});
    world.addComponent<SpriteComponent>(player.getId(), {spritePath, 0.0f, 0.0f, 33.2f, 19.0f});
    world.addComponent<AnimationComponent>(player.getId(), {1, 0.2f, 0});
    world.addComponent<ScoreComponent>(player.getId(), {0});

    return player;
}

Entity EntityFactory::createOnHitEffect(World &world, float x, float y, const std::string &spritePath, int nbFrames, float frameTime, float lifetime)
{
    Entity effect(world.createEntity());

    world.addComponent<PositionComponent>(effect.getId(), {x, y});
    world.addComponent<SpriteComponent>(effect.getId(), {spritePath, 0.0f, 0.0f, 64.0f, 64.0f});
    // world.addComponent<AnimationComponent>(effect.getId(), {nbFrames, frameTime, frameLock});
    world.addComponent<LifetimeComponent>(effect.getId(), {lifetime});

    World::NewEntity entity_ = {effect.getId(), "EFFECT", spritePath};
    world.newEntity_.push_back(entity_);

    return effect;
}

Entity EntityFactory::createOnHitEffectCli(World &world, float x, float y, const std::string &spritePath, int nbFrames, float frameTime, float lifetime, int frameLock, std::size_t &id)
{
    Entity effect(world.createEntityCli(id));

    world.addComponent<PositionComponent>(effect.getId(), {x, y});
    world.addComponent<SpriteComponent>(effect.getId(), {spritePath, 0.0f, 0.0f, 14.375f, 14.0f});
    world.addComponent<AnimationComponent>(effect.getId(), {nbFrames, frameTime, frameLock});
    world.addComponent<LifetimeComponent>(effect.getId(), {lifetime});

    return effect;
}


Entity EntityFactory::createEnemy(World& world, float x, float y, std::string script, std::string path) {
    Entity enemy(world.createEntity());

    std::cout << "enemy position " << x << " " << y << std::endl;
    world.addComponent<PositionComponent>(enemy.getId(), {x, y});
    world.addComponent<VelocityComponent>(enemy.getId(), {-50.0f, 0.0f});
    world.addComponent<HealthComponent>(enemy.getId(), {50});
    if (script == "enemy_basic" || script == "enemy_fast_basic") {
        world.addComponent<WeaponComponent>(enemy.getId(), {false, 0.0f, 1.0f, 10, 0});
    } else {
        world.addComponent<WeaponComponent>(enemy.getId(), {true, 0.0f, 1.0f, 10, 1});
    }
    world.addComponent<HitBoxComponent>(enemy.getId(), {20.0f, 20.0f});
    world.addComponent<SpriteComponent>(enemy.getId(), {path, 0.0f, 67.9f, 33.25f, 33.3f});
    world.addComponent<AiComponent>(enemy.getId(), {script, 0.0f, 5.0f});

    World::NewEntity entity_ = {enemy.getId(), "ENEMY", path};
    world.newEntity_.push_back(entity_);
    return enemy;
}

Entity EntityFactory::createEnemyCli(World& world, float x, float y, std::string script, std::string path, int nb_frame, float t_frame, int frameLock, EntityID &id) {
    Entity enemy(world.createEntityCli(id));

    std::cout << "enemy position " << x << " " << y << std::endl;
    world.addComponent<PositionComponent>(enemy.getId(), {x, y});
    world.addComponent<VelocityComponent>(enemy.getId(), {-50.0f, 0.0f});
    world.addComponent<HealthComponent>(enemy.getId(), {50});
    world.addComponent<WeaponComponent>(enemy.getId(), {true, 0.0f, 1.0f, 10, 0});
    world.addComponent<HitBoxComponent>(enemy.getId(), {50.0f, 50.0f});
    world.addComponent<SpriteComponent>(enemy.getId(), {path, 0.0f, 66.66f, 33.25f, 33.33f});
    world.addComponent<AiComponent>(enemy.getId(), {script, 0.0f, 5.0f});
    world.addComponent<AnimationComponent>(enemy.getId(), {nb_frame, t_frame, frameLock});

    return enemy;
}

Entity EntityFactory::createProjectile(World& world, float x, float y, int damage, float lifetime, EntityID id,  std::string spritePath) {
    Entity projectile(world.createEntity());

    std::cout << "Projectile position is  " << x << " " << y << "_________________________________________________________________________________________" << std::endl;
    world.addComponent<PositionComponent>(projectile.getId(), {x, y});
    world.addComponent<VelocityComponent>(projectile.getId(), {10.0f, 0.0f});
    world.addComponent<ProjectileComponent>(projectile.getId(), ProjectileComponent(id, damage, lifetime));
    world.addComponent<SpriteComponent>(projectile.getId(), {spritePath, 150.0f, 30.0f, 30.0f, 30.0f});
    return projectile;
}

Entity EntityFactory::createProjectileCli(World& world, float x, float y, int damage, float lifetime, EntityID id,int nb_frame, float t_frame, int frameLock, std::string spritePath) {
    Entity projectile(world.createEntityCli(id));

    std::cout << "Projectile position is  " << x << " " << y << "_________________________________________________________________________________________" << std::endl;
    world.addComponent<PositionComponent>(projectile.getId(), {x, y});
    world.addComponent<VelocityComponent>(projectile.getId(), {10.0f, 0.0f});
    world.addComponent<ProjectileComponent>(projectile.getId(), ProjectileComponent(id, damage, lifetime));
    world.addComponent<SpriteComponent>(projectile.getId(), {spritePath, 0.0f, 0.0f, 19.0f, 18.0f});
    world.addComponent<AnimationComponent>(projectile.getId(), {nb_frame, t_frame, frameLock});

    return projectile;
}

Entity EntityFactory::createBackGround(World& world, const std::string& spritePath)
{
    Entity background(world.createEntity());
    world.addComponent<PositionComponent>(background.getId(), {0.0f, 0.0f});
    world.addComponent<SpriteComponent>(background.getId(), {spritePath, 0.0f, 0.0f, 1226.0f, 207.0f});
    return background;
}

Entity EntityFactory::createBackGroundCli(World& world, const std::string& spritePath, EntityID &id)
{
    Entity background(world.createEntityCli(id));

    world.addComponent<PositionComponent>(background.getId(), {0.0f, 0.0f});
    world.addComponent<SpriteComponent>(background.getId(), {spritePath, 0.0f, 0.0f, 1226.0f, 207.0f});
    return background;
}

Entity EntityFactory::createSpawner(World &world, float x, float y, const std::string &entityType, float interval, float cooldown, int level)
{
    Entity spawner(world.createEntity());

    world.addComponent<PositionComponent>(spawner.getId(), {x, y});

    world.addComponent<SpawnerComponent>(spawner.getId(), {entityType, interval, cooldown, level});

    return spawner;
}

Entity EntityFactory::createParallaxLayer(World& world, float x, float y, std::string spritePath, float scrollSpeedX, float scrollSpeedY, float width, float height)
{
    Entity layer(world.createEntity());

    world.addComponent<PositionComponent>(layer.getId(), {x, y});
    world.addComponent<SpriteComponent>(layer.getId(), {spritePath, 0.0f, 0.0f, width, height});
    world.addComponent<ParallaxComponent>(layer.getId(), {scrollSpeedX, scrollSpeedY});

    return layer;
}

Entity EntityFactory::createParallaxLayerCli(World& world, float x, float y, std::string spritePath, float scrollSpeedX, float scrollSpeedY, float width, float height, std::size_t &id)
{
    Entity layer(world.createEntityCli(id));

    world.addComponent<PositionComponent>(layer.getId(), {x, y});
    world.addComponent<SpriteComponent>(layer.getId(), {spritePath, 0.0f, 0.0f, width, height});
    world.addComponent<ParallaxComponent>(layer.getId(), {scrollSpeedX, scrollSpeedY});

    return layer;
}

