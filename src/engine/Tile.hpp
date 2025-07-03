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
#include "entities/Egg.hpp"

namespace zappy::engine
{
    namespace entities
    {
        class Player;
    }

    class Tile
    {
        public:
            Tile() = default;
            ~Tile() = default;

            void addPlayer(const std::weak_ptr<entities::Player>& player);
            void removePlayer(const std::weak_ptr<entities::Player>& player);
            [[nodiscard]] bool hasPlayer(const std::weak_ptr<entities::Player>& player) const;
            [[nodiscard]] std::vector<std::shared_ptr<entities::Player>> getPlayers() const;
            [[nodiscard]] size_t getPlayerCount() const;

            void addEgg(const std::weak_ptr<entities::Egg>& egg);
            void removeEgg(const std::weak_ptr<entities::Egg>& egg);
            [[nodiscard]] bool hasEgg(const std::weak_ptr<entities::Egg>& egg) const;
            [[nodiscard]] std::vector<std::shared_ptr<entities::Egg>> getEggs() const;
            [[nodiscard]] size_t getEggsCount() const;

            void addResource(Ressources type, int quantity = 1);
            bool removeResource(Ressources type, int quantity = 1);
            [[nodiscard]] int getResourceQuantity(Ressources type) const;
            [[nodiscard]] const std::map<Ressources, int>& getAllResources() const;
            [[nodiscard]] size_t getResourceTypeCount() const;
            [[nodiscard]] int getTotalResourceCount() const;

        private:
            std::vector<std::weak_ptr<entities::Player>> _players;
            std::vector<std::weak_ptr<entities::Egg>> _eggs;
            std::map<Ressources, int> _resources;
    };
} // namespace zappy::engine
