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
#include "Tile.hpp"
#include "../utils/ZappyConfig.hpp"

namespace zappy {
    class ZappyServer;
}

namespace zappy::engine
{
    class GraphicalClient;
    
    class World
    {
        public:
            explicit World(const utils::ZappyConfig& config, ZappyServer& zappyServer);

            void tick();

            std::weak_ptr<Player> addPlayer(const std::string& teamName, unsigned int clientID);
            std::weak_ptr<GraphicalClient> addGraphicalClient();

            ZappyServer& getMainZappyServer() { return this->_zappyServer; }

            Tile& getTileAt(int x, int y);
            [[nodiscard]] const Tile& getTileAt(int x, int y) const;

            void movePlayer(const std::shared_ptr<Player>& player, int newX, int newY);

            [[nodiscard]] int getWidth() const;
            [[nodiscard]] int getHeight() const;

            [[nodiscard]] std::pair<int, int> normalizeCoordinates(int x, int y) const;

            void distributeRandomResources();
	    [[nodiscard]] std::vector<std::shared_ptr<Player>> getPlayers() const;

        private:
            void tickPlayer(const std::shared_ptr<Player>& player);
            void tickGraphic(const std::shared_ptr<GraphicalClient>& graphic);

            std::vector<std::shared_ptr<Player>> players;
            std::vector<std::shared_ptr<GraphicalClient>> graphical_clients;

            std::vector<std::vector<Tile>> _map;

            std::vector<std::string> teams;
            ZappyServer& _zappyServer;
    };
}
