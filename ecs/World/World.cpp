/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** World.cpp
*/

#include "EntityFactory.hpp"
#include "InputSystem.hpp"
#include "WeaponSystem.hpp"
#include "MovementSystem.hpp"
#include "CollisionSystem.hpp"
#include "RendererSystem.hpp"
#include "SpawnSystem.hpp"
#include "World.hpp"

World::World()
{
    // this->addSystem(std::make_unique<InputSystem>());
    // this->addSystem(std::make_unique<WeaponSystem>());
    // this->addSystem(std::make_unique<MovementSystem>());
    // this->addSystem(std::make_unique<CollisionSystem>());
    // this->addSystem(std::make_unique<RendererSystem>());
    // this->addSystem(std::make_unique<NetworkSystem>());
    // this->addSystem(std::make_unique<SoundSystem>());
}

void World::initialize()
{
    // createBackGround("../assets/sprites/background/space.png");
    createParallaxLayer("../assets/sprites/background/bg-back.png", 0.0f, 0.0f, -10.0f, 0.0f, 272.0f, 160.0f);
    // createParallaxLayer("../assets/sprites/background/bg-back.png", 272.0f, 0.0f, -10.0f, 0.0f, 272.0f, 160.0f);
    createParallaxLayer("../assets/sprites/background/bg-stars.png", 0.0f, 0.0f, -50.0f, 0.0f, 272.0f, 160.0f);

    // createPlayer("../assets/sprites/Player.gif", WeaponType::SINGLE_SHOT);
    // createEnemy(500.0f, 300.0f, "../assets/sprites/Minions/EplosiveMinions.gif");
    // createEnemy(500.0f, 200.0f, "../assets/sprites/Minions/EplosiveMinions.gif");
}

void World::cleanup()
{
    entities.clear();
    entityMasks.clear();
    components.clear();
    destroyedEntities.clear();

    systems.clear();

    ID_ticket = 0;
    nextEntityID = 0;
}

void World::addSystem(std::unique_ptr<ISystem> system)
{
    systems.push_back(std::move(system));
}

void World::updateSystems(float deltaTime)
{
    // std::cout << "Number of EntityID in World: " << entities.size() << std::endl;

    auto count = 0;

    for (auto &system : systems) {
        // std::cout << "Running System #" << count << std::endl;
        system->update(*this, deltaTime);
        count++;
    }
}

EntityID World::createEntity()
{
    std::lock_guard<std::mutex> lock(entitiesMutex);
    EntityID id = nextEntityID++;
    entities.push_back(id);
    entityMasks[id] = componentMask();
    return id;
}

EntityID World::createEntityCli(EntityID &id)
{
    std::lock_guard<std::mutex> lock(entitiesMutex);
    entities.push_back(id);
    entityMasks[id] = componentMask();
    return id;
}

Entity World::createPlayer(std::string spritePath, WeaponType weapon, std::size_t &id)
{
    Entity entity = EntityFactory::createPlayer(*this, spritePath, weapon);

    id = entity.getId();
    return entity;
}

Entity World::createPlayerCli(std::string sPath, WeaponType weapon, std::size_t &idEcs)
{
    Entity entity = EntityFactory::createPlayerCli(*this, sPath, weapon, idEcs);

    return entity;
}

Entity World::createOnHitEffect(float x, float y, const std::string &spritePath, int nbFrames, float frameTime, float lifetime)
{
    Entity entity = EntityFactory::createOnHitEffect(*this, x, y, spritePath, nbFrames, frameTime, lifetime);

    return entity;
}

Entity World::createOnHitEffectCli(float x, float y, const std::string &spritePath, int nbFrames, float frameTime, float lifetime, int frameLock, std::size_t &idEcs)
{
    Entity entity = EntityFactory::createOnHitEffectCli(*this, x, y, spritePath, nbFrames, frameTime, lifetime, frameLock, idEcs);

    return entity;
}

Entity World::createEnemy(float x, float y, std::string spritePath, std::string script)
{
    return EntityFactory::createEnemy(*this, x, y, script, spritePath);
}

Entity World::createEnemyCli(float x, float y, std::string spritePath, std::size_t &idEcs, int nb_frame, float t_frame, int frameLock, std::string script)
{
    Entity entity = EntityFactory::createEnemyCli(*this, x, y, script, spritePath, nb_frame, t_frame, frameLock, idEcs);

    return entity;
}

Entity World::createProjectile(float x, float y, int damage, float lifetime, EntityID id, std::string spritePath)
{
    return EntityFactory::createProjectile(*this, x, y, damage, lifetime, id, spritePath);
}

Entity World::createProjectileCli(float x, float y, int damage, float lifetime, EntityID id, std::string sPath, int nb_frame, float t_frame, int frameLock, EntityID idEcs)
{
    Entity entity = EntityFactory::createProjectileCli(*this, x, y, damage, lifetime, idEcs, nb_frame, t_frame, frameLock, sPath);

    printAllEntitiesDetails();
    return entity;
}

Entity World::createBackGround(std::string spritePath)
{
    return EntityFactory::createBackGround(*this, spritePath);
}

Entity World::createBackGroundCli(const std::string &sPath, std::size_t &idEcs)
{
    Entity entity = EntityFactory::createBackGroundCli(*this, sPath, idEcs);
    return entity;
}

Entity World::createParallaxLayer(std::string spritePath, float x, float y, float scrollSpeedX, float scrollSpeedY, float width, float height)
{
    return EntityFactory::createParallaxLayer(*this, x, y, spritePath, scrollSpeedX, scrollSpeedY,  width, height);
}

Entity World::createParallaxLayerCli(std::string spritePath, float x, float y, float scrollSpeedX, float scrollSpeedY, float width, float height, std::size_t &idEcs)
{
    Entity entity = EntityFactory::createParallaxLayerCli(*this, x, y, spritePath, scrollSpeedX, scrollSpeedY, width, height, idEcs);

    return entity;
}


Entity World::createSpawner(std::string entityType, float x, float y, float interval, float cooldown, int level)
{
    return EntityFactory::createSpawner(*this, x, y, entityType, interval, cooldown, level);
}

void World::cleanupDestroyedEntities()
{
    for (auto entity : destroyedEntities) {
        removeEntity(entity);
    }

    destroyedEntities.clear();
}


std::vector<EntityID> World::getEntitiesMatchingMask(const componentMask &mask) const
{
    // std::cout << "[World::getEntitiesMatchingMask] Start matching entities with mask: " << mask << std::endl;

    std::lock_guard<std::mutex> lock(entitiesMutex);
    std::vector<EntityID> matchingEntities;
    for (const auto &[entity, entityMask] : entityMasks)
    {
        // std::cout << "[World::getEntitiesMatchingMask] Checking entity ID: " << entity
        //           << ", Entity Mask: " << entityMask << ", Target Mask: " << mask << std::endl;

        if ((entityMask & mask) == mask)
        {
            // std::cout << "[World::getEntitiesMatchingMask] Entity ID " << entity
            //           << " matches the mask." << std::endl;
            matchingEntities.push_back(entity);
        }
        else
        {
            // std::cout << "[World::getEntitiesMatchingMask] Entity ID " << entity
            //           << " does not match the mask." << std::endl;
        }
    }

    // std::cout << "[World::getEntitiesMatchingMask] Total matching entities: " << matchingEntities.size() << std::endl;
    return matchingEntities;
}

const std::vector<EntityID>& World::getEntities() const
{
    return entities;
}

void World::removeEntity(EntityID entity)
{
    std::lock_guard<std::mutex> lock(entitiesMutex);
    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());

    entityMasks.erase(entity);

    for (auto &[typeID, componentMap] : components) {
        componentMap.erase(entity);
    }
    destroyedEntities.insert(entity);
    std::cout << "Entity " << entity << " has been removed." << std::endl;
}

bool World::doesEntityExist(EntityID entity) const
{
    return std::find(entities.begin(), entities.end(), entity) != entities.end();
}

void World::printAllEntitiesDetails() const
{
    std::lock_guard<std::mutex> lock(entitiesMutex);
    std::cout << "=== Entity Details ===" << std::endl;
    for (const auto &entity : entities) {
        std::cout << "Entity ID: " << entity;

        auto spriteIt = components.find(SpriteComponent::getStaticId());
        if (spriteIt != components.end()) {
            const auto &spriteMap = spriteIt->second;
            auto entityIt = spriteMap.find(entity);
            if (entityIt != spriteMap.end()) {
                const auto &sprite = *std::static_pointer_cast<SpriteComponent>(entityIt->second);
                std::cout << ", SpritePath: " << sprite.getSpritePath();
            } else {
                std::cout << ", SpritePath: None";
            }
        } else {
            std::cout << ", SpritePath: None";
        }

        std::cout << std::endl;
    }
    std::cout << "=======================\n" << std::endl;
}
