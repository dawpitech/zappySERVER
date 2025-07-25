/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdInventory.cpp
*/

#include "CmdInventory.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../entities/Player.hpp"

namespace zappy::engine::cmd
{
    void CmdInventory::cmdInventory(std::weak_ptr<entities::Player> player, World& world, [[maybe_unused]] const std::string& args)
    {
        const auto lockPlayer = player.lock();
        std::string reply = "[ ";
        bool first = true;

        for (const auto& [type, quantity] : lockPlayer->getInventory())
        {
            if (!first)
                reply.append(", ");
            reply.append(getRessourceName(type) + " " + std::to_string(quantity));
            first = false;
        }
        reply.append(" ]");
        world.getMainZappyServer().sendMessageToClient(reply, lockPlayer->ID);
    }
}
