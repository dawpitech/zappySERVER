/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdFork.hpp
*/

#pragma once

#include <memory>
#include <string>

namespace zappy::engine {
    class World;
}

namespace zappy::engine {
    class Player;
}

namespace zappy::engine::cmd
{
    class CmdFork
    {
        public:
            static void cmdFork(std::weak_ptr<Player> player, World& world, const std::string& args);
            static void cmdPreFork(std::weak_ptr<Player> player, World& world, const std::string& args);
    };
}
