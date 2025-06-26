/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** World.cpp
*/

#include <iostream>

#include "World.hpp"

#include <locale>
#include <stdexcept>

#include "Tile.hpp"
#include "../ZappyServer.hpp"
#include "actions/CommandInterpreter.hpp"
#include "graphical/Graphical.hpp"

namespace zappy::engine
{
    World::World(const utils::ZappyConfig& config, ZappyServer& zappyServer)
        : teams(config.teamNames), _zappyServer(zappyServer)
    {
        _map.resize(config.worldHeight);
        for (int y = 0; y < config.worldHeight; ++y)
            _map[y].resize(config.worldHeight);
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    void World::tick()
    {
        //std::cout << "[TRACE] World ticking" << std::endl;
        for (const auto& player : this->players)
            this->tickPlayer(player);

        for (const auto& graphic : this->graphical_clients)
            this->tickGraphic(graphic);
    }

    std::weak_ptr<Player> World::addPlayer(const std::string& teamName, const unsigned int clientID) {
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

        const auto& config = this->_zappyServer.getConfig();
        unsigned int randomX = std::rand() % config.worldWidth;
        unsigned int randomY = std::rand() % config.worldHeight;

        this->players.emplace_back(std::make_shared<Player>(randomX, randomY, teamID, clientID));
        std::cout << "[TRACE] PLAYER SPAWNED AT " << randomX << ":" << randomY << std::endl;
        return {this->players.back()};
    }

    std::weak_ptr<GraphicalClient> World::addGraphicalClient() {
        this->graphical_clients.emplace_back(std::make_shared<GraphicalClient>());
        return {this->graphical_clients.back()};
    }

    void World::tickGraphic(const std::shared_ptr<GraphicalClient>& graphic)
    {
        if (graphic->getCommandsBuffer().empty())
            return;
        
        std::cout << "[INFO] EXECUTING COMMAND " << graphic->getCommandsBuffer().front() << std::endl;

        std::string fullCommand = graphic->getCommandsBuffer().front();
        std::string action = fullCommand.substr(0, fullCommand.find_first_of(' '));

	try {
        CommandInterpreter::GRAPHIC_COMMANDS.at(action).handler(*graphic, this->_zappyServer.getConfig(), *this, fullCommand);
	} catch (std::out_of_range&) {
	    std::cout << "[WARN] Unknown command received from graphic client: " << action << std::endl;
	}
        graphic->getCommandsBuffer().pop();
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

                    CommandInterpreter::COMMANDS.at(action).handler(player, *this, fullCommand);
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
                    this->getMainZappyServer().sendMessageToClient("ko", player->ID);
                }
            }
        } while (player->getWaitingCyclesRemaining() <= 0);
    }

    std::pair<int, int> World::normalizeCoordinates(int x, int y) const {
        const auto width = static_cast<int>(this->_zappyServer.getConfig().worldWidth);
        const auto height = static_cast<int>(this->_zappyServer.getConfig().worldHeight);
        x = ((x % width) + width) % width;
        y = ((y % height) + height) % height;
        return {x, y};
    }

    void World::distributeRandomResources()
    {
        return;
    }

    Tile& World::getTileAt(const int x, const int y) {
        auto [nx, ny] = normalizeCoordinates(x, y);
        return _map[ny][nx];
    }

    const Tile& World::getTileAt(const int x, const int y) const {
        auto [nx, ny] = normalizeCoordinates(x, y);
        return _map[ny][nx];
    }

    void World::movePlayer(const std::shared_ptr<Player>& player, const int newX, const int newY) {
        const auto oldX = static_cast<int>(player->getX());
        const auto oldY = static_cast<int>(player->getY());
        getTileAt(oldX, oldY).removePlayer(player);
        auto [nx, ny] = normalizeCoordinates(newX, newY);
        player->setPosition(nx, ny);
        getTileAt(nx, ny).addPlayer(player);
    }

    int World::getWidth() const
    {
        return static_cast<int>(this->_zappyServer.getConfig().worldWidth);
    }

    int World::getHeight() const
    {
        return static_cast<int>(this->_zappyServer.getConfig().worldWidth);
    }
}
