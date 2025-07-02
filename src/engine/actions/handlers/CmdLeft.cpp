/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdLeft.cpp
*/

#include <iostream>

#include "CmdLeft.hpp"
#include "../../Player.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"

namespace zappy::engine::cmd
{
    void CmdLeft::cmdLeft(std::weak_ptr<Player> player, World& world, const std::string& args)
    {
        const auto& lockPlayer = player.lock();

        switch (lockPlayer->getDirection())
        {
            case Directions::NORTH: lockPlayer->setDirection(Directions::WEST); break;
            case Directions::EAST: lockPlayer->setDirection(Directions::NORTH); break;
            case Directions::SOUTH: lockPlayer->setDirection(Directions::EAST); break;
            case Directions::WEST: lockPlayer->setDirection(Directions::SOUTH); break;
        }

        std::cout << debug::getTS() << "[TRACE] Player " << lockPlayer->ID << " NOW FACING " << directionToString(lockPlayer->getDirection()) << std::endl;
        world.getMainZappyServer().sendMessageToClient("ok", lockPlayer->ID);
    }
}
