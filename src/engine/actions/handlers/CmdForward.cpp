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
    void CmdForward::cmdForward(std::weak_ptr<Player> player, World& world, const std::string& args)
    {
        const auto lockPlayer = player.lock();

        const auto currentX = lockPlayer->getX();
        const auto currentY = lockPlayer->getY();

        world.getTileAt(currentX, currentY).removePlayer(lockPlayer);

        switch (lockPlayer->getDirection())
        {
            case Directions::NORTH:
                lockPlayer->setPosition(currentX, currentY + 1);
                break;
            case Directions::EAST:
                lockPlayer->setPosition(currentX + 1, currentY);
                break;
            case Directions::SOUTH:
                lockPlayer->setPosition(currentX, currentY - 1);
                break;
            case Directions::WEST:
                lockPlayer->setPosition(currentX - 1, currentY);
                break;
        }

        world.getTileAt(lockPlayer->getX(), lockPlayer->getY()).addPlayer(lockPlayer);

        std::cout << "[TRACE] Player " << lockPlayer->ID << " MOVED TO " << lockPlayer->getX() << ":" << lockPlayer->getY() << std::endl;
        world.getMainZappyServer().sendMessageToClient("ok", lockPlayer->ID);
    }
}
