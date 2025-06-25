/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Player.cpp
*/

#include "Player.hpp"

#include <iostream>

zappy::engine::Player::Player(const unsigned int x, const unsigned int y, const unsigned int teamID)
    : _currentX(x), _currentY(y), _teamID(teamID), _currentLevel(0), _facing(Directions::NORTH)
{
    std::cout << "PLAYER CREATED AT " << x << ":" << y << std::endl;
}

void zappy::engine::Player::addCommandToBuffer(const std::string& command)
{
    if (this->_commandsBuffer.size() > 10) {
        std::cout << "[TRACE] Command to player was skipped due to buffer being full" << std::endl;
        return;
    }
    this->_commandsBuffer.emplace(command);
}
