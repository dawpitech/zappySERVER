/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Tile.hpp
*/

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "Ressources.hpp"

namespace zappy::engine {

    class Player;

    class Tile {
        public:
            Tile() = default;
            ~Tile() = default;

            void addPlayer(const std::weak_ptr<Player>& player);
            void removePlayer(const std::weak_ptr<Player>& player);
            bool hasPlayer(const std::weak_ptr<Player>& player) const;
            std::vector<std::shared_ptr<Player>> getPlayers() const;
            size_t getPlayerCount() const;

            void addResource(Ressources type, int quantity = 1);
            bool removeResource(Ressources type, int quantity = 1);
            int getResourceQuantity(Ressources type) const;
            const std::map<Ressources, int>& getAllResources() const;
            size_t getResourceTypeCount() const;
            int getTotalResourceCount() const;

        private:
            std::vector<std::weak_ptr<Player>> _players;
            std::map<Ressources, int> _resources;
    };

} // namespace zappy::engine
