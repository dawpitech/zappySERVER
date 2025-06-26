#pragma once

#include <vector>
#include <map>
#include <memory>
#include <algorithm>

namespace zappy::engine {

    class Player;
    enum class ResourceType {
        FOOD,
        LINEMATE,
        DERAUMERE,
        SIBUR,
        MENDIANE,
        PHIRAS,
        THYSTAME
    };

    class Tile {
        public:
            Tile() = default;
            ~Tile() = default;

            void addPlayer(const std::weak_ptr<Player>& player);
            void removePlayer(const std::weak_ptr<Player>& player);
            bool hasPlayer(const std::weak_ptr<Player>& player) const;
            std::vector<std::shared_ptr<Player>> getPlayers() const;
            size_t getPlayerCount() const;

            void addResource(ResourceType type, int quantity = 1);
            bool removeResource(ResourceType type, int quantity = 1);
            int getResourceQuantity(ResourceType type) const;
            const std::map<ResourceType, int>& getAllResources() const;
            size_t getResourceTypeCount() const;
            int getTotalResourceCount() const;

        private:
            std::vector<std::weak_ptr<Player>> _players;
            std::map<ResourceType, int> _resources;
    };

} // namespace zappy::engine
