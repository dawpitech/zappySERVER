/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Player.cpp
*/

#include <iostream>

#include "Player.hpp"
#include "../utils/Debug.hpp"

zappy::engine::Player::Player(const unsigned int x, const unsigned int y, const unsigned int teamID, const unsigned int playerID)
    : ID(playerID), _currentX(x), _currentY(y), _teamID(teamID), _currentLevel(0),
      _facing(Directions::NORTH), _status(Status::WAITING_FOR_COMMAND), _waitingCyclesRemaining(0)
{
    this->_inventory.insert({Ressources::FOOD, 10});
    for (const auto rsc : {Ressources::LINEMATE, Ressources::DERAUMERE, Ressources::SIBUR, Ressources::MENDIANE, Ressources::PHIRAS, Ressources::THYSTAME})
        this->_inventory.insert({rsc, 0});
    std::cout << debug::getTS() << "PLAYER CREATED AT " << x << ":" << y << std::endl;
}

void zappy::engine::Player::addCommandToBuffer(const std::string& command)
{
    if (this->_commandsBuffer.size() >= 10) {
        std::cout << debug::getTS() << "[TRACE] Command to player was skipped due to buffer being full" << std::endl;
        return;
    }
    this->_commandsBuffer.emplace(command);
}

std::queue<std::string>& zappy::engine::Player::getCommandsBuffer()
{
    return this->_commandsBuffer;
}

void zappy::engine::Player::removeRessource(const Ressources type, const int quantity)
{
    if (quantity > this->_inventory.at(type))
        throw std::runtime_error("Not enough ressources in player inventory to take action");
    this->_inventory.at(type) = this->_inventory.at(type) - quantity;
}

void zappy::engine::Player::addRessource(const Ressources type, const int quantity)
{
    this->_inventory.at(type) = this->_inventory.at(type) + quantity;
}

void zappy::engine::Player::setPosition(const int x, const int y)
{
    this->_currentX = x;
    this->_currentY = y;

}
