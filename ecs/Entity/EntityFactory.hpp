/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** EntityFactory.hpp
*/

#pragma once

#include "Entity.hpp"
#include "World.hpp"
#include <string>
#include <tuple>

class EntityFactory {
    public:
        static Entity createPlayer(World& world, std::string spritePath, WeaponType weapon);
        static Entity createEnemy(World& world, float x, float y, std::string spritePath, std::string script = "None");
        static Entity createProjectile(World& world, float x, float y, int damage, float lifetime, EntityID id, std::string spritePath);
        static Entity createBackGround(World& world,  const std::string& spritePath);
        static Entity createParallaxLayer(World& world, float x, float y, std::string spritePath, float scrollSpeedX, float scrollSpeedY, float width, float height);
        static Entity createSpawner(World& world, float x, float y, const std::string& entityType, float interval, float cooldown, int level);
        static Entity createOnHitEffect(World &world, float x, float y, const std::string &spritePath, int nbFrames, float frameTime, float lifetime);

        static Entity createEnemyCli(World& world, float x, float y, std::string script, std::string path, int nb_frame, float t_frame, int frameLock, EntityID &id);
        static Entity createProjectileCli(World& world, float x, float y, int damage, float lifetime, EntityID id, int nb_frame, float t_frame, int frameLock, std::string spritePath);
        static Entity createPlayerCli(World& world, std::string spritePath, WeaponType weapon, std::size_t &id);
        static Entity createBackGroundCli(World& world, const std::string& spritePath, EntityID &id);
        static Entity createParallaxLayerCli(World& world, float x, float y,std::string spritePath, float scrollSpeedX, float scrollSpeedY, float width, float height, std::size_t &id);
        static Entity createOnHitEffectCli(World &world, float x, float y, const std::string &spritePath, int nbFrames, float frameTime, float lifetime, int frameLock, std::size_t &id);

    private:
};