/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdForward.hpp
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
    class CmdForward
    {
        public:
            static void cmdForward(Player& player, World& world, const std::string& args);
    };
}
