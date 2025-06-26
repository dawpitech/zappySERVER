/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdLook.cpp
*/

#include "CmdLook.hpp"

#include <iostream>

#include "../../Player.hpp"
#include "../../Tile.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"

void zappy::engine::cmd::CmdLook::cmdLook(std::weak_ptr<Player> player, World& world, const std::string& args)
{
    const auto lockPlayer = player.lock();
    std::string reply = "[ ";
    bool first = true;

    if (lockPlayer->getDirection() == Directions::NORTH) {
        for (int ix = 0; ix <= lockPlayer->getLevel() + 1; ix++) {
            for (int iy = -ix; iy <= ix; iy++) {
                const auto [tileX, tileY] = world.normalizeCoordinates(static_cast<int>(lockPlayer->getX() - ix), static_cast<int>(lockPlayer->getY() - iy));
                const auto& tile = world.getTileAt(tileX, tileY);

                if (!first)
                    reply.append(",");
                first = false;
                for (int i = 0; i < tile.getPlayerCount(); i++)
                    reply.append("player ");
                reply.append("(" + std::to_string(tileX) + ":" + std::to_string(tileY) + ")");
            }
        }
    } else if (lockPlayer->getDirection() == Directions::SOUTH) {
        for (int ix = 0; ix <= lockPlayer->getLevel() + 1; ix++) {
            for (int iy = -ix; iy <= ix; iy++) {
                const auto [tileX, tileY] = world.normalizeCoordinates(static_cast<int>(lockPlayer->getX() + ix), static_cast<int>(lockPlayer->getY() + iy));
                const auto& tile = world.getTileAt(tileX, tileY);

                if (!first)
                    reply.append(",");
                first = false;
                for (int i = 0; i < tile.getPlayerCount(); i++)
                    reply.append("player ");
                reply.append("(" + std::to_string(tileX) + ":" + std::to_string(tileY) + ")");
            }
        }
    } else if (lockPlayer->getDirection() == Directions::EAST) {
        for (int dy = 0; dy <= lockPlayer->getLevel() + 1; dy++) {
            for (int dx = -dy; dx <= dy; dx++) {
                const auto [tileX, tileY] = world.normalizeCoordinates(static_cast<int>(lockPlayer->getX() + dx), static_cast<int>(lockPlayer->getY() + dy));
                const auto& tile = world.getTileAt(tileX, tileY);

                if (!first)
                    reply.append(",");
                first = false;
                for (int i = 0; i < tile.getPlayerCount(); i++)
                    reply.append("player ");
                reply.append("(" + std::to_string(tileX) + ":" + std::to_string(tileY) + ")");
            }
        }
    } else {
        for (int dy = 0; dy <= lockPlayer->getLevel() + 1; dy++) {
            for (int dx = -dy; dx <= dy; dx++) {
                const auto [tileX, tileY] = world.normalizeCoordinates(static_cast<int>(lockPlayer->getX() - dx), static_cast<int>(lockPlayer->getY() - dy));
                const auto& tile = world.getTileAt(tileX, tileY);

                if (!first)
                    reply.append(",");
                first = false;
                for (int i = 0; i < tile.getPlayerCount(); i++)
                    reply.append("player ");
                reply.append("(" + std::to_string(tileX) + ":" + std::to_string(tileY) + ")");
            }
        }
    }
    reply.append(" ]");
    world.getMainZappyServer().sendMessageToClient(reply, lockPlayer->ID);
}
