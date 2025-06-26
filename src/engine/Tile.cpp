#include "Tile.hpp"
#include "Player.hpp"

namespace zappy::engine {

namespace {
    bool weakPtrEquals(const std::weak_ptr<Player>& a, const std::weak_ptr<Player>& b) {
        return !a.owner_before(b) && !b.owner_before(a);
    }
}

void Tile::addPlayer(const std::weak_ptr<Player>& player) {
    if (!hasPlayer(player)) {
        _players.push_back(player);
    }
}

void Tile::removePlayer(const std::weak_ptr<Player>& player) {
    const auto it = std::ranges::remove_if(_players, [&](const std::weak_ptr<Player>& p) { return weakPtrEquals(p, player); }).begin();
    _players.erase(it, _players.end());
}

bool Tile::hasPlayer(const std::weak_ptr<Player>& player) const {
    return std::ranges::any_of(_players, [&](const std::weak_ptr<Player>& p) { return weakPtrEquals(p, player); });
}

std::vector<std::shared_ptr<Player>> Tile::getPlayers() const {
    std::vector<std::shared_ptr<Player>> result;
    for (const auto& wp : _players) {
        if (auto sp = wp.lock()) {
            result.push_back(sp);
        }
    }
    return result;
}

size_t Tile::getPlayerCount() const {
    return std::count_if(_players.begin(), _players.end(),
        [](const std::weak_ptr<Player>& wp){ return !wp.expired(); });
}

void Tile::addResource(ResourceType type, int quantity) {
    if (quantity > 0) {
        _resources[type] += quantity;
    }
}

bool Tile::removeResource(ResourceType type, int quantity) {
    const auto it = _resources.find(type);
    if (it == _resources.end() || it->second < quantity) {
        return false;
    }
    it->second -= quantity;
    if (it->second == 0) {
        _resources.erase(it);
    }
    return true;
}

int Tile::getResourceQuantity(ResourceType type) const {
    const auto it = _resources.find(type);
    return (it != _resources.end()) ? it->second : 0;
}

const std::map<ResourceType, int>& Tile::getAllResources() const {
    return _resources;
}

size_t Tile::getResourceTypeCount() const {
    return _resources.size();
}

int Tile::getTotalResourceCount() const {
    int total = 0;
    for (const auto& [type, count] : _resources) {
        total += count;
    }
    return total;
}

} // namespace zappy::engine
