/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** Data.hpp
*/

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <asio/ip/udp.hpp>
#include <optional>

class Data {
    public:
        struct Player {
            std::size_t idEcs;
            std::string id;
            asio::ip::udp::endpoint endpoint;
        };

        Data();
        ~Data();

        bool ifPlayerExist(const std::string &playerId);

        bool addPlayer(const std::string &playerId, const asio::ip::udp::endpoint &endpoint);

        bool removePlayer(const std::string &playerId);

        const std::vector<Player>& getAllPlayers() const;

        bool updatePlayerIdEcs(const std::string &playerId, std::size_t newIdEcs);

        size_t getPlayerCount() const;

        std::optional<std::size_t> getPlayerIdEcs(const std::string &playerId) const;
        std::optional<Player> getPlayer(const std::string &playerId) const;
        std::vector<Player> players_;

    private:
};
