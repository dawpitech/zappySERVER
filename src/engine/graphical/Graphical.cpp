#include "Graphical.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

void zappy::engine::GraphicalClient::sendInitData(unsigned int fd, const zappy::utils::ZappyConfig &config) {
    this->sendMsz(fd, config); 
    this->sendSgt(fd, config);
    this->sendTna(fd, config);
}

//map size
void zappy::engine::GraphicalClient::sendMsz(unsigned int fd, const zappy::utils::ZappyConfig &config) {
    std::string com = "msz ";

    com += std::to_string(config.worldWidth);
    com += " ";
    com += std::to_string(config.worldHeight);
    com += "\n";
    std::cout << "[TRACE][GRAPHIC] sending msz message to FD : " << fd << std::endl;
    write(fd, com.c_str(), com.length());
}

//frequence value
void zappy::engine::GraphicalClient::sendSgt(unsigned int fd, const zappy::utils::ZappyConfig &config) {
    std::string com = "sgt " + std::to_string((int)(config.freqValue)) + "\n";

    std::cout << "[TRACE][GRAPHIC] sending sgt message to FD : " << fd << std::endl;
    write(fd, com.c_str(), com.length());
}

//name of the teams
void zappy::engine::GraphicalClient::sendTna(unsigned int fd, const zappy::utils::ZappyConfig &config) {
    for (auto e : config.teamNames) {
	std::string com = "tna " + e + "\n";

	std::cout << "[TRACE][GRAPHIC] sending tna message to FD : " << fd << std::endl;
	write(fd, com.c_str(), com.length());
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
