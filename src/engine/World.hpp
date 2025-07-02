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
#include "entities/Egg.hpp"

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

            std::weak_ptr<Player> connectPlayer(const std::string& teamName, unsigned int clientID);
            void addPlayerEgg(const std::string& teamName);
            void addPlayerEgg(unsigned int teamID);
            std::weak_ptr<GraphicalClient> addGraphicalClient();

            ZappyServer& getMainZappyServer() const { return this->_zappyServer; }
            const std::vector<std::shared_ptr<GraphicalClient>> &getGraphicalClients() const { return this->graphical_clients; }

            Tile& getTileAt(int x, int y);
            [[nodiscard]] const Tile& getTileAt(int x, int y) const;

            void movePlayer(const std::shared_ptr<Player>& player, int newX, int newY);

            [[nodiscard]] int getWidth() const;
            [[nodiscard]] int getHeight() const;

            [[nodiscard]] std::pair<int, int> normalizeCoordinates(int x, int y) const;

            void distributeRandomResources();
	        [[nodiscard]] std::vector<std::shared_ptr<Player>> getPlayers() const;

            [[nodiscard]] unsigned int getEggCount(const std::string& teamName) const;

        private:
            unsigned int _tickSinceBigBang = 0;
            unsigned int _tickWhenLastRessourceSpawn = 0;
            unsigned int _eggIDCount = 0;

            void tickPlayer(const std::shared_ptr<Player>& player);
            void tickGraphic(const std::shared_ptr<GraphicalClient>& graphic);

            int getTeamID(const std::string& teamName) const;

            std::map<Ressources, int> getCurrentRessourcesPlacedOnMap();

            std::vector<std::shared_ptr<Player>> players;
            std::vector<std::shared_ptr<entities::Egg>> eggs;
            std::vector<std::shared_ptr<GraphicalClient>> graphical_clients;

            std::vector<std::vector<Tile>> _map;

            std::vector<std::string> teams;
            ZappyServer& _zappyServer;
    };
}
