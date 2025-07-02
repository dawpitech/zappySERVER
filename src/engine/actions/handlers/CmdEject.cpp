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
#include "../../../utils/EventSystem.hpp"

const int dir_dx[4] = { 0, 1, 0, -1 };
const int dir_dy[4] = { -1, 0, 1, 0 };

namespace
{
    zappy::engine::Directions turn_right(zappy::engine::Directions d) { return static_cast<zappy::engine::Directions>((static_cast<int>(d) + 1) % 4); }
    zappy::engine::Directions turn_left(zappy::engine::Directions d)  { return static_cast<zappy::engine::Directions>((static_cast<int>(d) + 3) % 4); }
    zappy::engine::Directions turn_back(zappy::engine::Directions d)  { return static_cast<zappy::engine::Directions>((static_cast<int>(d) + 2) % 4); }

    int computeEjectFacingDirValue(int vx, int vy, zappy::engine::Directions vdir, int px, int py, const int map_width, const int map_height)
    {
        auto norm = [&](const int v, const int max) { return (v + max) % max; };
        vx = norm(vx, map_width);
        vy = norm(vy, map_height);
        px = norm(px, map_width);
        py = norm(py, map_height);

        int dx = px - vx;
        int dy = py - vy;
        if (dx > map_width/2) dx -= map_width;
        if (dx < -map_width/2) dx += map_width;
        if (dy > map_height/2) dy -= map_height;
        if (dy < -map_height/2) dy += map_height;

        const int offsets[4][2] = {
            { dir_dx[static_cast<int>(vdir)],     dir_dy[static_cast<int>(vdir)] },
            { dir_dx[static_cast<int>(turn_right(vdir))], dir_dy[static_cast<int>(turn_right(vdir))] },
            { dir_dx[static_cast<int>(turn_back(vdir))],  dir_dy[static_cast<int>(turn_back(vdir))] },
            { dir_dx[static_cast<int>(turn_left(vdir))],  dir_dy[static_cast<int>(turn_left(vdir))] }
        };

        for (int i = 0; i < 4; ++i) {
            if (dx == offsets[i][0] && dy == offsets[i][1]) {
                return 1 + 2*i;
            }
        }
        throw std::runtime_error("Couldn't compute eject facing value");
    }
}

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
            std::to_string(computeEjectFacingDirValue(
                victim->getX(),
                victim->getY(),
                victim->getDirection(),
                pusher->getX(),
                pusher->getY(),
                world.getWidth(),
                world.getHeight())),
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
