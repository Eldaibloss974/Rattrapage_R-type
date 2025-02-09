/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** World.hpp
*/

#ifndef WORLD_HPP
#define WORLD_HPP

#include "Entity.hpp"
#include <algorithm>
#include <vector>
#include <thread>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include "WeaponComponent.hpp"
#include <algorithm>
#include "HealthCMPT.hpp"
#include "DamageCMPT.hpp"
#include "FireRateCMPT.hpp"
#include "SpriteCMPT.hpp"
#include "PositionCMPT.hpp"
#include "VelocityCMPT.hpp"
#include "ItBox.hpp"
#include "AiCMPT.hpp"
#include "InputComponent.hpp"
#include "ISystem.hpp"
#include "EntityID.hpp"
#include "ProjectileComponent.hpp"
#include "LifetimeCMPT.hpp"
#include "SpawnerComponent.hpp"
#include "AnimationComponent.hpp"
#include "ParallaxComponent.hpp"
#include "ScoreCMPT.hpp"
#include <iostream>
#include <typeinfo>
#include "Client.hpp"

class EntityFactory;

constexpr size_t MAX_COMPONENTS = 64;
using componentMask = std::bitset<MAX_COMPONENTS>;

class World {
    public:
        World();

        World(World&& other) noexcept
            : ID_ticket(other.ID_ticket),
            nextEntityID(other.nextEntityID),
            entities(std::move(other.entities)),
            destroyedEntities(std::move(other.destroyedEntities)),
            systems(std::move(other.systems)),
            entityMasks(std::move(other.entityMasks)),
            components(std::move(other.components)) {
            other.ID_ticket = 0;
            other.nextEntityID = 0;
        }

        World& operator=(World&& other) noexcept {
            if (this != &other) {
                cleanup();

                ID_ticket = other.ID_ticket;
                nextEntityID = other.nextEntityID;
                entities = std::move(other.entities);
                destroyedEntities = std::move(other.destroyedEntities);
                systems = std::move(other.systems);
                entityMasks = std::move(other.entityMasks);
                components = std::move(other.components);

                other.ID_ticket = 0;
                other.nextEntityID = 0;
            }
            return *this;
        }

        void initialize();
        void cleanup();

        EntityID createEntity();
        EntityID createEntityCli(EntityID &id);

        Entity createPlayer(std::string sPath, WeaponType weapon, std::size_t &idEcs);
        Entity createPlayerCli(std::string sPath, WeaponType weapon, std::size_t &idEcs);

        Entity createEnemy(float x, float y, std::string sPath, std::string script = "basic_enemy");
        Entity createEnemyCli(float x, float y, std::string sPath, std::size_t &idEcs, int nb_frame, float t_frame, int frameLock, std::string script = "basic_enemy");

        Entity createProjectile(float x, float y, int damage, float lifetime, EntityID id, std::string sPath);
        Entity createProjectileCli(float x, float y, int damage, float lifetime, EntityID id, std::string sPath, int nb_frame, float t_frame, int frameLock, EntityID idEcs);

        Entity createBackGround(std::string spritePath);
        Entity createBackGroundCli(const std::string &sPath, std::size_t &idEcs);

        Entity createOnHitEffect(float x, float y, const std::string &spritePath, int nbFrames, float frameTime, float lifetime);
        Entity createOnHitEffectCli(float x, float y, const std::string &spritePath, int nbFrames, float frameTime, float lifetime, int frameLock, std::size_t &idEcs);

        Entity createParallaxLayer(std::string spritePath, float x, float y, float scrollSpeedX, float scrollSpeedY, float width, float height);
        Entity createParallaxLayerCli(std::string spritePath, float x, float y,float scrollSpeedX, float scrollSpeedY, float width, float height, std::size_t &id);

        Entity createSpawner(std::string entityPath, float x, float y, float interval, float cooldown, int level);

        void destroyEntity(EntityID entity) {
            destroyedEntities.insert(entity);
        }

        bool isEntityDestroyed(EntityID entity) const {
            return destroyedEntities.find(entity) != destroyedEntities.end();
        }

        void cleanupDestroyedEntities();

        void addSystem(std::unique_ptr<ISystem> system);
        void updateSystems(float deltaTime);

        template<typename T>
        void addComponent(EntityID id, T component)
        {


            components[T::getStaticId()][id] = std::make_shared<T>(component);
            entityMasks[id].set(T::getStaticId());
        }


        template <typename T>
        T& getComponent(EntityID entity, std::string str = "OTHER")
        {
            auto& entityComponents = components[T::getStaticId()];
            auto it = entityComponents.find(entity);
            
            if (it == entityComponents.end() || !it->second) {

                std::cerr << "[getComponent] Component not found for Entity ID: " << entity 
                        << ". Creating a new component." << std::endl;
                throw std::runtime_error("[getComponent] from [" + str + "] Unsupported component type for Entity ID: " + std::to_string(entity));
            }
            return *std::static_pointer_cast<T>(it->second);
        }


        template <typename T>
        bool hasComponent(EntityID entity) const {
            return entityMasks.at(entity).test(T::getStaticId());
        }

        std::vector<EntityID> getEntitiesMatchingMask(const componentMask& mask) const;
        const std::vector<EntityID>& getEntities() const;

        void removeEntity(EntityID entity);

        struct NewEntity {
            std::size_t id;
            std::string type;
            std::string path;
        };

        std::vector<NewEntity> newEntity_;

        std::vector<EntityID> deletedEntity_;

        std::vector<EntityID> newScoreId_;

        bool doesEntityExist(EntityID entity) const;

        void printAllEntitiesDetails() const;


        bool isEntityValid(EntityID entityId) const {
            return std::find(entities.begin(), entities.end(), entityId) != entities.end();
        }



        mutable std::mutex entitiesMutex;
        std::vector<EntityID> entities;

    private:
        size_t ID_ticket;
        EntityID nextEntityID = 0;
        std::unordered_set<EntityID> destroyedEntities;
        std::vector<std::unique_ptr<ISystem>> systems;
        std::unordered_map<EntityID, componentMask> entityMasks;
        std::unordered_map<size_t, std::unordered_map<EntityID, std::shared_ptr<void>>> components;
};
#endif