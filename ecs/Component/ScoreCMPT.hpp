/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-enzo.dziewulski
** File description:
** ScoreComponent.hpp
*/

#pragma once

#include "AComponent.hpp"
#include <iostream>

class ScoreComponent : public AComponent
{
    public:

        ScoreComponent(int score) 
            : AComponent(idComponent::Score), _score(score) {}
        ~ScoreComponent() = default;

        static idComponent getStaticId() {
            return idComponent::Score;
        }

        void addScore(int score) {
            _score += score;
        }

        int getScore() const {
            return _score;
        }

        void setScore(int score) {
            _score = score;
        }

    private:
        int _score;
};
