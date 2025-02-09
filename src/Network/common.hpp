/*
** EPITECH PROJECT, 2024
** Game
** File description:
** common.hpp
*/

#pragma once

#include <cstdint>
#include <cstring>
#include <SFML/Network.hpp>

struct PacketHeader {
    uint16_t seq;
    uint16_t ack;
    uint8_t  flags;
    uint8_t  reserved;
};

static constexpr size_t HEADER_SIZE = sizeof(PacketHeader);

// CLIENT -> SERVER
static constexpr sf::Uint8 CONNECT_PLAYER = 0; // -> CONNECT_PLAYER >> playerId
static constexpr sf::Uint8 DISCONNECT_PLAYER = 1; // -> DISCONNECT_PLAYER >> playerId
static constexpr sf::Uint8 CREATE_ROOM = 2; // -> CREATE_ROOM >> playerId >> roomName >> level
static constexpr sf::Uint8 SEARCH_ROOM = 5; // -> SEARCH_ROOM 
static constexpr sf::Uint8 JOIN_ROOM = 6; // -> JOIN_ROOM >> clientId >> roomName >> spritePath
static constexpr sf::Uint8 REQUEST_ROOMS = 8; // -> REQUEST_ROOM


static constexpr sf::Uint8 SEND_INPUT = 9; // -> SEND_INPUT >> playerId >> key

static constexpr sf::Uint8 QUIT_GAME = 10; // -> QUIT_GAME >> playerId



// SERVER -> CLIENT
static constexpr sf::Uint8 ACCEPT_CONNECT_PLAYER = 0; // -> ACCEPT_CONNECT_PLAYER >> clientId_
static constexpr sf::Uint8 NOT_ACCEPT_CONNECT_PLAYER = 1; // -> NOT_ACCEPT_CONNECT_PLAYER
static constexpr sf::Uint8 ACCEPT_DISCONNECT_PLAYER = 2; // -> ACCEPT_DISCONNECT_PLAYER
static constexpr sf::Uint8 ACCEPT_CREATE_ROOM = 3; // -> ACCEPT_CREATE_ROOM >> roomName >> level
static constexpr sf::Uint8 NOT_ACCEPT_CREATE_ROOM = 4; // -> NOT_ACCEPT_CREATE_ROOM >> errorMsg
static constexpr sf::Uint8 RECEIVE_ROOMS = 5; // -> RECEIVE_ROOMS >> roomCount
static constexpr sf::Uint8 JOIN_ROOM_SUCCESS = 6; // -> JOIN_ROOM_SUCCESS >> roomName
static constexpr sf::Uint8 JOIN_ROOM_FAILURE = 7; // -> JOIN_ROOM_FAILURE >> errorMsg

static constexpr sf::Uint8 CREATE_ENTITY = 8; // -> CREATE_ENTITY >> id >> posX >> posY >> spritePath >> type >> script;
static constexpr sf::Uint8 DELETE_ENTITY = 9; // -> DELETE_ENTITY >> entityId
static constexpr sf::Uint8 POSITION_ENTITY = 10; // -> POSITION_ENTITY >> (packet >> entityId >> x >> y)

static constexpr sf::Uint8 GAME_OVER = 11; // -> GAME_OVER

static constexpr sf::Uint8 SCORE_PLAYER = 12; // -> SCORE_PLAYER >> score

inline void write_header(char *buffer, uint16_t seq, uint16_t ack, uint8_t flags = 0)
{
    PacketHeader header;

    header.seq = seq;
    header.ack = ack;
    header.flags = flags;
    header.reserved = 0;
    std::memcpy(buffer, &header, HEADER_SIZE);
}

inline PacketHeader read_header(const char *buffer)
{
    PacketHeader header;

    std::memcpy(&header, buffer, HEADER_SIZE);
    return header;
}

inline std::string packetToString(const sf::Packet &packet)
{
    const void *data = packet.getData();
    std::size_t size = packet.getDataSize();
    return std::string(static_cast<const char*>(data), size);
}

inline sf::Packet stringToPacket(const std::string &str)
{
    sf::Packet packet;
    packet.append(str.data(), str.size());
    return packet;
}
