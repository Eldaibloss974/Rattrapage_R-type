/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** RendererSystem.cpp
*/

#include "RendererSystem.hpp"

RendererSystem::RendererSystem()
{
    std::cout << "[RendererSystem] TextureManager initialized." << std::endl;
}

componentMask RendererSystem::getRequiredComponents()
{
    componentMask mask;
    mask.set(int(idComponent::Position));
    mask.set(int(idComponent::Sprite));
    return mask;
}