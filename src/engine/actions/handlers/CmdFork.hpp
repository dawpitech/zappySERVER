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
    namespace entities {
        class Player;
    }

    class World;
}

namespace zappy::engine::cmd
{
    class CmdFork
    {
        public:
            static void cmdFork(std::weak_ptr<entities::Player> player, World& world, const std::string& args);
            static bool cmdPreFork(std::weak_ptr<entities::Player> player, World& world, const std::string& args);
    };
}
