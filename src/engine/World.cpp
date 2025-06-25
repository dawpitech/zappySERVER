/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** World.cpp
*/

#include "World.hpp"

namespace zappy::engine
{
    World::World(const utils::ZappyConfig& config) {
        this->teams = config.teamNames;
    }

    std::weak_ptr<Player> World::addPlayer(const std::string& teamName) {
        // TODO: random spawn location
        int teamID = -1;
        for (size_t i = 0; i < this->teams.size(); i++) {
            if (this->teams.at(i) != teamName)
                continue;
            teamID = static_cast<int>(i);
            break;
        }
        if (teamID == -1)
            throw std::runtime_error("Unknown team " + teamName);
        this->players.emplace_back(std::make_shared<Player>(2, 2, teamID));
        return {this->players.back()};
    }
}
