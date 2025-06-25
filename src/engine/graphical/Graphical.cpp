#include "Graphical.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

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

void zappy::engine::GraphicalClient::sendInitData(unsigned int fd, const zappy::utils::ZappyConfig &config) {
    this->sendMsz(fd, config); 
}
