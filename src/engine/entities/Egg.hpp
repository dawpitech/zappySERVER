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
            explicit Egg(unsigned int x, unsigned int y, unsigned int teamID, unsigned int EggID, int motherPlayerID);
            ~Egg() = default;

            unsigned int ID;

            [[nodiscard]] unsigned int getTeamID() const { return this->_teamID; }
            [[nodiscard]] unsigned int getX() const { return this->_x; }
            [[nodiscard]] unsigned int getY() const { return this->_y; }
            [[nodiscard]] int getMotherID() const { return this->motherPlayerID; }

            void crush() { this->alive = false; }
            [[nodiscard]] bool isDead() const { return !this->alive; }

        private:
            bool alive = true;
            const unsigned int _x;
            const unsigned int _y;
            const unsigned int _teamID;
            const int motherPlayerID;
    };
}
