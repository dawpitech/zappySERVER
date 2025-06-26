/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Player.cpp
*/

#include "Player.hpp"

#include <iostream>

zappy::engine::Player::Player(const unsigned int x, const unsigned int y, const unsigned int teamID, const unsigned int playerID)
    : ID(playerID), _currentX(x), _currentY(y), _teamID(teamID), _currentLevel(0),
      _facing(Directions::NORTH), _status(Status::WAITING_FOR_COMMAND), _waitingCyclesRemaining(0)
{
    this->_inventory.insert({Ressources::FOOD, 10});
    this->_inventory.insert({Ressources::LINEMATE, 0});
    this->_inventory.insert({Ressources::DERAUMERE, 0});
    this->_inventory.insert({Ressources::SIBUR, 0});
    this->_inventory.insert({Ressources::MENDIANE, 0});
    this->_inventory.insert({Ressources::PHIRAS, 0});
    this->_inventory.insert({Ressources::THYSTAME, 0});
    std::cout << "PLAYER CREATED AT " << x << ":" << y << std::endl;
}

void zappy::engine::Player::addCommandToBuffer(const std::string& command)
{
    if (this->_commandsBuffer.size() >= 10) {
        std::cout << "[TRACE] Command to player was skipped due to buffer being full" << std::endl;
        return;
    }
    this->_commandsBuffer.emplace(command);
}

std::queue<std::string>& zappy::engine::Player::getCommandsBuffer()
{
    return this->_commandsBuffer;
}
