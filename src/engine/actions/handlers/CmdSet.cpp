/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdSet.cpp
*/

#include "CmdSet.hpp"

#include <iostream>

#include "../../Player.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"
#include "../../../utils/EventSystem.hpp"

void zappy::engine::cmd::CmdSet::cmdSet(std::weak_ptr<Player> player, World& world, const std::string& args)
{
    const auto lockPlayer = player.lock();
    std::stringstream ss(args);
    std::string firstWorld;
    std::string secondWord;

    ss >> firstWorld >> secondWord;
    if (secondWord.empty()) {
        world.getMainZappyServer().sendMessageToClient("ko", lockPlayer->ID);
        std::cout << debug::getTS() << "[TRACE] PLAYER " << lockPlayer->ID << " SENT SET COMMAND WITH INCORRECT SYNTAX" << std::endl;
        return;
    }
    const auto type = getRessourceFromName(secondWord);
    if (type == std::nullopt) {
        world.getMainZappyServer().sendMessageToClient("ko", lockPlayer->ID);
        std::cout << debug::getTS() << "[TRACE] PLAYER " << lockPlayer->ID << " SENT SET COMMAND WITH UNKNOWN TYPE" << std::endl;
        return;
    }
    const auto safeType = type.value();
    if (lockPlayer->getInventory().at(safeType) == 0) {
        world.getMainZappyServer().sendMessageToClient("ko", lockPlayer->ID);
        std::cout << debug::getTS() << "[TRACE] PLAYER " << lockPlayer->ID <<
            " TRY TO SET ON A CELL WITHOUT HAVING SPECIFIED RESSOURCE IN INVENTORY" << std::endl;
        return;
    }
    lockPlayer->removeRessource(safeType, 1);
    world.getTileAt(lockPlayer->getX(), lockPlayer->getY()).addResource(safeType);

    std::cout << debug::getTS() << "[TRACE] PLAYER " << lockPlayer->ID << " SET " <<
        getRessourceName(safeType) << " DOWN AT " << lockPlayer->getX() << ":"
        << lockPlayer->getY() << std::endl;

    EventSystem::trigger("player_set", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world, (unsigned int)safeType, lockPlayer->ID);
    EventSystem::trigger("map_refill", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world);
    world.getMainZappyServer().sendMessageToClient("ok", lockPlayer->ID);
}
