/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Egg.cpp
*/

#include "Egg.hpp"

zappy::engine::entities::Egg::Egg(const unsigned int x, const unsigned int y, const unsigned int teamID, const unsigned int EggID, const unsigned int motherPlayerID):
    ID(EggID), _x(x),
    _y(y), _teamID(teamID), motherPlayerID(motherPlayerID) {}
