/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdLook.cpp
*/

#include "CmdLook.hpp"
#include "../../Tile.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../entities/Player.hpp"

void zappy::engine::cmd::CmdLook::cmdLook(std::weak_ptr<entities::Player> player, World& world, [[maybe_unused]] const std::string& args) // NOLINT(*-unnecessary-value-param)
{
    const auto lockPlayer = player.lock();
    std::string reply = "[ ";
    bool firstCell = true;

    const int playerX = static_cast<int>(lockPlayer->getX());
    const int playerY = static_cast<int>(lockPlayer->getY());
    const Directions direction = lockPlayer->getDirection();
    const int level = static_cast<int>(lockPlayer->getLevel());

    int mainAxisDelta = 0;
    int mainAxis = 0;
    bool invertOffset = false;

    switch (direction)
    {
        case Directions::NORTH:
            mainAxisDelta = -1;
            mainAxis = 1;
            invertOffset = false;
            break;
        case Directions::EAST:
            mainAxisDelta = 1;
            mainAxis = 0;
            invertOffset = false;
            break;
        case Directions::SOUTH:
            mainAxisDelta = 1;
            mainAxis = 1;
            invertOffset = true;
            break;
        case Directions::WEST:
            mainAxisDelta = -1;
            mainAxis = 0;
            invertOffset = true;
            break;
    }

    for (int l = 0; l <= level; l++)
    {
        for (int offset = -l; offset <= l; offset++)
        {
            bool firstElement = true;
            int tileX = playerX;
            int tileY = playerY;

            if (mainAxis == 0)
            {
                tileX += l * mainAxisDelta;
                tileY += invertOffset ? -offset : offset;
            }
            else
            {
                tileX += invertOffset ? -offset : offset;
                tileY += l * mainAxisDelta;
            }

            const auto [normalizedX, normalizedY] = world.normalizeCoordinates(tileX, tileY);
            const auto& tile = world.getTileAt(normalizedX, normalizedY);

            if (!firstCell)
                reply.append(",");
            firstCell = false;

            for (int i = 0; i < tile.getPlayerCount(); i++)
            {
                if (!firstElement)
                    reply.append(" ");
                firstElement = false;
                reply.append("player");
            }
            for (const auto [type, quantity] : tile.getAllResources())
            {
                for (int i = 0; i < quantity; i++)
                {
                    if (!firstElement)
                        reply.append(" ");
                    firstElement = false;
                    reply.append(getRessourceName(type));
                }
            }
            //DEBUG: used to print the coords of the looked cells
            //reply.append("(" + std::to_string(normalizedX) + ":" + std::to_string(normalizedY) + ")");
        }
    }

    reply.append(" ]");
    world.getMainZappyServer().sendMessageToClient(reply, lockPlayer->ID);
}
