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

    while (true) {
        this->_networkServer->pollNetworkActivity(*this);
        this->_world->tick();
    }
}

std::weak_ptr<zappy::engine::Player> zappy::ZappyServer::createNewPlayerInTeam(const std::string& teamName, unsigned int clientID)
{
    return this->_world->addPlayer(teamName, clientID);
}

std::weak_ptr<zappy::engine::GraphicalClient> zappy::ZappyServer::createNewGraphicalClient() {
    return this->_world->addGraphicalClient();
}

void zappy::ZappyServer::sendMessageToClient(const std::string& message, const unsigned int clientID)
{
    this->_networkServer->writeToClient(message, clientID);
}

const zappy::utils::ZappyConfig& zappy::ZappyServer::getConfig()
{
    return this->_config;
}
