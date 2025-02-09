/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** AComponent
*/

#pragma once

#include "IComponent.hpp"

class AComponent : public IComponent
{
public:
    virtual ~AComponent() = default;

    idComponent getId() const;

protected:
    AComponent(idComponent ID);

    const idComponent Id;

private:
};
