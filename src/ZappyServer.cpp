/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** ZappyServer.cpp
*/

#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "ZappyServer.hpp"
#include "engine/entities/Egg.hpp"
#include "engine/entities/Player.hpp"
#include "engine/graphical/Graphical.hpp"
#include "utils/Debug.hpp"
#include "utils/EventSystem.hpp"

namespace
{
    std::atomic_flag shouldRun = true;

    void signalHandler([[maybe_unused]] int signum) {
        shouldRun.clear();
    }
}

zappy::ZappyServer::ZappyServer(const utils::ZappyConfig& config)
    : _config(config), _tickDuration(static_cast<int>(1000 / config.freqValue))
{
    this->_networkServer = std::make_unique<generic::NetworkServer>(config.listeningPort);
    this->_world = std::make_unique<engine::World>(config, *this);

    signal(SIGINT, signalHandler);


    EventSystem::subscribe<std::weak_ptr<engine::entities::Player>, int>("end_incantation", std::function(engine::GraphicalClient::sendPie));
    EventSystem::subscribe<std::vector<std::weak_ptr<engine::entities::Player>>>("start_incantation", std::function(engine::GraphicalClient::sendPic));
    EventSystem::subscribe("player_spawn", std::function(engine::GraphicalClient::sendPnw_proxy));
    EventSystem::subscribe<unsigned int>("player_egg_spawn", std::function(engine::GraphicalClient::sendEbo));
    EventSystem::subscribe<unsigned int>("player_egg_died", std::function(engine::GraphicalClient::sendEdi));
    EventSystem::subscribe<unsigned int>("player_died", std::function(engine::GraphicalClient::sendPdi));
    EventSystem::subscribe<unsigned int, unsigned int>("player_set", std::function(engine::GraphicalClient::sendPdr));
    EventSystem::subscribe<unsigned int, unsigned int>("player_set", std::function(engine::GraphicalClient::sendPinProxy));
    EventSystem::subscribe<unsigned int, unsigned int>("player_take", std::function(engine::GraphicalClient::sendPinProxy));
    EventSystem::subscribe<unsigned int, unsigned int>("player_take", std::function(engine::GraphicalClient::sendPgt));
    EventSystem::subscribe<unsigned int, unsigned int>("player_eat", std::function(engine::GraphicalClient::sendPinProxy));
    EventSystem::subscribe<unsigned int>("player_move", std::function(engine::GraphicalClient::sendPpoProxy));
    EventSystem::subscribe<unsigned int>("player_eject", std::function(engine::GraphicalClient::sendPpoProxy));
    EventSystem::subscribe<unsigned int>("player_eject", std::function(engine::GraphicalClient::sendPex));
    EventSystem::subscribe<unsigned int, std::string>("player_broadcast", std::function(engine::GraphicalClient::sendPbc));
    EventSystem::subscribe<unsigned int>("pre_fork", std::function(engine::GraphicalClient::sendPfk));
    EventSystem::subscribe("map_refill", std::function(engine::GraphicalClient::sendMctProxy));
    EventSystem::subscribe<std::weak_ptr<engine::entities::Egg>>("egg_layed", std::function(engine::GraphicalClient::sendEnwProxy));
}

void zappy::ZappyServer::launch()
{
    std::cout << "=== Zappy server ===" << std::endl;
    std::cout << "Port:\t\t" << this->_config.listeningPort << std::endl;
    std::cout << "Width:\t\t" << this->_config.worldWidth << std::endl;
    std::cout << "Height:\t\t" << this->_config.worldHeight << std::endl;
    std::cout << "Team names:\t" << std::endl;
    for (const auto& name : this->_config.teamNames)
        std::cout << "   " << name << std::endl;
    std::cout << "Team size:\t" << this->_config.initialTeamSize << std::endl;
    std::cout << "Freq:\t\t" << this->_config.freqValue << std::endl;
    std::cout << "=== Now running ===" << std::endl;

    _lastTickTime = std::chrono::steady_clock::now();

    while (shouldRun.test()) {
        this->_networkServer->pollNetworkActivity(*this, 5);

        const auto currentTime = std::chrono::steady_clock::now();
        // ReSharper disable once CppTooWideScopeInitStatement
        const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _lastTickTime);

        if (elapsedTime >= _tickDuration) {
            this->_world->tick();
            _lastTickTime = currentTime;

            if (elapsedTime > _tickDuration * 1.5)
                std::cout << debug::getTS() << "[WARN] Server running slow, elapsed: " << elapsedTime.count() << "ms, expected: " << _tickDuration.count() << "ms" << std::endl;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    std::cout << "Zappy server exiting..." << std::endl;
}

std::weak_ptr<zappy::engine::entities::Player> zappy::ZappyServer::createNewPlayerInTeam(const std::string& teamName, const unsigned int clientID) const
{
    const auto player = this->_world->connectPlayer(teamName, clientID);
    {
        const auto lockPlayer = player.lock();
        this->_world->getTileAt(static_cast<int>(lockPlayer->getX()), static_cast<int>(lockPlayer->getY())).addPlayer(lockPlayer);
    }
    this->sendMessageToClient(std::to_string(this->_world->getEggCount(teamName)), clientID);
    this->sendMessageToClient(std::to_string(this->_config.worldWidth) + " " + std::to_string(this->_config.worldHeight), clientID);
    return player;
}

std::weak_ptr<zappy::engine::GraphicalClient> zappy::ZappyServer::createNewGraphicalClient(const unsigned int id) {
    const auto graphic = this->_world->addGraphicalClient();
    const auto graphic_ = graphic.lock();
    graphic_->setID(id);
    graphic_->sendGreetings(this->getConfig(), *this->_world, "");
    return graphic;
}

void zappy::ZappyServer::sendMessageToClient(const std::string& message, const unsigned int clientID) const
{
    this->_networkServer->writeToClient(message, clientID);
}

void zappy::ZappyServer::markClientAsDead(const unsigned int clientID) const
{
    this->_networkServer->markConnectionAsDead(clientID);
}

const zappy::utils::ZappyConfig& zappy::ZappyServer::getConfig() const
{
    return this->_config;
}

zappy::utils::ZappyConfig& zappy::ZappyServer::getConfig()
{
    return this->_config;
}
