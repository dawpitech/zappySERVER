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

void zappy::engine::cmd::CmdFork::cmdFork(std::weak_ptr<Player> player, World& world, const std::string& args)
{
    const auto lockPlayer = player.lock();

    world.addPlayerEgg(lockPlayer->getTeamId());
    std::cout << debug::getTS() << "[INFO] PLAYER " << lockPlayer->ID << " LAID AN EGG" << std::endl;

    world.getMainZappyServer().sendMessageToClient("ok", lockPlayer->ID);
}
