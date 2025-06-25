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
                WAITING_FOR_COMMAND,
                WAITING_BEFORE_EXECUTE,
            };

            Player(unsigned int x, unsigned int y, unsigned int teamID);
            ~Player() = default;

            void addCommandToBuffer(const std::string& command);
            std::queue<std::string>& getCommandsBuffer();

            unsigned int& getWaitingCyclesRemaining() { return this->waitingCyclesRemaining; }

            [[nodiscard]] Status getStatus() const { return this->_status; }
            void setStatus(const Status playerStatus) { this->_status = playerStatus; }

        private:
            unsigned int _currentX;
            unsigned int _currentY;
            unsigned int _teamID;
            unsigned int _currentLevel;
            std::map<int, Ressources> _inventory;
            Directions _facing;

            Status _status;
            unsigned int waitingCyclesRemaining;
            std::queue<std::string> _commandsBuffer;
    };
}
