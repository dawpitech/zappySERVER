/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdTake.hpp
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
    class CmdTake
    {
        public:
            static void cmdTake(std::weak_ptr<Player> player, World& world, const std::string& args);
    };
}
