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

#include "../Directions.hpp"
#include "../Ressources.hpp"

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

            Player(unsigned int x, unsigned int y, unsigned int teamID, unsigned int playerID, unsigned int tickAtSpawn);
            ~Player() = default;

            void addCommandToBuffer(const std::string& command);
            std::queue<std::string>& getCommandsBuffer();

            [[nodiscard]] Status getStatus() const { return this->_status; }
            void setStatus(const Status playerStatus) { this->_status = playerStatus; }

            [[nodiscard]] unsigned int getWaitingCyclesRemaining() const { return this->_waitingCyclesRemaining; }
            void setWaitingCyclesRemaining(const unsigned int remainingCycles) { this->_waitingCyclesRemaining = remainingCycles; }

            [[nodiscard]] const std::map<Ressources, int>& getInventory() { return this->_inventory; }
            void removeRessource(Ressources type, int quantity);
            void addRessource(Ressources type, int quantity);

            [[nodiscard]] unsigned int getX() const { return this->_currentX; }
            [[nodiscard]] unsigned int getY() const { return this->_currentY; }
            [[nodiscard]] Directions getDirection() const { return this->_facing; }
            [[nodiscard]] unsigned int getLevel() const { return this->_currentLevel; }
            [[nodiscard]] unsigned int getTickAtLastMeal() const { return this->_tickAtLastMeal; }
            [[nodiscard]] unsigned int getTeamId() const { return this->_teamID; }

            void setPosition(int x, int y);
            void setDirection(const Directions direction) { this->_facing = direction; }

            void eat(unsigned int tick);

            [[nodiscard]] bool isDead() const { return !this->alive; }
            void markAsDead() { this->alive = false; }

            void upLevel() { this->_currentLevel++; }

            const unsigned int ID;

        private:
            unsigned int _tickAtLastMeal;
            unsigned int _currentX;
            unsigned int _currentY;
            unsigned int _teamID;
            unsigned int _currentLevel;
            std::map<Ressources, int> _inventory;
            Directions _facing;

            bool alive;

            Status _status;
            unsigned int _waitingCyclesRemaining;
            std::queue<std::string> _commandsBuffer;
    };
}
