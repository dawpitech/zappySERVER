/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** ZappyServer.cpp
*/

#include <iostream>

#include "ZappyServer.hpp"
#include "engine/graphical/Graphical.hpp"

zappy::ZappyServer::ZappyServer(const utils::ZappyConfig& config)
    : _config(config)
{
    this->_networkServer = std::make_unique<generic::NetworkServer>(config.listeningPort);
    this->_world = std::make_unique<engine::World>(config, *this);
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

    while (true) {
        this->_networkServer->pollNetworkActivity(*this);
        this->_world->tick();
    }
}

std::weak_ptr<zappy::engine::Player> zappy::ZappyServer::createNewPlayerInTeam(const std::string& teamName, const unsigned int clientID)
{
    const auto player = this->_world->addPlayer(teamName, clientID);
    this->sendMessageToClient("?", clientID);
    this->sendMessageToClient(std::to_string(this->_config.worldWidth) + " " + std::to_string(this->_config.worldHeight), clientID);
    return player;
}

std::weak_ptr<zappy::engine::GraphicalClient> zappy::ZappyServer::createNewGraphicalClient(unsigned int id) {
    const auto graphic = this->_world->addGraphicalClient();
    auto graphic_ = graphic.lock();
    graphic_->setID(id);
    graphic_->sendGreetings(this->getConfig(), *(this->_world.get()), "");
    return graphic;
}

void zappy::ZappyServer::sendMessageToClient(const std::string& message, const unsigned int clientID)
{
    this->_networkServer->writeToClient(message, clientID);
}

const zappy::utils::ZappyConfig& zappy::ZappyServer::getConfig() const
{
    return this->_config;
}

zappy::utils::ZappyConfig& zappy::ZappyServer::getConfig()
{
    return this->_config;
}
