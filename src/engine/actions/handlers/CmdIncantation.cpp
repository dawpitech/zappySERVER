/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CmdIncantation.cpp
*/

#include <memory>

#include "CmdIncantation.hpp"
#include "../../World.hpp"
#include "../../../ZappyServer.hpp"
#include "../../../utils/Debug.hpp"
#include "../../../utils/EventSystem.hpp"
#include "../../entities/Player.hpp"

const std::map<unsigned int, zappy::engine::cmd::CmdIncantation::LevelInfo>
zappy::engine::cmd::CmdIncantation::ELEVATION_101 = {
    {
        2, {
            1, {
                {Ressources::LINEMATE, 1}
            }
        }
    },
    {
        3, {
            2, {
                {Ressources::LINEMATE, 1},
                {Ressources::DERAUMERE, 1},
                {Ressources::SIBUR, 1}
            }
        }
    },
    {
        4, {
            2, {
                {Ressources::LINEMATE, 2},
                {Ressources::SIBUR, 1},
                {Ressources::PHIRAS, 2}
            }
        }
    },
    {
        5, {
            4, {
                {Ressources::LINEMATE, 1},
                {Ressources::DERAUMERE, 1},
                {Ressources::SIBUR, 2},
                {Ressources::PHIRAS, 1}
            }
        }
    },
    {
        6, {
            4, {
                {Ressources::LINEMATE, 1},
                {Ressources::DERAUMERE, 2},
                {Ressources::SIBUR, 1},
                {Ressources::MENDIANE, 3}
            }
        }
    },
    {
        7, {
            6, {
                {Ressources::LINEMATE, 1},
                {Ressources::DERAUMERE, 2},
                {Ressources::SIBUR, 3},
                {Ressources::PHIRAS, 1}
            }
        }
    },
    {
        8, {
            6, {
                {Ressources::LINEMATE, 2},
                {Ressources::DERAUMERE, 2},
                {Ressources::SIBUR, 2},
                {Ressources::MENDIANE, 2},
                {Ressources::PHIRAS, 2},
                {Ressources::THYSTAME, 1}
            }
        }
    }
};

void zappy::engine::cmd::CmdIncantation::cmdIncantation(std::weak_ptr<entities::Player> player, World& world,
                                                        [[maybe_unused]] const std::string& args)
{
    const auto lockPlayer = player.lock();
    const auto& [_, requiredRessources] = ELEVATION_101.at(lockPlayer->getLevel() + 1);
    auto& tile = world.getTileAt(static_cast<int>(lockPlayer->getX()), static_cast<int>(lockPlayer->getY()));

    if (!canIncantationBeDone(*player.lock(), world))
    {
        world.getMainZappyServer().sendMessageToClient("ko", lockPlayer->ID);
        EventSystem::trigger("end_incantation", world.getGraphicalClients(), world.getMainZappyServer().getConfig(),
                             world, player, 0);
        return;
    }
    for (const auto& [element, quantity] : requiredRessources)
        tile.removeResource(element, quantity);
    EventSystem::trigger("map_refill", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world);

    std::vector<std::weak_ptr<entities::Player>> helpers;
    helpers.push_back(lockPlayer);
    for (const auto possiblePlayer : tile.getPlayers())
        if (possiblePlayer->getLevel() == lockPlayer->getLevel() && possiblePlayer->ID != lockPlayer->ID)
            helpers.push_back(possiblePlayer);

    for (auto helper : helpers) {
        const auto helperLock = helper.lock();
        helperLock->upLevel();
        EventSystem::trigger("player_levelup", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world, helperLock->ID);
        world.getMainZappyServer().sendMessageToClient("Current level: " + std::to_string(helperLock->getLevel()),
                                               helperLock->ID);
    }
    std::weak_ptr<engine::entities::Player> helperLock = helpers[0].lock();
    EventSystem::trigger("end_incantation", world.getGraphicalClients(), world.getMainZappyServer().getConfig(), world, helperLock, 1);
}

bool zappy::engine::cmd::CmdIncantation::cmdPreIncantation(std::weak_ptr<entities::Player> player, World& world,
                                                           [[maybe_unused]] const std::string& args)
{
    const auto lockPlayer = player.lock();
    const auto& tile = world.getTileAt(static_cast<int>(lockPlayer->getX()), static_cast<int>(lockPlayer->getY()));
    std::vector<std::weak_ptr<entities::Player>> players;

    if (!canIncantationBeDone(*lockPlayer, world))
    {
        world.getMainZappyServer().sendMessageToClient("ko", lockPlayer->ID);
        return false;
    }

    for (const auto possiblePlayer : tile.getPlayers())
        if (possiblePlayer->getLevel() == lockPlayer->getLevel())
            players.push_back(possiblePlayer);

    EventSystem::trigger("start_incantation", world.getGraphicalClients(), world.getMainZappyServer().getConfig(),
                         world, players);

    std::vector<std::weak_ptr<entities::Player>> helpers;
    helpers.push_back(lockPlayer);
    for (const auto possiblePlayer : tile.getPlayers())
        if (possiblePlayer->getLevel() == lockPlayer->getLevel() && possiblePlayer->ID != lockPlayer->ID)
            helpers.push_back(possiblePlayer);

    for (auto helper : helpers)
        world.getMainZappyServer().sendMessageToClient("Elevation underway", helper.lock()->ID);

    return true;
}

bool zappy::engine::cmd::CmdIncantation::hasRequiredRessources(const std::map<Ressources, int>& present,
                                                               const std::map<Ressources, int>& required)
{
    for (const auto& [element, quantity] : required) // NOLINT(*-use-anyofallof)
    {
        if (!present.contains(element))
            return false;
        if (present.at(element) < quantity)
            return false;
    }
    return true;
}

bool zappy::engine::cmd::CmdIncantation::canIncantationBeDone(entities::Player& player, World& world)
{
    const auto& tile = world.getTileAt(static_cast<int>(player.getX()), static_cast<int>(player.getY()));
    int fittingPlayersOnTheTile = 0;

    if (player.getLevel() == 8)
        return false;

    for (const auto possiblePlayer : tile.getPlayers())
        if (possiblePlayer->getLevel() == player.getLevel())
            fittingPlayersOnTheTile++;

    // ReSharper disable once CppTooWideScopeInitStatement
    const auto& [numberOfRequiredPlayers, requiredRessources] = ELEVATION_101.at(player.getLevel() + 1);

    if (fittingPlayersOnTheTile < numberOfRequiredPlayers || !hasRequiredRessources(
        tile.getAllResources(), requiredRessources))
        return false;

    return true;
}
