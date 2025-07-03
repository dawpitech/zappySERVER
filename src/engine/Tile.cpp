/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Tile.cpp
*/

#include <algorithm>
#include <ranges>

#include "Tile.hpp"
#include "entities/Player.hpp"

namespace zappy::engine
{
    namespace
    {
        bool weakPtrEquals(const std::weak_ptr<entities::Player>& a, const std::weak_ptr<entities::Player>& b)
        {
            return !a.owner_before(b) && !b.owner_before(a);
        }

        bool weakPtrEquals(const std::weak_ptr<entities::Egg>& a, const std::weak_ptr<entities::Egg>& b)
        {
            return !a.owner_before(b) && !b.owner_before(a);
        }
    }

    void Tile::addPlayer(const std::weak_ptr<entities::Player>& player)
    {
        if (!hasPlayer(player))
        {
            this->_players.push_back(player);
        }
    }

    void Tile::removePlayer(const std::weak_ptr<entities::Player>& player)
    {
        const auto it = std::ranges::remove_if(this->_players, [&](const std::weak_ptr<entities::Player>& p)
        {
            return weakPtrEquals(p, player);
        }).begin();
        this->_players.erase(it, this->_players.end());
    }

    bool Tile::hasPlayer(const std::weak_ptr<entities::Player>& player) const
    {
        return std::ranges::any_of(this->_players, [&](const std::weak_ptr<entities::Player>& p)
        {
            return weakPtrEquals(p, player);
        });
    }

    std::vector<std::shared_ptr<entities::Player>> Tile::getPlayers() const
    {
        std::vector<std::shared_ptr<entities::Player>> result;
        for (const auto& wp : this->_players)
        {
            if (auto sp = wp.lock())
            {
                result.push_back(sp);
            }
        }
        return result;
    }

    size_t Tile::getPlayerCount() const
    {
        return std::ranges::count_if(this->_players, [](const std::weak_ptr<entities::Player>& wp)
        {
            return !wp.expired();
        });
    }

    void Tile::addEgg(const std::weak_ptr<entities::Egg>& egg)
    {
        if (!this->hasEgg(egg))
            this->_eggs.push_back(egg);
    }

    void Tile::removeEgg(const std::weak_ptr<entities::Egg>& egg)
    {
        const auto it = std::ranges::remove_if(this->_eggs, [&](const std::weak_ptr<entities::Egg>& p)
        {
            return weakPtrEquals(p, egg);
        }).begin();
        this->_eggs.erase(it, this->_eggs.end());
    }

    bool Tile::hasEgg(const std::weak_ptr<entities::Egg>& egg) const
    {
        return std::ranges::any_of(this->_eggs, [&](const std::weak_ptr<entities::Egg>& p)
        {
            return weakPtrEquals(p, egg);
        });
    }

    std::vector<std::shared_ptr<entities::Egg>> Tile::getEggs() const
    {
        std::vector<std::shared_ptr<entities::Egg>> result;
        for (const auto& wp : this->_eggs)
        {
            if (auto sp = wp.lock())
            {
                result.push_back(sp);
            }
        }
        return result;
    }

    size_t Tile::getEggsCount() const
    {
        return std::ranges::count_if(this->_eggs, [](const std::weak_ptr<entities::Egg>& wp) { return !wp.expired(); });
    }

    void Tile::addResource(const Ressources type, const int quantity)
    {
        if (quantity > 0)
        {
            this->_resources[type] += quantity;
        }
    }

    bool Tile::removeResource(const Ressources type, const int quantity)
    {
        const auto it = this->_resources.find(type);
        if (it == this->_resources.end() || it->second < quantity)
        {
            return false;
        }
        it->second -= quantity;
        if (it->second == 0)
        {
            this->_resources.erase(it);
        }
        return true;
    }

    int Tile::getResourceQuantity(const Ressources type) const
    {
        const auto it = this->_resources.find(type);
        return it != this->_resources.end() ? it->second : 0;
    }

    const std::map<Ressources, int>& Tile::getAllResources() const
    {
        return this->_resources;
    }

    size_t Tile::getResourceTypeCount() const
    {
        return this->_resources.size();
    }

    int Tile::getTotalResourceCount() const
    {
        int total = 0;
        for (const auto& count : this->_resources | std::views::values)
            total += count;
        return total;
    }
}
