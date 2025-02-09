cmake_minimum_required(VERSION 3.16)
project(Rtype)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERVER_SRC
    src/main_server.cpp
    src/Server/GameServer.cpp
    src/Network/Server.cpp
    src/Server/Data.cpp
    src/Server/ServerPacketHandler.cpp

    ecs/World/World.cpp
    ecs/Systems/InputSystem/InputSystem.cpp
    ecs/Systems/RendererSystem/RendererSystem.cpp
    ecs/Systems/CollisionSystem/CollisionSystem.cpp
    ecs/Systems/MovementSystem/MovementSystem.cpp
    ecs/Systems/WeaponSystem/WeaponSystem.cpp
    ecs/Systems/SpawnSystem/SpawnSystem.cpp
    ecs/Systems/AnimationSystem/AnimationSystem.cpp
    ecs/Entity/Entity.cpp
    ecs/Entity/EntityFactory.cpp

    ecs/Component/AComponent.cpp
    ecs/Component/AiCMPT.cpp
    ecs/Component/DamageCMPT.cpp
    ecs/Component/FireRateCMPT.cpp
    ecs/Component/HealthCMPT.cpp
    ecs/Component/ItBox.cpp
    ecs/Component/LifetimeCMPT.cpp
    ecs/Component/PositionCMPT.cpp
    ecs/Component/SpriteCMPT.cpp
    ecs/Component/VelocityCMPT.cpp
    ecs/Component/SpawnerComponent.cpp
    
)

set(CLIENT_SRC
    src/main_client.cpp
    src/Graphics/Game.cpp
    src/Graphics/Menu/Menu.cpp
    src/Network/Client.cpp
    src/Graphics/InitialisedWorld.cpp
    src/Graphics/Menu/MenuPacketHandler.cpp

    ecs/World/World.cpp
    ecs/Systems/InputSystem/InputSystem.cpp
    ecs/Systems/RendererSystem/RendererSystem.cpp
    ecs/Systems/CollisionSystem/CollisionSystem.cpp
    ecs/Systems/MovementSystem/MovementSystem.cpp
    ecs/Systems/WeaponSystem/WeaponSystem.cpp
    ecs/Systems/SpawnSystem/SpawnSystem.cpp
    ecs/Systems/AnimationSystem/AnimationSystem.cpp
    ecs/Entity/Entity.cpp
    ecs/Entity/EntityFactory.cpp

    ecs/Component/AComponent.cpp
    ecs/Component/AiCMPT.cpp
    ecs/Component/DamageCMPT.cpp
    ecs/Component/FireRateCMPT.cpp
    ecs/Component/HealthCMPT.cpp
    ecs/Component/ItBox.cpp
    ecs/Component/LifetimeCMPT.cpp
    ecs/Component/PositionCMPT.cpp
    ecs/Component/SpriteCMPT.cpp
    ecs/Component/VelocityCMPT.cpp
    ecs/Component/SpawnerComponent.cpp
)

find_package(SFML 2.5 COMPONENTS network REQUIRED)
find_package(Boost 1.74 REQUIRED COMPONENTS system thread OPTIONAL_COMPONENTS container)

if(Boost_FOUND)
    message(STATUS "Boost version: ${Boost_VERSION}")
    message(STATUS "Boost include directory: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost not found or required components missing.")
endif()

find_package(raylib REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Graphics
    ${CMAKE_SOURCE_DIR}/src/Graphics/Menu
    ${CMAKE_SOURCE_DIR}/src/Graphics/SoundManager
    ${CMAKE_SOURCE_DIR}/src/Network
    ${CMAKE_SOURCE_DIR}/src/Server
    ${CMAKE_SOURCE_DIR}/ecs
    ${CMAKE_SOURCE_DIR}/ecs/Component
    ${CMAKE_SOURCE_DIR}/ecs/Entity
    ${CMAKE_SOURCE_DIR}/ecs/Systems
    ${CMAKE_SOURCE_DIR}/ecs/Systems/CollisionSystem
    ${CMAKE_SOURCE_DIR}/ecs/Systems/InputSystem
    ${CMAKE_SOURCE_DIR}/ecs/Systems/MovementSystem
    ${CMAKE_SOURCE_DIR}/ecs/Systems/RendererSystem
    ${CMAKE_SOURCE_DIR}/ecs/Systems/WeaponSystem
    ${CMAKE_SOURCE_DIR}/ecs/Systems/SpawnSystem
    ${CMAKE_SOURCE_DIR}/ecs/Systems/AnimationSystem
    ${CMAKE_SOURCE_DIR}/ecs/Systems/LifetimeSystem
    ${CMAKE_SOURCE_DIR}/ecs/Systems/ParallaxSystem
    ${CMAKE_SOURCE_DIR}/ecs/World
)

add_executable(r-type_server
    ${SERVER_SRC}
)
target_include_directories(r-type_server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(r-type_server PRIVATE ${Boost_LIBRARIES} sfml-network raylib)

add_executable(r-type_client
    ${CLIENT_SRC}
)
target_include_directories(r-type_client PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(r-type_client PRIVATE ${Boost_LIBRARIES} sfml-network raylib)