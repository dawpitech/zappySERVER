/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Ressources.hpp
*/

#pragma once

namespace zappy::engine
{
    enum class Ressources
    {
        FOOD,
        LINEMATE,
        DERAUMERE,
        SIBUR,
        MENDIANE,
        PHIRAS,
        THYSTAME,
    };

    inline std::string getRessourceName(const Ressources& ressource)
    {
        switch (ressource)
        {
            case Ressources::FOOD: return "food";
            case Ressources::LINEMATE: return "linemate";
            case Ressources::DERAUMERE: return "deraumere";
            case Ressources::SIBUR: return "sibur";
            case Ressources::MENDIANE: return "mendiane";
            case Ressources::PHIRAS: return "phiras";
            case Ressources::THYSTAME: return "thystame";
        }
        return "????";
    }
}
