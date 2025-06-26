#include "Graphical.hpp"
#include "../../ZappyServer.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <queue>

unsigned int zappy::engine::GraphicalClient::getID() {
    return this->_ID;
}

void zappy::engine::GraphicalClient::setID(unsigned int id) {
    this->_ID = id;
}

void zappy::engine::GraphicalClient::addCommandToBuffer(const std::string& command)
{
    if (this->_commandsBuffer.size() >= 10) {
        std::cout << "[TRACE] Command to player was skipped due to buffer being full" << std::endl;
        return;
    }
    this->_commandsBuffer.emplace(command);
}

std::queue<std::string>& zappy::engine::GraphicalClient::getCommandsBuffer()
{
    return this->_commandsBuffer;
}

void zappy::engine::GraphicalClient::sendGreetings(const zappy::utils::ZappyConfig &config, zappy::engine::World &world, const std::string& args) {
    this->sendMsz(*this, config, world, args);
    this->sendSgt(*this, config, world, args);
    this->sendTna(*this, config, world, args);
}

//map size
void zappy::engine::GraphicalClient::sendMsz(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, zappy::engine::World &world, const std::string& args) {
    std::string com = "msz ";

    com += std::to_string(config.worldWidth);
    com += " ";
    com += std::to_string(config.worldHeight);
    std::cout << "[TRACE][GRAPHIC] sending msz message to CLIENT : " << graphic.getID() << std::endl;

    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}

//frequence valu
void zappy::engine::GraphicalClient::sendSgt(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args) {
    std::string com = "sgt " + std::to_string((int)(config.freqValue));

    std::cout << "[TRACE][GRAPHIC] sending sgt message to CLIENT : " << graphic.getID() << std::endl;
    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}

//name of the teams
void zappy::engine::GraphicalClient::sendTna(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args) {
    for (auto e : config.teamNames) {
        std::string com = "tna " + e;

        std::cout << "[TRACE][GRAPHIC] sending tna message to CLIENT : " << graphic.getID() << std::endl;
	world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
    }
}

//TODO
// content of the map
/*
void zappy::engine::GraphicalClient::sendBct(unsigned int fd, const
zappy::utils::ZappyConfig &config) {
}

//content of a tile
void zappy::engine::GraphicalClient::sendMct(unsigned int fd, const
zappy::utils::ZappyConfig &config) {
}
*/
