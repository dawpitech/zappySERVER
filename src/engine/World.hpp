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

namespace zappy {
    class ZappyServer;
}

namespace zappy::engine
{
    class World
    {
        public:
            explicit World(const utils::ZappyConfig& config, ZappyServer& zappyServer);

            void tick();

            std::weak_ptr<Player> addPlayer(const std::string& teamName, unsigned int clientID);
            std::weak_ptr<GraphicalClient> addGraphicalClient();

            ZappyServer& getMainZappyServer() { return this->_zappyServer; }

        private:
            void tickPlayer(const std::shared_ptr<Player>& player);

            std::vector<std::shared_ptr<Player>> players;
            std::vector<std::shared_ptr<GraphicalClient>> graphical_clients;

            std::vector<std::string> teams;
            ZappyServer& _zappyServer;
    };
}
