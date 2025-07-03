/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdBroadcast.cpp
*/

#include "CmdBroadcast.hpp"

#include <iostream>

#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"
#include "../../../utils/EventRayDirectionInterpreter.hpp"
#include "../../../utils/EventSystem.hpp"
#include "../../entities/Player.hpp"

void zappy::engine::cmd::CmdBroadcast::cmdBroadcast(std::weak_ptr<entities::Player> player, World& world, const std::string& args)
{
    const auto sender = player.lock();
    auto messageContent = args;

    if (const size_t pos = messageContent.find(' '); pos != std::string::npos) {
        messageContent.erase(0, pos + 1);
    } else {
        std::cout << debug::getTS() << "[TRACE] PLAYER " << sender->ID << " SENT BROADCAST COMMAND WITH INCORRECT SYNTAX" << std::endl;
        world.getMainZappyServer().sendMessageToClient("ko", sender->ID);
        return;
    }

    for (const auto target : world.getPlayers()) {
        if (target->ID == sender->ID)
            continue;
        world.getMainZappyServer().sendMessageToClient("message " +
            std::to_string(utils::getRelativeDirection(
                target->getX(),
                target->getY(),
                target->getDirection(),
                sender->getX(),
                sender->getY(),
                world.getWidth(),
                world.getHeight(),
                true)) + ", " + messageContent,
            target->ID);
    }

    std::cout << debug::getTS() << "[TRACE] PLAYER " << sender->ID << " SENT A BROADCAST" << std::endl;
    EventSystem::trigger("player_broadcast", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world, sender->ID, messageContent);
    world.getMainZappyServer().sendMessageToClient("ok", sender->ID);
}
