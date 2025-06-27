/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Ressources.hpp
*/

#pragma once

#include <algorithm>
#include <optional>
#include <string>

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

    class Ressource
    {
        public:
            const static std::map<Ressources, float> RESSOURCE_DENSITY;
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

    inline std::optional<Ressources> getRessourceFromName(const std::string& name)
    {
        std::string lowerName = name;
        std::ranges::transform(lowerName, lowerName.begin(), ::tolower);

        if (lowerName == "food") return Ressources::FOOD;
        if (lowerName == "linemate") return Ressources::LINEMATE;
        if (lowerName == "deraumere") return Ressources::DERAUMERE;
        if (lowerName == "sibur") return Ressources::SIBUR;
        if (lowerName == "mendiane") return Ressources::MENDIANE;
        if (lowerName == "phiras") return Ressources::PHIRAS;
        if (lowerName == "thystame") return Ressources::THYSTAME;
        return std::nullopt;
    }
}
