#include "Graphical.hpp"
#include "../../ZappyServer.hpp"
#include "../../utils/Debug.hpp"
#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <queue>
#include <sstream>
#include <memory>
#include <algorithm>
#include <vector>

unsigned int zappy::engine::GraphicalClient::getID() {
    return this->_ID;
}

void zappy::engine::GraphicalClient::setID(unsigned int id) {
    this->_ID = id;
}

void zappy::engine::GraphicalClient::addCommandToBuffer(const std::string& command)
{
    if (this->_commandsBuffer.size() >= 10) {
        std::cout << debug::getTS() << "[TRACE] Command to player was skipped due to buffer being full" << std::endl;
        return;
    }
    this->_commandsBuffer.emplace(command);
}

std::queue<std::string>& zappy::engine::GraphicalClient::getCommandsBuffer()
{
    return this->_commandsBuffer;
}

void zappy::engine::GraphicalClient::sendGreetings(zappy::utils::ZappyConfig &config, const zappy::engine::World &world, const std::string& args) {
    this->sendMsz(*this, config, world, args);
    this->sendSgt(*this, config, world, args);
    this->sendMct(*this, config, world, args);
    this->sendTna(*this, config, world, args);
    auto shareptr = this->shared_from_this();//std::shared_ptr<GraphicalClient>(this);
    std::vector<std::shared_ptr<GraphicalClient>> vec = {shareptr};
    this->sendPnw(vec, config, world, true);
    for (auto player: world.getPlayers()) {
        this->sendPin(*this, config, world, std::to_string(player->ID));
        this->sendPlv(*this, config, world, std::to_string(player->ID));
    }
    this->sendEnw(*this, config, world);
}

//bad command
void zappy::engine::GraphicalClient::sendSuc(unsigned int graphic_id, const zappy::engine::World &world) {
    std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending suc message to CLIENT : " << graphic_id << std::endl;
    world.getMainZappyServer().sendMessageToClient("suc", graphic_id);
}

//map size
void zappy::engine::GraphicalClient::sendMsz(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, const zappy::engine::World &world, const std::string& args) {
    std::string com = "msz ";

    com += std::to_string(config.worldWidth);
    com += " ";
    com += std::to_string(config.worldHeight);
    std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending msz message to CLIENT : " << graphic.getID() << std::endl;

    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}

//frequence valu
void zappy::engine::GraphicalClient::sendSgt(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, const World &world, const std::string& args) {
    std::string com = "sgt " + std::to_string((int)(config.freqValue));

    std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending sgt message to CLIENT : " << graphic.getID() << std::endl;
    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}

//name of the teams
void zappy::engine::GraphicalClient::sendTna(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, const World &world, const std::string& args) {
    for (auto e : config.teamNames) {
        std::string com = "tna " + e;

        std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending tna message to CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
    }
}

void zappy::engine::GraphicalClient::sendMctProxy(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world) {
    for (auto graphic: graphics) {
        sendMct(*graphic, config, world, "");
    }
}

// content of the map
void zappy::engine::GraphicalClient::sendMct(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, const World &world, const std::string& args) {
    std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending mct command to CLIENT : " << graphic.getID() << std::endl;
    for (int x = 0; x < world.getWidth(); x++) {
        for (int y = 0; y < world.getHeight(); y++) {
            sendBct(graphic, config, world, std::to_string(x) + " " + std::to_string(y));
        }
    }
}

void zappy::engine::GraphicalClient::sendEnwProxy(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, std::weak_ptr<engine::entities::Egg> egg) {
    auto eggLock = egg.lock();
    for (auto graphic: graphics) {
	std::string cmd = "enw #" + std::to_string(eggLock->ID) + " #" + std::to_string(eggLock->getMotherID()) + " " + std::to_string(eggLock->getX()) + " " + std::to_string(eggLock->getY());

	std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending enw command to CLIENT : " << graphic->getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(cmd, graphic->getID());
    }
}

void zappy::engine::GraphicalClient::sendEnw(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, const World &world) {
    for (auto egg: world.getEggs()) {
	std::string cmd = "enw #" + std::to_string(egg->ID) + " #" + std::to_string(egg->getMotherID()) + " " + std::to_string(egg->getX()) + " " + std::to_string(egg->getY());

	std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending enw command to CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(cmd, graphic.getID());
    }
}

//content of a tile
void zappy::engine::GraphicalClient::sendBct(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, const World &world, const std::string& args) {
    std::istringstream iss(args);
    std::string com = "bct ";
    int x, y;

    std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending bct command to CLIENT : " << graphic.getID() << std::endl;
    if (!(iss >> x >> y) || y >= world.getHeight() || x >= world.getWidth()) {
        std::cout << debug::getTS() << "[TRACE][GRAPHIC] bad bct command from CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("sbp", graphic.getID());
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
            case Ressources::FOOD: q0 = r.second; break;
            case Ressources::LINEMATE: q1 = r.second; break;
            case Ressources::DERAUMERE: q2 = r.second; break;
            case Ressources::SIBUR: q3 = r.second; break;
            case Ressources::MENDIANE: q4 = r.second; break;
            case Ressources::PHIRAS: q5 = r.second; break;
            case Ressources::THYSTAME: q6 = r.second; break;
        }
    }
    com += std::to_string(q0) + " " + std::to_string(q1) + " " + std::to_string(q2) + " " + std::to_string(q3) + " " + std::to_string(q4) + " " + std::to_string(q5) + " " + std::to_string(q6);
    
    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}

void zappy::engine::GraphicalClient::sendPpoProxy(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, unsigned int pl_id) {
    for (auto graphic: graphics) {
        sendPpo(*graphic, config, world, std::to_string(pl_id));
    }
}

//player's position
void zappy::engine::GraphicalClient::sendPpo(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, const World &world, const std::string& args) {
    std::istringstream iss(args);
    std::string com = "ppo #";
    int n;
    
    std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending ppo command to CLIENT : " << graphic.getID() << std::endl;
    if (!(iss >> n) || n > world.getPlayers().size()) {
        std::cout << debug::getTS() << "[TRACE][GRAPHIC] bad ppo command from CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("sbp", graphic.getID());
        return;
    }

    std::shared_ptr<Player> player;
    try {
	player = world.getPlayer(n);
    } catch (...) {
        std::cout << debug::getTS() << "[TRACE][GRAPHIC][ppo] player was null, CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("sbp", graphic.getID());
        return;
    }

    com += std::to_string(n) + " " + std::to_string(player->getX()) + " " + std::to_string(player->getY()) + " " + std::to_string((int)player->getDirection() + 1);
    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}

//player level
void zappy::engine::GraphicalClient::sendPlv(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, const World &world, const std::string& args) {
    std::istringstream iss(args);
    std::string com = "plv #";
    int n;
    
    std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending plv command to CLIENT : " << graphic.getID() << std::endl;
    if (!(iss >> n) || n > world.getPlayers().size()) {
        std::cout << debug::getTS() << "[TRACE][GRAPHIC] bad plv command from CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("sbp", graphic.getID());
        return;
    }

    std::shared_ptr<Player> player;
    try {
	player = world.getPlayer(n);
    } catch (...) {
        std::cout << debug::getTS() << "[TRACE][GRAPHIC][plv] player was null, CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("sbp", graphic.getID());
        return;
    }

    com += std::to_string(n) + " " + std::to_string(player->getLevel());
    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}

void zappy::engine::GraphicalClient::sendPinProxy(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, unsigned int unused, unsigned int pl_id) {
    for (auto graphic: graphics) {
        sendPin(*graphic, config, world, std::to_string(pl_id));
    }
}

// player inventory
void zappy::engine::GraphicalClient::sendPin(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, const World &world, const std::string& args) {
    std::istringstream iss(args);
    std::string com = "pin #";
    int n;
    
    std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending pin command to CLIENT : " << graphic.getID() << std::endl;
    if (!(iss >> n) || n > world.getPlayers().size()) {
        std::cout << debug::getTS() << "[TRACE][GRAPHIC] bad pin command from CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("sbp", graphic.getID());
        return;
    }

    std::shared_ptr<Player> player;
    try {
	player = world.getPlayer(n);
    } catch (...) {
        std::cout << debug::getTS() << "[TRACE][GRAPHIC][pin] player was null, CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("sbp", graphic.getID());
        return;
    }

    com += std::to_string(n) + " " + std::to_string(player->getX()) + " " + std::to_string(player->getY()) + " ";

    int q0 = 0;
    int q1 = 0;
    int q2 = 0;
    int q3 = 0;
    int q4 = 0;
    int q5 = 0;
    int q6 = 0;
    for (auto o : player->getInventory()) {
        switch (o.first) {
            case Ressources::FOOD: q0 = o.second; break;
            case Ressources::LINEMATE: q1 = o.second; break;
            case Ressources::DERAUMERE: q2 = o.second; break;
            case Ressources::SIBUR: q3 = o.second; break;
            case Ressources::MENDIANE: q4 = o.second; break;
            case Ressources::PHIRAS: q5 = o.second; break;
            case Ressources::THYSTAME: q6 = o.second; break;
        }
    }
    com += std::to_string(q0) + " " + std::to_string(q1) + " " + std::to_string(q2) + " " + std::to_string(q3) + " " + std::to_string(q4) + " " + std::to_string(q5) + " " + std::to_string(q6);
    world.getMainZappyServer().sendMessageToClient(com, graphic.getID());
}

void zappy::engine::GraphicalClient::sendSst(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, const World &world, const std::string& args) {
    std::istringstream iss(args);
    std::string com = "sst ";
    int t;

    std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending sst command to CLIENT : " << graphic.getID() << std::endl;
    if (!(iss >> t)) {
        std::cout << debug::getTS() << "[TRACE][GRAPHIC] bad sst command from CLIENT : " << graphic.getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient("sbp", graphic.getID());
        return;
    }
    config.freqValue = (float)t;
}

void zappy::engine::GraphicalClient::sendPnw(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, bool firsttime) {
    if (world.getPlayers().empty()) return;
    std::vector<std::shared_ptr<Player>> last_player {world.getPlayers().back()};
    auto players = firsttime ? world.getPlayers() : last_player;

    std::string com = "pnw #";
    int n;

    for (auto graphic : graphics) {
        for (auto p : players) {
            std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending pnw command to CLIENT : " << graphic->getID() << std::endl;

            try {
                com += std::to_string(p->ID) + " " + std::to_string(p->getX()) + " " + std::to_string(p->getY()) + " " + std::to_string((int)p->getDirection() + 1) + " " + std::to_string(p->getLevel()) + " " + config.teamNames[p->getTeamId()];
            } catch (std::out_of_range &) {
                std::cout << debug::getTS() << "[ERROR][GRAPHIC] pnw command failed, CLIENT : " << graphic->getID() << std::endl;
                continue;
            }
            world.getMainZappyServer().sendMessageToClient(com, graphic->getID());
            com = "pnw #";
        }
    }
}

void zappy::engine::GraphicalClient::sendPnw_proxy(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world) {
    sendPnw(graphics, config, world, false);
}

void zappy::engine::GraphicalClient::sendPdr(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, unsigned int res_id, unsigned int pl_id) {
    std::string com = "pdr ";
    com += "#" + std::to_string(pl_id) + " " + std::to_string(res_id);

    for (auto graphic : graphics) {
        std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending pdr command to CLIENT : " << graphic->getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(com, graphic->getID());
    }
}

void zappy::engine::GraphicalClient::sendPex(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, unsigned int pl_id) {
    for (auto graphic : graphics) {
        std::string com = "pex #";
        com += std::to_string(pl_id);
        std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending pex command to CLIENT : " << graphic->getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(com, graphic->getID());
    }
}

void zappy::engine::GraphicalClient::sendPbc(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, unsigned int pl_id, std::string msg) {
    for (auto graphic: graphics) {
	std::string com = "pbc #" + std::to_string(pl_id) + " " + msg;
	std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending pbc command to CLIENT : " << graphic->getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(com, graphic->getID());
    }
}

void zappy::engine::GraphicalClient::sendPfk(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, unsigned int pl_id) {
    for (auto graphic: graphics) {
	std::string com = "pfk #" + std::to_string(pl_id);
	std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending pfk command to CLIENT : " << graphic->getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(com, graphic->getID());
    }
}

void zappy::engine::GraphicalClient::sendEbo(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, unsigned int egg_id) {
    for (auto graphic: graphics) {
	std::string com = "ebo #" + std::to_string(egg_id);
	std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending ebo command to CLIENT : " << graphic->getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(com, graphic->getID());
    }
}

void zappy::engine::GraphicalClient::sendEdi(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, unsigned int egg_id) {
    for (auto graphic: graphics) {
	std::string com = "edi #" + std::to_string(egg_id);
	std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending edi command to CLIENT : " << graphic->getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(com, graphic->getID());
    }
}

void zappy::engine::GraphicalClient::sendPdi(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, unsigned int pl_id) {
    for (auto graphic: graphics) {
	std::string com = "pdi #" + std::to_string(pl_id);
	std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending pdi command to CLIENT : " << graphic->getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(com, graphic->getID());
    }
}

void zappy::engine::GraphicalClient::sendSmg(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, std::string message) {
    for (auto graphic: graphics) {
	std::string com = "smg " + message;
	std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending smg command to CLIENT : " << graphic->getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(com, graphic->getID());
    }
}

void zappy::engine::GraphicalClient::sendPgt(const std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, unsigned int res_id, unsigned int pl_id) {
    std::string com = "pgt #" + std::to_string(pl_id) + " " + std::to_string(res_id);
    for (auto graphic: graphics) {
	std::cout << debug::getTS() << "[TRACE][GRAPHIC] sending pgt command to CLIENT : " << graphic->getID() << std::endl;
        world.getMainZappyServer().sendMessageToClient(com, graphic->getID());
    }
}
