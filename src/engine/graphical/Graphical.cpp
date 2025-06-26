#include "Graphical.hpp"
#include "../../ZappyServer.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>
#include <queue>
#include <sstream>
#include <algorithm>

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
    this->sendMct(*this, config, world, args);
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
void zappy::engine::GraphicalClient::sendMct(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args) {
    std::cout << "[TRACE][GRAPHIC] sending mct command to CLIENT : " << graphic.getID() << std::endl;
    for (int x = 0; x < world.getWidth(); x++) {
        for (int y = 0; y < world.getHeight(); y++) {
            sendBct(graphic, config, world, std::to_string(x) + " " + std::to_string(y));
        }
    }
}

//content of a tile
void zappy::engine::GraphicalClient::sendBct(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args) {
    std::istringstream iss(args);
    std::string com = "bct ";
    int x, y;

    std::cout << "[TRACE][GRAPHIC] sending bct command to CLIENT : " << graphic.getID() << std::endl;
    if (!(iss >> x >> y) || y >= world.getHeight() || x >= world.getWidth()) {
        std::cout << "[TRACE][GRAPHIC] bad bct command from CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("ko", graphic.getID());
        return;
    }

    com += std::to_string(x) + " " + std::to_string(y) + " ";
    int q0 = 0;
    int q1 = 0;
    int q2 = 0;
    int q3 = 0;
    int q4 = 0;
    int q5 = 0;
    int q6 = 0;
    for (auto r : world.getTileAt(x, y).getAllResources()) {
        switch (r.first) {
            case ResourceType::FOOD: q0 = r.second; break;
            case ResourceType::LINEMATE: q1 = r.second; break;
            case ResourceType::DERAUMERE: q2 = r.second; break;
            case ResourceType::SIBUR: q3 = r.second; break;
            case ResourceType::MENDIANE: q4 = r.second; break;
            case ResourceType::PHIRAS: q5 = r.second; break;
            case ResourceType::THYSTAME: q6 = r.second; break;
        }
    }
    com += std::to_string(q0) + " " + std::to_string(q1) + " " + std::to_string(q2) + " " + std::to_string(q3) + " " + std::to_string(q4) + " " + std::to_string(q5) + " " + std::to_string(q6);
    
    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}

//player's position
//untested
void zappy::engine::GraphicalClient::sendPpo(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args) {
    std::string args2 = args;
    args2.erase(std::remove(args2.begin(), args2.end(), '#'), args2.end());
    std::istringstream iss(args2);
    std::string com = "ppo ";
    int n;
    
    std::cout << "[TRACE][GRAPHIC] sending ppo command to CLIENT : " << graphic.getID() << std::endl;
    if (!(iss >> n) || n > world.getPlayers().size()) {
        std::cout << "[TRACE][GRAPHIC] bad ppo command from CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("ko", graphic.getID());
        return;
    }

    auto player = world.getPlayers()[n].get();
    if (player == nullptr) {
        std::cout << "[TRACE][GRAPHIC][ppo] player was null, CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("ko", graphic.getID());
        return;
    }

    com += std::to_string(n) + " " + std::to_string(player->getX()) + " " + std::to_string(player->getY()) + " " + std::to_string((int)player->getDirection() + 1);
    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}

void zappy::engine::GraphicalClient::sendPlv(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args) {
    std::string args2 = args;
    args2.erase(std::remove(args2.begin(), args2.end(), '#'), args2.end());
    std::istringstream iss(args2);
    std::string com = "plv ";
    int n;
    
    std::cout << "[TRACE][GRAPHIC] sending plv command to CLIENT : " << graphic.getID() << std::endl;
    if (!(iss >> n) || n > world.getPlayers().size()) {
        std::cout << "[TRACE][GRAPHIC] bad plv command from CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("ko", graphic.getID());
        return;
    }

    auto player = world.getPlayers()[n].get();
    if (player == nullptr) {
        std::cout << "[TRACE][GRAPHIC][plv] player was null, CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("ko", graphic.getID());
        return;
    }

    com += std::to_string(n) + " " + std::to_string(player->getLevel());
    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}
