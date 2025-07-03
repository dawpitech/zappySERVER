/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdIncantation.hpp
*/

#pragma once

#include <map>
#include <memory>
#include <string>

#include "../../Ressources.hpp"

namespace zappy::engine {
    class World;
}

namespace zappy::engine {
    class Player;
}

namespace zappy::engine::cmd
{
    class CmdIncantation
    {
        public:
            static void cmdIncantation(std::weak_ptr<Player> player, World& world, const std::string& args);
            static bool cmdPreIncantation(std::weak_ptr<Player> player, World& world, const std::string& args);

        private:
            struct LevelInfo
            {
                int numberOfRequiredPlayers;
                std::map<Ressources, int> requiredRessources;
            };

            const static std::map<unsigned int, LevelInfo> ELEVATION_101;
            static bool hasRequiredRessources(const std::map<Ressources, int>& present, const std::map<Ressources, int>& required);
            static bool canIncantationBeDone(Player& player, World& world);
    };
}
