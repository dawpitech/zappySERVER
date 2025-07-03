/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdEject.cpp
*/

#include "CmdEject.hpp"

#include <iostream>

#include "../../Player.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"
#include "../../../utils/EventRayDirectionInterpreter.hpp"

void zappy::engine::cmd::CmdEject::cmdEject(std::weak_ptr<Player> player, World& world, const std::string& args)
{
    const auto pusher = player.lock();
    const auto& tile = world.getTileAt(pusher->getX(), pusher->getY());

    for (const auto egg : tile.getEggs())
        egg->crush();

    int dx = 0;
    int dy = 0;
    switch (pusher->getDirection()) {
        case Directions::NORTH: dx = 0; dy = -1; break;
        case Directions::EAST: dx = 1; dy = 0; break;
        case Directions::SOUTH: dx = 0; dy = 1; break;
        case Directions::WEST: dx = -1; dy = 0; break;
    }

    for (const auto victim : tile.getPlayers()) {
        if (victim->ID == pusher->ID)
            continue;
        const auto& [newX, newY] = world.normalizeCoordinates(victim->getX() + dx, victim->getY() + dy);
        victim->setPosition(newX, newY);
        world.getMainZappyServer().sendMessageToClient("eject: " +
            std::to_string(utils::getRelativeDirection(
                victim->getX(),
                victim->getY(),
                victim->getDirection(),
                pusher->getX(),
                pusher->getY(),
                world.getWidth(),
                world.getHeight(),
                false)),
            victim->ID);
        std::cout << debug::getTS() << "[TRACE] PLAYER " << pusher->ID << " PUSH PLAYER "
            << victim->ID << " TO CELL " << victim->getX() << ":" << victim->getY() << std::endl;
    }

    for (const auto egg : tile.getEggs()) {
        egg->crush();
        std::cout << debug::getTS() << "[TRACE] PLAYER " << pusher->ID <<
            " CRUSHED EGG " << egg->ID << " OF TEAM " << world.getTeamName(egg->getTeamID()) << std::endl;
    }

    world.getMainZappyServer().sendMessageToClient("ok", pusher->ID);
}
