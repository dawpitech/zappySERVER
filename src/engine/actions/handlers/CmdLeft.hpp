/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdLeft.hpp
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
    class CmdLeft
    {
        public:
            static void cmdLeft(std::weak_ptr<entities::Player> player, World& world, const std::string& args);
    };
}
