/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Ressources.cpp
*/

#include <map>

#include "Ressources.hpp"

namespace zappy::engine
{
    const std::map<Ressources, float>  Ressource::RESSOURCE_DENSITY =  {
        {Ressources::FOOD, 0.5},
        {Ressources::LINEMATE, 0.3},
        {Ressources::DERAUMERE, 0.15},
        {Ressources::SIBUR, 0.1},
        {Ressources::MENDIANE, 0.1},
        {Ressources::PHIRAS, 0.08},
        {Ressources::THYSTAME, 0.05},
    };
}
