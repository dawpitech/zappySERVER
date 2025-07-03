/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** World.hpp
*/

#pragma once

#include <memory>
#include <vector>

#include "Tile.hpp"
#include "../utils/ZappyConfig.hpp"
#include "entities/Egg.hpp"
#include "entities/Player.hpp"

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

            std::weak_ptr<entities::Player> connectPlayer(const std::string& teamName, unsigned int clientID);
            std::weak_ptr<entities::Egg> addPlayerEgg(const std::string& teamName, unsigned int motherPlayerID);
            std::weak_ptr<entities::Egg> addPlayerEgg(unsigned int teamID, unsigned int motherPlayerID);
            std::weak_ptr<GraphicalClient> addGraphicalClient();

            [[nodiscard]] ZappyServer& getMainZappyServer() const { return this->_zappyServer; }
            [[nodiscard]] const std::vector<std::shared_ptr<GraphicalClient>> &getGraphicalClients() const { return this->graphicalClients; }

            Tile& getTileAt(int x, int y);
            [[nodiscard]] const Tile& getTileAt(int x, int y) const;

            void movePlayer(const std::shared_ptr<entities::Player>& player, int newX, int newY);

            [[nodiscard]] int getWidth() const;
            [[nodiscard]] int getHeight() const;

            [[nodiscard]] std::pair<int, int> normalizeCoordinates(int x, int y) const;

            void distributeRandomResources();
	    [[nodiscard]] std::vector<std::shared_ptr<entities::Player>> getPlayers() const;
	    [[nodiscard]] std::shared_ptr<entities::Player> getPlayer(unsigned int id) const;

            [[nodiscard]] unsigned int getEggCount(const std::string& teamName) const;
            [[nodiscard]] unsigned int getEggCount(unsigned int teamID) const;
            [[nodiscard]] std::vector<std::shared_ptr<entities::Egg>> getEggs() const;

            [[nodiscard]] std::string getTeamName(unsigned int teamID) const;

            void doEggCleanup();
            void doPlayerCleanup();
            void doGraphicalCleanup();

        private:
            unsigned int _tickSinceBigBang = 0;
            unsigned int _tickWhenLastRessourceSpawn = 0;
            unsigned int _eggIDCount = 0;

            void tickPlayer(const std::shared_ptr<entities::Player>& player);
            void tickGraphic(const std::shared_ptr<GraphicalClient>& graphic) const;

            int getTeamID(const std::string& teamName) const;

            std::map<Ressources, int> getCurrentRessourcesPlacedOnMap();

            std::vector<std::shared_ptr<entities::Player>> players;
            std::vector<std::shared_ptr<entities::Egg>> eggs;
            std::vector<std::shared_ptr<GraphicalClient>> graphicalClients;

            std::vector<std::vector<Tile>> _map;

            std::vector<std::string> teams;
            ZappyServer& _zappyServer;
    };
}
