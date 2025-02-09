/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** IEntity
*/

#pragma once

#include <cstddef>

class Entity {
    public:
        Entity(std::size_t Id);
        ~Entity() = default;
        std::size_t getId() const;
        void setId(std::size_t &id_) {
            id = id_;
        }

    private:
        std::size_t id;
};
