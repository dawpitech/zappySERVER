/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Player.cpp
*/

#include <iostream>

#include "Player.hpp"
#include "../../utils/Debug.hpp"

zappy::engine::entities::Player::Player(const unsigned int x, const unsigned int y, const unsigned int teamID,
                                        const unsigned int playerID, const unsigned int tickAtSpawn)
    : ID(playerID), _tickAtLastMeal(tickAtSpawn), _currentX(x), _currentY(y), _teamID(teamID),
      _currentLevel(1), _facing(Directions::NORTH), alive(true), _status(Status::WAITING_FOR_COMMAND),
      _waitingCyclesRemaining(0)
{
    switch (std::rand() % 4) // NOLINT(*-msc50-cpp)
    {
        case 0: this->_facing = Directions::NORTH; break;
        case 1: this->_facing = Directions::EAST; break;
        case 2: this->_facing = Directions::SOUTH; break;
        default:
        case 3: this->_facing = Directions::WEST; break;
    }

    this->_inventory.insert({Ressources::FOOD, 10});
    for (const auto rsc : {
             Ressources::LINEMATE, Ressources::DERAUMERE, Ressources::SIBUR, Ressources::MENDIANE, Ressources::PHIRAS,
             Ressources::THYSTAME
         })
        this->_inventory.insert({rsc, 0});
    std::cout << debug::getTS() << "[TRACE] PLAYER CREATED AT " << x << ":" << y << std::endl;
}

void zappy::engine::entities::Player::addCommandToBuffer(const std::string& command)
{
    if (this->_commandsBuffer.size() >= 10)
    {
        std::cout << debug::getTS() << "[TRACE] Command to player was skipped due to buffer being full" << std::endl;
        return;
    }
    this->_commandsBuffer.emplace(command);
}

std::queue<std::string>& zappy::engine::entities::Player::getCommandsBuffer()
{
    return this->_commandsBuffer;
}

void zappy::engine::entities::Player::removeRessource(const Ressources type, const int quantity)
{
    if (quantity > this->_inventory.at(type))
        throw std::runtime_error("Not enough ressources in player inventory to take action");
    this->_inventory.at(type) = this->_inventory.at(type) - quantity;
}

void zappy::engine::entities::Player::addRessource(const Ressources type, const int quantity)
{
    this->_inventory.at(type) = this->_inventory.at(type) + quantity;
}

void zappy::engine::entities::Player::setPosition(const int x, const int y)
{
    this->_currentX = x;
    this->_currentY = y;
}

void zappy::engine::entities::Player::eat(const unsigned int tick)
{
    if (this->_inventory.at(Ressources::FOOD) == 0)
    {
        std::cout << debug::getTS() << "[INFO] PLATER " << this->ID << " HAS DIED OF HUNGER" << std::endl;
        this->alive = false;
        return;
    }
    std::cout << debug::getTS() << "[TRACE] PLAYER " << this->ID << " EAT" << std::endl;
    this->_inventory.at(Ressources::FOOD) -= 1;
    this->_tickAtLastMeal = tick;
}
