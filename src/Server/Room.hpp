/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** Room.hpp
*/

#pragma once

#include "GameServer.hpp"

class Room {
    public:
    Room() : roomName_(""), level_(0) {}
    Room(std::string RoomName, int level) : roomName_(RoomName), level_(level) {}

    Room(Room&& other) noexcept
        : roomName_(std::move(other.roomName_)),
          level_(other.level_),
          players_(std::move(other.players_)),
          world_(std::move(other.world_)),
          PlayerOwner(std::move(other.PlayerOwner)) {}

    Room& operator=(Room&& other) noexcept {
        if (this != &other) {
            roomName_ = std::move(other.roomName_);
            level_ = other.level_;
            players_ = std::move(other.players_);
            world_ = std::move(other.world_);
            PlayerOwner = std::move(other.PlayerOwner);
        }
        return *this;
    }

        World &getWorld() { return world_; }
        std::vector<Data::Player> &getPlayers() { return players_; }

        void addPlayer(std::string PlayerName, asio::ip::udp::endpoint endpoint, std::size_t idEcs)
        {
            Data::Player player = {idEcs, PlayerName, endpoint};
            players_.push_back(player);
        }

        void delPlayer(std::string PlayerName)
        {
            auto it = std::remove_if(players_.begin(), players_.end(), [&](const Data::Player& player) {
                return player.id == PlayerName;
            });

            if (it != players_.end()) {
                players_.erase(it, players_.end());
            }
        }

        std::string getRoomName()
        {
            return roomName_;
        }

        int getLevel()
        {
            return level_;
        }


    private:
        std::string roomName_;
        int level_;
        std::vector<Data::Player> players_;
        World world_;
        std::string PlayerOwner;
};
