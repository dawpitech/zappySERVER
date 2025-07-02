/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** World.cpp
*/

#include <algorithm>
#include <iostream>
#include <locale>
#include <stdexcept>

#include "World.hpp"
#include "Tile.hpp"
#include "../ZappyServer.hpp"
#include "../utils/Debug.hpp"
#include "../utils/EventSystem.hpp"
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

        for (const auto& team : teams)
            for (int i = 0; i < config.initialTeamSize; i++)
                this->addPlayerEgg(team);
    }

    void World::tick()
    {
        //std::cout << "[TRACE] World ticking" << std::endl;
        for (auto it = players.begin(); it != players.end();) {
            if (this->_zappyServer.isClientDead((*it)->ID)) {
                std::cout << debug::getTS() << "[WARN] PLAYER is dead/disconnected" << std::endl;
                it = players.erase(it);
		//TODO kill player
            } else {
                this->tickPlayer(*it);
                ++it;
            }
        }

        for (auto it = graphical_clients.begin(); it != graphical_clients.end();) {
            if (this->_zappyServer.isClientDead((*it)->getID())) {
                std::cout << debug::getTS() << "[WARN] GRAPHICAL client is disconnected" << std::endl;
                it = graphical_clients.erase(it);
            } else {
                this->tickGraphic(*it);
                ++it;
            }
        }

        if (this->_tickSinceBigBang == 0 || this->_tickSinceBigBang - this->_tickWhenLastRessourceSpawn >= 20)
            this->distributeRandomResources();

        this->_tickSinceBigBang++;
    }

    std::weak_ptr<Player> World::connectPlayer(const std::string& teamName, const unsigned int clientID) {
        int teamID = this->getTeamID(teamName);

        int eggIdx = -1;
        for (int i = 0; i < this->eggs.size(); i++)
            if (this->eggs.at(i)->getTeamID() == teamID)
                eggIdx = i;

        if (eggIdx == -1)
            throw std::runtime_error("No slot available in corresponding team");

        {
            const auto egg = this->eggs.at(eggIdx);

            this->eggs.erase(this->eggs.begin() + eggIdx);
            this->getTileAt(static_cast<int>(egg->getX()), static_cast<int>(egg->getY())).removeEgg(egg);
            this->players.emplace_back(std::make_shared<Player>(egg->getX(), egg->getY(), teamID, clientID, this->_tickSinceBigBang));

            std::cout << debug::getTS() << "[INFO] EGG " << egg->ID << " OF TEAM "
                << teamName << " HAS HATCHED INTO PLAYER " << clientID << " AT "
                << egg->getX() << ":" << egg->getY() << std::endl;
        }

        EventSystem::trigger("player_spawn", this->graphical_clients, this->_zappyServer.getConfig(), *this);
        return {this->players.back()};
    }

    void World::addPlayerEgg(const std::string& teamName)
    {
        this->addPlayerEgg(this->getTeamID(teamName));
    }

    void World::addPlayerEgg(unsigned int teamID)
    {
        const auto& config = this->_zappyServer.getConfig();
        unsigned int randomX = std::rand() % config.worldWidth;
        unsigned int randomY = std::rand() % config.worldHeight;

        this->eggs.emplace_back(std::make_shared<entities::Egg>(randomX, randomY, teamID, ++this->_eggIDCount));
        this->getTileAt(static_cast<int>(randomX), static_cast<int>(randomY)).addEgg(this->eggs.back());
        std::cout << debug::getTS() << "[INFO] EGG " << this->_eggIDCount << " OF TEAM " << this->teams.at(teamID) << " SPAWNED AT " << randomX << ":" << randomY << std::endl;
    }

    std::weak_ptr<GraphicalClient> World::addGraphicalClient() {
        this->graphical_clients.emplace_back(std::make_shared<GraphicalClient>());
        return {this->graphical_clients.back()};
    }

    void World::tickGraphic(const std::shared_ptr<GraphicalClient>& graphic)
    {
        if (graphic->getCommandsBuffer().empty())
            return;

        std::cout << debug::getTS() << "[INFO] EXECUTING COMMAND " << graphic->getCommandsBuffer().front() << std::endl;

            const std::string fullCommand = graphic->getCommandsBuffer().front();
            const std::string action = fullCommand.substr(0, fullCommand.find_first_of(' '));
            std::string args;

            const size_t spacePos = fullCommand.find_first_of(' ');
            if (spacePos != std::string::npos)
                args = fullCommand.substr(spacePos + 1);
            else
                args = "";

            std::erase(args, '#');
            try {
            CommandInterpreter::GRAPHIC_COMMANDS.at(action).handler(*graphic, this->_zappyServer.getConfig(), *this, args);
            } catch (std::out_of_range&) {
                std::cout << debug::getTS() << "[WARN] Unknown command received from graphic client: " << action << std::endl;
                GraphicalClient::sendSuc(graphic->getID(), *this);
            }
        graphic->getCommandsBuffer().pop();
    }

    int World::getTeamID(const std::string& teamName) const
    {
        for (size_t i = 0; i < this->teams.size(); i++) {
            if (this->teams.at(i) != teamName)
                continue;
            return static_cast<int>(i);
        }
        throw std::runtime_error("Unknown team " + teamName);
    }

    std::map<Ressources, int> World::getCurrentRessourcesPlacedOnMap()
    {
        std::map<Ressources, int> RESSOURCES_PRESENT;

        for (int x = 0; x < this->getWidth(); x++) {
            for (int y = 0; y < this->getHeight(); y++) {
                for (const auto& [type, quantity] : this->getTileAt(x, y).getAllResources()) {
                    if (RESSOURCES_PRESENT.contains(type))
                        RESSOURCES_PRESENT.at(type) += quantity;
                    else
                        RESSOURCES_PRESENT.insert(std::make_pair(type, quantity));
                }
            }
        }

        for (const auto rsc : {Ressources::FOOD, Ressources::LINEMATE, Ressources::DERAUMERE, Ressources::SIBUR, Ressources::MENDIANE, Ressources::PHIRAS, Ressources::THYSTAME})
            if (!RESSOURCES_PRESENT.contains(rsc))
                RESSOURCES_PRESENT.insert(std::make_pair(rsc, 0));

        return RESSOURCES_PRESENT;
    }

    void World::tickPlayer(const std::shared_ptr<Player>& player)
    {
        if (this->_tickSinceBigBang - player->getTickAtLastMeal() >= 126)
            player->eat(this->_tickSinceBigBang);

        do {
            if (player->getStatus() == Player::Status::WAITING_BEFORE_EXECUTE) {
                if (player->getWaitingCyclesRemaining() > 0)
                    player->setWaitingCyclesRemaining(player->getWaitingCyclesRemaining() - 1);
                if (player->getWaitingCyclesRemaining() <= 0) {
                    std::cout << debug::getTS() << "[INFO] EXECUTING COMMAND " << player->getCommandsBuffer().front() << std::endl;

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
                    std::cout << debug::getTS() << "[WARN] Unknown command received from player: " << action << std::endl;
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
        std::cout << debug::getTS() << "[TRACE] TICK RESSOURCES RANDOM GENERATOR" << std::endl;
        this->_tickWhenLastRessourceSpawn = this->_tickSinceBigBang;

        const auto mapArea = this->getHeight() * this->getWidth();
        const auto RESSOURCES_ALREADY_PLACED = this->getCurrentRessourcesPlacedOnMap();
        std::map<Ressources, int> RESSOURCES_TO_BE_PLACED;

        for (const auto& rsc : {Ressources::FOOD, Ressources::LINEMATE, Ressources::DERAUMERE, Ressources::SIBUR, Ressources::MENDIANE, Ressources::PHIRAS, Ressources::THYSTAME})
            RESSOURCES_TO_BE_PLACED.insert(std::make_pair(rsc, static_cast<int>(Ressource::RESSOURCE_DENSITY.at(rsc) * static_cast<float>(mapArea))));

        //DEBUG: Used to print ressource generation density
        //std::cout << "TO BE PLACED (WITHOUT CURRENT RESSOURCE TAKEN INTO ACCOUNT)" << std::endl;
        //for (const auto& [type, quantity] : RESSOURCES_TO_BE_PLACED)
        //    std::cout << "\t" << getRessourceName(type) << " x" << quantity << std::endl;

        for (const auto& rsc : {Ressources::FOOD, Ressources::LINEMATE, Ressources::DERAUMERE, Ressources::SIBUR, Ressources::MENDIANE, Ressources::PHIRAS, Ressources::THYSTAME})
            RESSOURCES_TO_BE_PLACED.at(rsc) -= RESSOURCES_ALREADY_PLACED.at(rsc);

        // Should NEVER be the case but its one of the rules of the ressource generation sustem
        for (const auto& rsc : {Ressources::FOOD, Ressources::LINEMATE, Ressources::DERAUMERE, Ressources::SIBUR, Ressources::MENDIANE, Ressources::PHIRAS, Ressources::THYSTAME})
            if (RESSOURCES_TO_BE_PLACED.at(rsc) <= 0)
                RESSOURCES_TO_BE_PLACED.at(rsc) = 1;

        for (const auto& [type, quantity] : RESSOURCES_TO_BE_PLACED) {
            for (int i = 0; i < quantity; i++) {
                const int randomX = std::rand() % this->getMainZappyServer().getConfig().worldWidth;
                const int randomY = std::rand() % this->getMainZappyServer().getConfig().worldHeight;

                this->getTileAt(randomX, randomY).addResource(type);
                //DEBUG: Used to print actual spawned ressources
                //std::cout << "[TRACE] SPAWN RESSOURCE " << getRessourceName(type) << " AT " << randomX << ":" << randomY << std::endl;
            }
        }
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


    [[nodiscard]] std::vector<std::shared_ptr<Player>> World::getPlayers() const {
        return this->players;
    }

    unsigned int World::getEggCount(const std::string& teamName) const
    {
        return this->getEggCount(this->getTeamID(teamName));
    }

    unsigned int World::getEggCount(const unsigned int teamID) const
    {
        int eggCount = 0;
        for (const auto& egg : this->eggs)
            if (egg->getTeamID() == teamID)
                eggCount++;
        return eggCount;
    }
}
