/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdForward.cpp
*/

#include <iostream>

#include "CmdForward.hpp"
#include "../../Player.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"

namespace zappy::engine::cmd
{
    void CmdForward::cmdForward(std::weak_ptr<Player> player, World& world, const std::string& args)
    {
        const auto lockPlayer = player.lock();

        auto playerX = lockPlayer->getX();
        auto playerY = lockPlayer->getY();
        world.getTileAt(playerX, playerY).removePlayer(lockPlayer);

        switch (lockPlayer->getDirection())
        {
            case Directions::NORTH: playerY -= 1; break;
            case Directions::EAST: playerX += 1; break;
            case Directions::SOUTH: playerY += 1; break;
            case Directions::WEST: playerX -= 1; break;
        }

        const auto [newX, newY] = world.normalizeCoordinates(playerX, playerY);
        lockPlayer->setPosition(newX, newY);
        world.getTileAt(lockPlayer->getX(), lockPlayer->getY()).addPlayer(lockPlayer);

        std::cout << "[TRACE] Player " << lockPlayer->ID << " MOVED TO " << lockPlayer->getX() << ":" << lockPlayer->getY() << std::endl;
        world.getMainZappyServer().sendMessageToClient("ok", lockPlayer->ID);
    }
}
