/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdConNbr.cpp
*/

#include "CmdConNbr.hpp"

#include "../../Player.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"

void zappy::engine::cmd::CmdConNbr::cmdConNbr(std::weak_ptr<Player> player, World& world, const std::string& args)
{
    const auto lockPlayer = player.lock();

    world.getMainZappyServer().sendMessageToClient(std::to_string(world.getEggCount(lockPlayer->getTeamId())), lockPlayer->ID);
}
