# ECS (Entity Component System)

This page explains the ECS architecture implemented for this project. An ECS (Entity Component System) is an architecture that enables independence between component objects and is divided into three categories:

## Entity

Entities are objects containing only their own unique ID. In the case of R-Type, this could be a player, an enemy, projectiles from a player or an enemy.

```
pseudo class Entity {
    public:
        getID()
    private:
        id
}
```

## Component

Components are connected to these entities. They have their own properties. For example, a position component has x and y coordinates (2D Game). For R-TYPE's operation, several types of components will be necessary for each type of entity, and each component type will have a different ID to identify them in the systems.

```
enum class idComponent
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
    ....
};
```

```
pseudo class PositionComponent {
    public:
        getX()
        getY()
        setPosition(x, y)
    private:
        _x
        _y
}
```

## Systems

Systems are objects that will interact with and modify component types based on the game state. Take the case of the MovementSystem, which will handle modifying position components based on the velocity component assigned to its entity.

```
pseudo class MovementSystem {
    public:
        getRequiredComponent()
        update(world, deltaTime)
}
```

To update the correct components, each system will have its "ComponentMask" that will determine which type of component it will modify, and therefore which entities will be updated. To do this, each entity will have a "mask" that will be modified according to the components connected to it. A comparison between the entity's mask and the system's mask will be performed to identify if the entity has the components required by the system using the "bitset" comparison method.

## Bitset Method

Why use the bitset approach to check entity content for a system (bitset from the C++ standard library / std::bitset)?

- Very efficient method as it allows bit-level operations, which uses very few resources and is very fast.

- Necessary because many entities will be created and destroyed.

- Shots, enemies, and power-ups can represent dozens or hundreds of simultaneous entities. Bitsets allow quick filtering of relevant entities for each system without complete iteration.

How it works:

---
### Associate a unique index with each component type
A unique index is assigned for each component type.
These IDs correspond to bit indices in the bitset.

```
PositionComponentID = 0,
VelocityComponentID = 1,
SpriteComponentID = 2,
HealthComponentID = 3,
LifetimeComponentID = 4,
AIComponentID = 5,
DamageComponentID = 6,
```
---
### Assign a bitset to each entity
Each entity has a bitset that indicates which components it has. If the entity has a PositionComponent, the bit corresponding to PositionComponentID is activated (1).

```
MAX_COMPONENTS = 64
maskComponent = std::bitset<MAX_COMPONENTS>
pseudo-class Entity {
    public:
        addComponent(componentID) {
            maskComponent.set(componentID)
        }
        // other functions like "hasComponents"
    private:
        maskComponent
}
```
---
### Define component masks for systems
Each system defines a "maskComponent" that indicates the required components to process an entity.

---
### Compare entity bitsets to the mask
An entity is processed by a system if its bitset satisfies the system's "maskComponent" (AND logical operation).

```
pseudo class System {
    public:
        getRequiredComponents() {
            mask
            mask.set(Component1ID)
            mask.set(Component2ID)
            etc...
            return mask
        }
        
        update(world, deltaTime)
            For all entities in world
                if entity has required components
                    update its components
}
```

## World

World will be the class that links each ECS object together. It will store entity IDs, their components, systems, each entity's masks, and this class implements the creation methods for each entity (Player, Enemy, Projectiles) and functions to destroy entities.

```
pseudo class World {
    public:
        createPlayer()
        createEnemy()
        createProjectile()
        createEntityID()
        destroyEntity(entity)
    private:
        nextEntityID;
        entities;
        destroyedEntities;
        systems;
        entityMasks;
        components;
}
```

## ECS Server

The ECS is divided into 2 distinct parts: game logic and display. The server handles the first part - it will detect collisions between entities and manage their consequences, such as creating impact effects, destroying entities, and more. It will also manage entity movement, lifetime of certain entities, etc.

## ECS Client

The client-side ECS will handle rendering entity images on the screen and sending client actions (Inputs) to the server. It therefore handles "sprite" display and animation of entities with a "sprite".