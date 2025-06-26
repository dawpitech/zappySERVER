/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdForward.hpp
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
    class CmdForward
    {
        public:
            static void cmdForward(std::weak_ptr<Player> player, World& world, const std::string& args);
    };
}
