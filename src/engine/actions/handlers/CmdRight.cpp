/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdRight.cpp
*/

#include <iostream>

#include "CmdRight.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"
#include "../../../utils/EventSystem.hpp"
#include "../../entities/Player.hpp"

namespace zappy::engine::cmd
{
    void CmdRight::cmdRight(std::weak_ptr<entities::Player> player, World& world, [[maybe_unused]] const std::string& args) // NOLINT(*-unnecessary-value-param)
    {
        const auto& lockPlayer = player.lock();

        switch (lockPlayer->getDirection())
        {
            case Directions::NORTH: lockPlayer->setDirection(Directions::EAST);
                break;
            case Directions::EAST: lockPlayer->setDirection(Directions::SOUTH);
                break;
            case Directions::SOUTH: lockPlayer->setDirection(Directions::WEST);
                break;
            case Directions::WEST: lockPlayer->setDirection(Directions::NORTH);
                break;
        }

        std::cout << debug::getTS() << "[TRACE] Player " << lockPlayer->ID << " NOW FACING " << directionToString(
            lockPlayer->getDirection()) << std::endl;
        EventSystem::trigger("player_move", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world,
                             lockPlayer->ID);
        world.getMainZappyServer().sendMessageToClient("ok", lockPlayer->ID);
    }
}
