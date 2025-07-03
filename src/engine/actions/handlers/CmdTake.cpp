/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdTake.cpp
*/

#include "CmdTake.hpp"

#include <iostream>

#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"
#include "../../../utils/EventSystem.hpp"

void zappy::engine::cmd::CmdTake::cmdTake(std::weak_ptr<Player> player, World& world, const std::string& args)
{
    const auto lockPlayer = player.lock();
    std::stringstream ss(args);
    std::string firstWorld;
    std::string secondWord;

    ss >> firstWorld >> secondWord;
    if (secondWord.empty()) {
        world.getMainZappyServer().sendMessageToClient("ko", lockPlayer->ID);
        std::cout << debug::getTS() << "[TRACE] PLAYER " << lockPlayer->ID << " SENT TAKE COMMAND WITH INCORRECT SYNTAX" << std::endl;
        return;
    }
    const auto type = getRessourceFromName(secondWord);
    if (type == std::nullopt) {
        world.getMainZappyServer().sendMessageToClient("ko", lockPlayer->ID);
        std::cout << debug::getTS() << "[TRACE] PLAYER " << lockPlayer->ID << " SENT TAKE COMMAND WITH UNKNOWN TYPE" << std::endl;
        return;
    }
    const auto safeType = type.value();
    auto& tile = world.getTileAt(lockPlayer->getX(), lockPlayer->getY());
    if (tile.getResourceQuantity(safeType) == 0) {
        world.getMainZappyServer().sendMessageToClient("ko", lockPlayer->ID);
        std::cout << debug::getTS() << "[TRACE] PLAYER " << lockPlayer->ID << " TRY TO TAKE FROM A CELL WITHOUT SPECIFIED RESSOURCE" << std::endl;
        return;
    }
    tile.removeResource(safeType);
    lockPlayer->addRessource(safeType, 1);

    std::cout << debug::getTS() << "[TRACE] PLAYER " << lockPlayer->ID << " TOOK " <<
        getRessourceName(safeType) << " FROM " << lockPlayer->getX() << ":"
        << lockPlayer->getY() << std::endl;

    EventSystem::trigger("player_take", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world, (unsigned int)safeType, lockPlayer->ID);
    world.getMainZappyServer().sendMessageToClient("ok", lockPlayer->ID);
}
