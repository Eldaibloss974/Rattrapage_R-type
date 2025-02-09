/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** Data.cpp
*/

#include "Data.hpp"

Data::Data() {}

Data::~Data() {}

bool Data::ifPlayerExist(const std::string &playerId)
{
    return std::any_of(players_.begin(), players_.end(), [&playerId](const Player &player) {
        return player.id == playerId;
    });
}

bool Data::addPlayer(const std::string &playerId, const asio::ip::udp::endpoint &endpoint)
{
    if (ifPlayerExist(playerId)) {
        return false;
    }
    players_.push_back({0, playerId, endpoint});
    return true;
}

bool Data::removePlayer(const std::string &playerId)
{
    auto it = std::remove_if(players_.begin(), players_.end(), [&playerId](const Player &player) {
        return player.id == playerId;
    });

    if (it != players_.end()) {
        players_.erase(it, players_.end());
        return true;
    }

    return false;
}

const std::vector<Data::Player>& Data::getAllPlayers() const
{
    return players_;
}

size_t Data::getPlayerCount() const
{
    return players_.size();
}

bool Data::updatePlayerIdEcs(const std::string &playerId, std::size_t newIdEcs)
{
    auto it = std::find_if(players_.begin(), players_.end(), [&playerId](const Player& player) {
        return player.id == playerId;
    });
    if (it != players_.end()) {
        it->idEcs = newIdEcs;
        return true;
    }
    return false;
}

std::optional<std::size_t> Data::getPlayerIdEcs(const std::string &playerId) const
{
    auto it = std::find_if(players_.begin(), players_.end(), [&playerId](const Player &player) {
        return player.id == playerId;
    });

    if (it != players_.end()) {
        return it->idEcs;
    }
    return std::nullopt;
}

std::optional<Data::Player> Data::getPlayer(const std::string &playerId) const
{
    auto it = std::find_if(players_.begin(), players_.end(), [&](const Player &player) {
        return player.id == playerId;
    });
    if (it != players_.end()) {
        return *it;
    }
    return std::nullopt;
}
