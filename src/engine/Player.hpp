/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Player.hpp
*/

#pragma once

#include <map>
#include <queue>
#include <string>

#include "Directions.hpp"
#include "Ressources.hpp"

namespace zappy::engine
{
    class Player
    {
        public:
            enum class Status
            {
                EGG,
                ALIVE,
                DEAD
            };

            Player(unsigned int x, unsigned int y, unsigned int teamID);
            ~Player() = default;

            void addCommandToBuffer(const std::string& command);

        private:
            unsigned int _currentX;
            unsigned int _currentY;
            unsigned int _teamID;
            unsigned int _currentLevel;
            std::map<int, Ressources> _inventory;
            Directions _facing;
            std::queue<std::string> _commandsBuffer;
    };
}
