/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Egg.hpp
*/

#pragma once

namespace zappy::engine::entities
{
    class Egg
    {
        public:
            explicit Egg(unsigned int x, unsigned int y, unsigned int teamID, unsigned int EggID);
            ~Egg() = default;

            unsigned int ID;

            unsigned int getTeamID() const { return this->_teamID; }
            unsigned int getX() const { return this->_x; }
            unsigned int getY() const { return this->_y; }

            void crush() { this->alive = false; }
            bool isDead() const { return !this->alive; }

        private:
            bool alive = true;
            unsigned int _x;
            unsigned int _y;
            unsigned int _teamID;
    };
}
