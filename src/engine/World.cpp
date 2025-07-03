/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** World.cpp
*/

#include <algorithm>
#include <cstring>
#include <exception>
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
        std::srand(static_cast<unsigned int>(std::time(nullptr))); // NOLINT(*-msc51-cpp)

        for (const auto& team : teams)
            for (int i = 0; i < config.initialTeamSize; i++)
                this->addPlayerEgg(team, -1);

        for (int i = 0; i < teams.size(); i++)
            this->teamWins.emplace_back(false);
    }

    void World::tick()
    {
        //std::cout << "[TRACE] World ticking" << std::endl;
        for (const auto& player : this->players)
        {
            if (player->isDead())
                std::cout << debug::getTS() << "[WARN] PLAYER " << player->ID << " was disconnected" << std::endl;
            else
                this->tickPlayer(player);
        }

        for (const auto& graphical : this->graphicalClients)
        {
            if (graphical->isDead())
                std::cout << debug::getTS() << "[WARN] GUI CLIENT " << graphical->getID() << " was disconnected" <<
                    std::endl;
            else
                this->tickGraphic(graphical);
        }

        if (this->_tickSinceBigBang == 0 || this->_tickSinceBigBang - this->_tickWhenLastRessourceSpawn >= 20)
            this->distributeRandomResources();

        this->doEggCleanup();
        this->doPlayerCleanup();
        this->doGraphicalCleanup();
        this->checkForWin();
        this->_tickSinceBigBang++;
    }

    std::weak_ptr<entities::Player> World::connectPlayer(const std::string& teamName, const unsigned int clientID)
    {
        int teamID = this->getTeamID(teamName);

        int eggIdx = -1;
        for (int i = 0; i < this->eggs.size(); i++)
            if (this->eggs.at(i)->getTeamID() == teamID && !this->eggs.at(i)->isDead())
                eggIdx = i;

        if (eggIdx == -1)
            throw std::runtime_error("No slot available in corresponding team");

        {
            const auto egg = this->eggs.at(eggIdx);

            this->eggs.erase(this->eggs.begin() + eggIdx);
            this->getTileAt(static_cast<int>(egg->getX()), static_cast<int>(egg->getY())).removeEgg(egg);
            this->players.emplace_back(
                std::make_shared<entities::Player>(egg->getX(), egg->getY(), teamID, clientID,
                                                   this->_tickSinceBigBang));

            std::cout << debug::getTS() << "[INFO] EGG " << egg->ID << " OF TEAM "
                << teamName << " HAS HATCHED INTO PLAYER " << clientID << " AT "
                << egg->getX() << ":" << egg->getY() << std::endl;
            EventSystem::trigger("player_egg_spawn", this->graphicalClients, this->_zappyServer.getConfig(), *this,
                                 egg->ID);
        }

        EventSystem::trigger("player_spawn", this->graphicalClients, this->_zappyServer.getConfig(), *this);
        return {this->players.back()};
    }

    std::weak_ptr<entities::Egg> World::addPlayerEgg(const std::string& teamName, const unsigned int motherPlayerID)
    {
        return this->addPlayerEgg(this->getTeamID(teamName), motherPlayerID);
    }

    std::weak_ptr<entities::Egg> World::addPlayerEgg(const unsigned int teamID, const unsigned int motherPlayerID)
    {
        const auto& config = this->_zappyServer.getConfig();
        unsigned int randomX = std::rand() % config.worldWidth; // NOLINT(*-msc50-cpp)
        unsigned int randomY = std::rand() % config.worldHeight; // NOLINT(*-msc50-cpp)

        this->eggs.emplace_back(
            std::make_shared<entities::Egg>(randomX, randomY, teamID, ++this->_eggIDCount, motherPlayerID));
        this->getTileAt(static_cast<int>(randomX), static_cast<int>(randomY)).addEgg(this->eggs.back());
        std::cout << debug::getTS() << "[INFO] EGG " << this->_eggIDCount << " OF TEAM " << this->teams.at(teamID) <<
            " SPAWNED AT " << randomX << ":" << randomY << std::endl;
        return this->eggs.back();
    }

    std::weak_ptr<GraphicalClient> World::addGraphicalClient()
    {
        this->graphicalClients.emplace_back(std::make_shared<GraphicalClient>());
        return {this->graphicalClients.back()};
    }

    void World::tickGraphic(const std::shared_ptr<GraphicalClient>& graphic) const
    {
        if (graphic->getCommandsBuffer().empty())
            return;

        std::cout << debug::getTS() << "[INFO] EXECUTING COMMAND " << graphic->getCommandsBuffer().front() << std::endl;

        const std::string fullCommand = graphic->getCommandsBuffer().front();
        const std::string action = fullCommand.substr(0, fullCommand.find_first_of(' '));
        std::string args;

        // ReSharper disable once CppTooWideScopeInitStatement
        const size_t spacePos = fullCommand.find_first_of(' ');
        if (spacePos != std::string::npos)
            args = fullCommand.substr(spacePos + 1);
        else
            args = "";

        std::erase(args, '#');
        try
        {
            CommandInterpreter::GRAPHIC_COMMANDS.at(action).handler(*graphic, this->_zappyServer.getConfig(), *this,
                                                                    args);
        }
        catch (std::out_of_range&)
        {
            std::cout << debug::getTS() << "[WARN] Unknown command received from graphic client: " << action <<
                std::endl;
            GraphicalClient::sendSuc(graphic->getID(), *this);
        }
        graphic->getCommandsBuffer().pop();
    }

    int World::getTeamID(const std::string& teamName) const
    {
        for (size_t i = 0; i < this->teams.size(); i++)
        {
            if (this->teams.at(i) != teamName)
                continue;
            return static_cast<int>(i);
        }
        throw std::runtime_error("Unknown team " + teamName);
    }

    std::map<Ressources, int> World::getCurrentRessourcesPlacedOnMap()
    {
        std::map<Ressources, int> RESSOURCES_PRESENT;

        for (int x = 0; x < this->getWidth(); x++)
        {
            for (int y = 0; y < this->getHeight(); y++)
            {
                for (const auto& [type, quantity] : this->getTileAt(x, y).getAllResources())
                {
                    if (RESSOURCES_PRESENT.contains(type))
                        RESSOURCES_PRESENT.at(type) += quantity;
                    else
                        RESSOURCES_PRESENT.insert(std::make_pair(type, quantity));
                }
            }
        }

        for (const auto rsc : {
                 Ressources::FOOD, Ressources::LINEMATE, Ressources::DERAUMERE, Ressources::SIBUR, Ressources::MENDIANE,
                 Ressources::PHIRAS, Ressources::THYSTAME
             })
            if (!RESSOURCES_PRESENT.contains(rsc))
                RESSOURCES_PRESENT.insert(std::make_pair(rsc, 0));

        return RESSOURCES_PRESENT;
    }

    void World::tickPlayer(const std::shared_ptr<entities::Player>& player)
    {
        if (this->_tickSinceBigBang - player->getTickAtLastMeal() >= 126)
        {
            player->eat(this->_tickSinceBigBang);
            EventSystem::trigger("player_eat", this->graphicalClients, this->_zappyServer.getConfig(), *this,
                                 static_cast<unsigned int>(0), player->ID);
        }

        do
        {
            if (player->getStatus() == entities::Player::Status::WAITING_BEFORE_EXECUTE)
            {
                if (player->getWaitingCyclesRemaining() > 0)
                    player->setWaitingCyclesRemaining(player->getWaitingCyclesRemaining() - 1);
                if (player->getWaitingCyclesRemaining() <= 0)
                {
                    std::cout << debug::getTS() << "[INFO] EXECUTING COMMAND " << player->getCommandsBuffer().front() <<
                        std::endl;

                    std::string fullCommand = player->getCommandsBuffer().front();
                    std::string action = fullCommand.substr(0, fullCommand.find_first_of(' '));

                    CommandInterpreter::COMMANDS.at(action).handler(player, *this, fullCommand);
                    player->getCommandsBuffer().pop();
                    player->setStatus(entities::Player::Status::WAITING_FOR_COMMAND);
                }
            }
            if (player->getStatus() == entities::Player::Status::WAITING_FOR_COMMAND)
            {
                if (player->getCommandsBuffer().empty())
                    return;

                std::string fullCommand = player->getCommandsBuffer().front();
                std::string action = fullCommand.substr(0, fullCommand.find_first_of(' '));

                try
                {
                    const unsigned int duration = CommandInterpreter::COMMANDS.at(action).duration;
                    player->setWaitingCyclesRemaining(duration);
                    player->setStatus(entities::Player::Status::WAITING_BEFORE_EXECUTE);
                    if (!CommandInterpreter::COMMANDS.at(action).preHandler(player, *this, fullCommand))
                    {
                        player->getCommandsBuffer().pop();
                        player->setStatus(entities::Player::Status::WAITING_FOR_COMMAND);
                    }
                }
                catch (std::out_of_range&)
                {
                    std::cout << debug::getTS() << "[WARN] Unknown command received from player: " << action <<
                        std::endl;
                    player->getCommandsBuffer().pop();
                    this->getMainZappyServer().sendMessageToClient("ko", player->ID);
                }
            }
        }
        while (player->getWaitingCyclesRemaining() <= 0);
    }

    std::pair<int, int> World::normalizeCoordinates(int x, int y) const
    {
        const auto width = static_cast<int>(this->_zappyServer.getConfig().worldWidth);
        const auto height = static_cast<int>(this->_zappyServer.getConfig().worldHeight);
        x = (x % width + width) % width;
        y = (y % height + height) % height;
        return {x, y};
    }

    void World::distributeRandomResources()
    {
        std::cout << debug::getTS() << "[TRACE] TICK RESSOURCES RANDOM GENERATOR" << std::endl;
        this->_tickWhenLastRessourceSpawn = this->_tickSinceBigBang;

        const auto mapArea = this->getHeight() * this->getWidth();
        const auto RESSOURCES_ALREADY_PLACED = this->getCurrentRessourcesPlacedOnMap();
        std::map<Ressources, int> RESSOURCES_TO_BE_PLACED;

        for (const auto& rsc : {
                 Ressources::FOOD, Ressources::LINEMATE, Ressources::DERAUMERE, Ressources::SIBUR, Ressources::MENDIANE,
                 Ressources::PHIRAS, Ressources::THYSTAME
             })
            RESSOURCES_TO_BE_PLACED.insert(std::make_pair(
                rsc, static_cast<int>(Ressource::RESSOURCE_DENSITY.at(rsc) * static_cast<float>(mapArea))));

        //DEBUG: Used to print ressource generation density
        //std::cout << "TO BE PLACED (WITHOUT CURRENT RESSOURCE TAKEN INTO ACCOUNT)" << std::endl;
        //for (const auto& [type, quantity] : RESSOURCES_TO_BE_PLACED)
        //    std::cout << "\t" << getRessourceName(type) << " x" << quantity << std::endl;

        for (const auto& rsc : {
                 Ressources::FOOD, Ressources::LINEMATE, Ressources::DERAUMERE, Ressources::SIBUR, Ressources::MENDIANE,
                 Ressources::PHIRAS, Ressources::THYSTAME
             })
            RESSOURCES_TO_BE_PLACED.at(rsc) -= RESSOURCES_ALREADY_PLACED.at(rsc);

        // Should NEVER be the case but its one of the rules of the ressource generation system
        for (const auto& rsc : {
                 Ressources::FOOD, Ressources::LINEMATE, Ressources::DERAUMERE, Ressources::SIBUR, Ressources::MENDIANE,
                 Ressources::PHIRAS, Ressources::THYSTAME
             })
            if (RESSOURCES_TO_BE_PLACED.at(rsc) <= 0)
                RESSOURCES_TO_BE_PLACED.at(rsc) = 1;

        for (const auto& [type, quantity] : RESSOURCES_TO_BE_PLACED)
        {
            for (int i = 0; i < quantity; i++)
            {
                const int randomX = static_cast<int>(std::rand() % this->getMainZappyServer().getConfig().worldWidth); // NOLINT(*-msc50-cpp)
                const int randomY = static_cast<int>(std::rand() % this->getMainZappyServer().getConfig().worldHeight); // NOLINT(*-msc50-cpp)

                this->getTileAt(randomX, randomY).addResource(type);
                //DEBUG: Used to print actual spawned ressources
                //std::cout << "[TRACE] SPAWN RESSOURCE " << getRessourceName(type) << " AT " << randomX << ":" << randomY << std::endl;
            }
        }
        EventSystem::trigger("map_refill", this->graphicalClients, this->_zappyServer.getConfig(), *this);
    }

    Tile& World::getTileAt(const int x, const int y)
    {
        auto [nx, ny] = normalizeCoordinates(x, y);
        return _map[ny][nx];
    }

    const Tile& World::getTileAt(const int x, const int y) const
    {
        auto [nx, ny] = normalizeCoordinates(x, y);
        return _map[ny][nx];
    }

    void World::movePlayer(const std::shared_ptr<entities::Player>& player, const int newX, const int newY)
    {
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


    [[nodiscard]] std::vector<std::shared_ptr<entities::Player>> World::getPlayers() const
    {
        return this->players;
    }

    [[nodiscard]] std::shared_ptr<entities::Player> World::getPlayer(const unsigned int id) const
    {
        for (const auto& player : this->players)
        {
            if (player->ID == id)
                return player;
        }
        throw std::exception();
    }

    unsigned int World::getEggCount(const std::string& teamName) const
    {
        return this->getEggCount(this->getTeamID(teamName));
    }

    std::vector<std::shared_ptr<entities::Egg>> World::getEggs() const
    {
        return this->eggs;
    }

    unsigned int World::getEggCount(const unsigned int teamID) const
    {
        int eggCount = 0;
        for (const auto& egg : this->eggs)
            if (egg->getTeamID() == teamID && !egg->isDead())
                eggCount++;
        return eggCount;
    }

    std::string World::getTeamName(const unsigned int teamID) const
    {
        return this->teams.at(teamID);
    }

    void World::doEggCleanup()
    {
        for (int i = 0; i < this->eggs.size(); i++)
        {
            const auto egg = this->eggs.at(i);
            if (!egg->isDead())
                continue;
            EventSystem::trigger("player_egg_died", this->graphicalClients, this->_zappyServer.getConfig(), *this,
                                 egg->ID);
            this->getTileAt(static_cast<int>(egg->getX()), static_cast<int>(egg->getY()))
                .removeEgg(egg);
            this->eggs.erase(this->eggs.begin() + i);
        }
    }

    void World::doPlayerCleanup()
    {
        for (int i = static_cast<int>(this->players.size()) - 1; i >= 0; --i)
        {
            const auto player = this->players.at(i);
            if (!player->isDead())
                continue;
            try
            {
                this->getMainZappyServer().sendMessageToClient("dead", player->ID);
            }
            catch (std::runtime_error&) {}
            EventSystem::trigger("player_died", this->graphicalClients, this->_zappyServer.getConfig(), *this,
                                 player->ID);
            this->getMainZappyServer().markClientAsDead(player->ID);
            this->getTileAt(static_cast<int>(player->getX()), static_cast<int>(player->getY()))
                .removePlayer(player);
            this->players.erase(this->players.begin() + i);
        }
    }

    void World::doGraphicalCleanup()
    {
        for (int i = static_cast<int>(this->graphicalClients.size()) - 1; i >= 0; --i)
        {
            const auto graphical = this->graphicalClients.at(i);
            if (!graphical->isDead())
                continue;
            this->getMainZappyServer().markClientAsDead(graphical->getID());
            this->graphicalClients.erase(this->graphicalClients.begin() + i);
        }
    }

    void World::checkForWin() {
        std::map<int, int> level8PlayersByTeam;

        for (const auto& player : this->players) {
            if (player->getLevel() != 8)
                continue;
            if (level8PlayersByTeam.contains(player->getTeamId()))
                level8PlayersByTeam.at(player->getTeamId()) += 1;
            else
                level8PlayersByTeam.insert({player->getTeamId(), 1});
        }

        for (const auto& [teamID, level8Players] : level8PlayersByTeam) {
            if (this->teamWins.at(teamID) == true)
                continue;
            if (level8Players >= 6) {
                EventSystem::trigger("end_game", this->graphicalClients, this->_zappyServer.getConfig(), *this, getTeamName(teamID));
                    std::cout << debug::getTS() << BOLDRED << "[INFO] TEAM " << getTeamName(teamID)
                    << " (ID " << teamID << ") HAS " << YELLOW << "W" << GREEN
                    << "O" << BLUE << "N" << CYAN "!" << RESET << std::endl;
                this->teamWins.at(teamID) = true;
            }
        }
    }
}
