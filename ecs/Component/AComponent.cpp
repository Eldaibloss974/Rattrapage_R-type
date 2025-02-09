/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** AComponent
*/

#include "AComponent.hpp"

AComponent::AComponent(idComponent ID) : Id(ID)
{
}

idComponent AComponent::getId() const
{
    return Id;
}
