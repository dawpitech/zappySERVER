/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdConNbr.cpp
*/

#include "CmdConNbr.hpp"

#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"
#include "../../entities/Player.hpp"

void zappy::engine::cmd::CmdConNbr::cmdConNbr(std::weak_ptr<entities::Player> player, World& world,
                                              [[maybe_unused]] const std::string& args)
{
    const auto lockPlayer = player.lock();

    world.getMainZappyServer().sendMessageToClient(std::to_string(world.getEggCount(lockPlayer->getTeamId())),
                                                   lockPlayer->ID);
}
