/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** World.cpp
*/

#include "World.hpp"
#include "graphical/Graphical.hpp"

#include <iostream>

#include "actions/CommandInterpreter.hpp"

namespace zappy::engine
{
    World::World(const utils::ZappyConfig& config) {
        this->teams = config.teamNames;
    }

    void World::tick()
    {
        //std::cout << "[TRACE] World ticking" << std::endl;
        for (const auto& player : this->players)
            this->tickPlayer(player);
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

    std::weak_ptr<GraphicalClient> World::addGraphicalClient() {
        this->graphical_clients.emplace_back(std::make_shared<GraphicalClient>());
        return {this->graphical_clients.back()};
    }

    void World::tickPlayer(const std::shared_ptr<Player>& player)
    {
        do {
            if (player->getStatus() == Player::Status::WAITING_BEFORE_EXECUTE) {
                player->setWaitingCyclesRemaining(player->getWaitingCyclesRemaining() - 1);
                if (player->getWaitingCyclesRemaining() <= 0) {
                    std::cout << "[INFO] EXECUTING COMMAND " << player->getCommandsBuffer().front() << std::endl;

                    std::string fullCommand = player->getCommandsBuffer().front();
                    std::string action = fullCommand.substr(0, fullCommand.find_first_of(' '));

                    CommandInterpreter::COMMANDS.at(action).handler(*player, *this, fullCommand);
                    player->getCommandsBuffer().pop();
                    player->setStatus(Player::Status::WAITING_FOR_COMMAND);
                }
            }
            if (player->getStatus() == Player::Status::WAITING_FOR_COMMAND) {
                if (player->getCommandsBuffer().empty())
                    return;

                std::string fullCommand = player->getCommandsBuffer().front();
                std::string action = fullCommand.substr(0, fullCommand.find_first_of(' '));

                try {
                    const unsigned int duration = CommandInterpreter::COMMANDS.at(action).duration;
                    player->setWaitingCyclesRemaining(duration);
                    player->setStatus(Player::Status::WAITING_BEFORE_EXECUTE);
                } catch (std::out_of_range&) {
                    std::cout << "[WARN] Unknown command received from player: " << action << std::endl;
                    player->getCommandsBuffer().pop();
                }
            }
        } while (player->getWaitingCyclesRemaining() <= 0);
    }
}
