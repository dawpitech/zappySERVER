/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdConNbr.hpp
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
    class CmdConNbr
    {
        public:
            static void cmdConNbr(std::weak_ptr<Player> player, World& world, const std::string& args);
    };
}
