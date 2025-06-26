/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdForward.cpp
*/

#include "CmdForward.hpp"

#include <iostream>

#include "../../Player.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"

namespace zappy::engine::cmd
{
    void CmdForward::cmdForward(Player& player, World& world, const std::string& args)
    {
        const auto currentX = player.getX();
        const auto currentY = player.getY();

        switch (player.getDirection())
        {
            case Directions::NORTH:
                player.setPosition(currentX, currentY + 1);
                break;
            case Directions::EAST:
                player.setPosition(currentX + 1, currentY);
                break;
            case Directions::SOUTH:
                player.setPosition(currentX, currentY - 1);
                break;
            case Directions::WEST:
                player.setPosition(currentX - 1, currentY);
                break;
        }

        std::cout << "[TRACE] Player " << player.ID << " MOVED TO " << player.getX() << ":" << player.getY() << std::endl;
        world.getMainZappyServer().sendMessageToClient("ok", player.ID);
    }
}
