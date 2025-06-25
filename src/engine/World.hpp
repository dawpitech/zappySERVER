/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** World.hpp
*/

#pragma once

#include <memory>
#include <vector>

#include "Player.hpp"
#include "../utils/ZappyConfig.hpp"
#include "graphical/Graphical.hpp"

namespace zappy::engine
{
    class World
    {
        public:
            explicit World(const utils::ZappyConfig& config);

            void tick();

            std::weak_ptr<Player> addPlayer(const std::string& teamName);
            std::weak_ptr<GraphicalClient> addGraphicalClient();

        private:
            std::vector<std::shared_ptr<Player>> players;
            std::vector<std::shared_ptr<GraphicalClient>> graphical_clients;
            std::vector<std::string> teams;
    };
}
