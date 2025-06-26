/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Directions.hpp
*/

#pragma once

namespace zappy::engine
{
    enum class Directions
    {
        NORTH,
        EAST,
        SOUTH,
        WEST,
    };

    static std::string directionToString(const Directions direction)
    {
        switch (direction)
        {
            case Directions::NORTH: return "NORTH";
            case Directions::EAST: return "EAST";
            case Directions::SOUTH: return "SOUTH";
            case Directions::WEST: return "WEST";
            default: return "?????";
        }
    }
}
