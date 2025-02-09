/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** IEntity
*/

#include "Entity.hpp"

Entity::Entity(std::size_t Id) : id(Id)
{}

std::size_t Entity::getId() const
{
    return id;
}
