/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdForward.cpp
*/

#include <iostream>

#include "CmdForward.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"
#include "../../../utils/EventSystem.hpp"
#include "../../entities/Player.hpp"

namespace zappy::engine::cmd
{
    void CmdForward::cmdForward(std::weak_ptr<entities::Player> player, World& world, [[maybe_unused]] const std::string& args)
    {
        const auto lockPlayer = player.lock();

        auto playerX = lockPlayer->getX();
        auto playerY = lockPlayer->getY();
        world.getTileAt(static_cast<int>(playerX), static_cast<int>(playerY)).removePlayer(lockPlayer);

        switch (lockPlayer->getDirection())
        {
            case Directions::NORTH: playerY -= 1;
                break;
            case Directions::EAST: playerX += 1;
                break;
            case Directions::SOUTH: playerY += 1;
                break;
            case Directions::WEST: playerX -= 1;
                break;
        }

        const auto [newX, newY] = world.normalizeCoordinates(static_cast<int>(playerX), static_cast<int>(playerY));
        lockPlayer->setPosition(newX, newY);
        world.getTileAt(static_cast<int>(lockPlayer->getX()), static_cast<int>(lockPlayer->getY())).addPlayer(lockPlayer);

        std::cout << debug::getTS() << "[TRACE] Player " << lockPlayer->ID << " MOVED TO " << lockPlayer->getX() << ":"
            << lockPlayer->getY() << std::endl;
        EventSystem::trigger("player_move", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world,
                             lockPlayer->ID);

        world.getMainZappyServer().sendMessageToClient("ok", lockPlayer->ID);
    }
}
