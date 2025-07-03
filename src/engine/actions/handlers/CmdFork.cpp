/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdFork.cpp
*/

#include "CmdFork.hpp"

#include <iostream>

#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"
#include "../../../utils/EventSystem.hpp"

void zappy::engine::cmd::CmdFork::cmdFork(std::weak_ptr<entities::Player> player, World& world, const std::string& args)
{
    const auto lockPlayer = player.lock();

    std::cout << debug::getTS() << "[INFO] PLAYER " << lockPlayer->ID << " LAID AN EGG" << std::endl;

    const auto egg = world.addPlayerEgg(lockPlayer->getTeamId(), lockPlayer->ID);
    world.getTileAt(static_cast<int>(lockPlayer->getX()), static_cast<int>(lockPlayer->getY())).addEgg(egg);

    EventSystem::trigger("egg_layed", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world, egg);

    world.getMainZappyServer().sendMessageToClient("ok", lockPlayer->ID);
}

bool zappy::engine::cmd::CmdFork::cmdPreFork(std::weak_ptr<entities::Player> player, World& world,
                                             const std::string& args)
{
    EventSystem::trigger("pre_fork", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world,
                         player.lock()->ID);
    return true;
}
