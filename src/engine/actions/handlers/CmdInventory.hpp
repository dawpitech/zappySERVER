/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdInventory.hpp
*/

#pragma once

#include <string>

namespace zappy::engine {
    class World;
}

namespace zappy::engine {
    class Player;
}

namespace zappy::engine::cmd
{
    class CmdInventory
    {
        public:
            static void cmdInventory(Player& player, World& world, const std::string& args);
    };
}
