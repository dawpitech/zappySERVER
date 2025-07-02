/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CommandInterpreter.cpp
*/

#include <iostream>

#include "CommandInterpreter.hpp"
#include "../graphical/Graphical.hpp"
#include "handlers/CmdConNbr.hpp"
#include "handlers/CmdEject.hpp"
#include "handlers/CmdFork.hpp"
#include "handlers/CmdForward.hpp"
#include "handlers/CmdInventory.hpp"
#include "handlers/CmdLeft.hpp"
#include "handlers/CmdLook.hpp"
#include "handlers/CmdRight.hpp"
#include "handlers/CmdSet.hpp"
#include "handlers/CmdTake.hpp"

namespace zappy::engine
{
    const std::map<std::string, CommandInterpreter::GraphicCommandInfo> CommandInterpreter::GRAPHIC_COMMANDS = {
        {"msz", {GraphicCommand::MSZ, &zappy::engine::GraphicalClient::sendMsz}},
        {"sgt", {GraphicCommand::SGT, &zappy::engine::GraphicalClient::sendSgt}},
        {"tna", {GraphicCommand::TNA, &zappy::engine::GraphicalClient::sendTna}},
        {"mct", {GraphicCommand::MCT, &zappy::engine::GraphicalClient::sendMct}},
        {"bct", {GraphicCommand::BCT, &zappy::engine::GraphicalClient::sendBct}},
        {"ppo", {GraphicCommand::PPO, &zappy::engine::GraphicalClient::sendPpo}},
        {"plv", {GraphicCommand::PLV, &zappy::engine::GraphicalClient::sendPlv}},
        {"pin", {GraphicCommand::PIN, &zappy::engine::GraphicalClient::sendPin}},
        {"sgt", {GraphicCommand::SGT, &zappy::engine::GraphicalClient::sendSgt}},
        {"sst", {GraphicCommand::SST, &zappy::engine::GraphicalClient::sendSst}},
    };

    const std::map<std::string, CommandInterpreter::CommandInfo> CommandInterpreter::COMMANDS =  {
        {"Forward", {Command::FORWARD, 7, &cmd::CmdForward::cmdForward, &CommandInterpreter::silenceDummyAction}},
        {"Right", {Command::RIGHT, 7, &cmd::CmdRight::cmdRight, &CommandInterpreter::silenceDummyAction}},
        {"Left", {Command::LEFT, 7, &cmd::CmdLeft::cmdLeft, &CommandInterpreter::silenceDummyAction}},
        {"Look", {Command::LOOK, 7, &cmd::CmdLook::cmdLook, &CommandInterpreter::silenceDummyAction}},
        {"Inventory", {Command::INVENTORY, 1, &cmd::CmdInventory::cmdInventory, &CommandInterpreter::silenceDummyAction}},
        {"Broadcast", {Command::BROADCAST, 7, &CommandInterpreter::dummyAction, &CommandInterpreter::silenceDummyAction}},
        {"Connect_nbr", {Command::CONNECT_NBR, 0, &cmd::CmdConNbr::cmdConNbr, &CommandInterpreter::silenceDummyAction}},
        {"Fork", {Command::FORK, 42, &cmd::CmdFork::cmdFork, &cmd::CmdFork::cmdPreFork}},
        {"Eject", {Command::EJECT, 7, &cmd::CmdEject::cmdEject, &CommandInterpreter::silenceDummyAction}},
        {"Take", {Command::TAKE, 7, &cmd::CmdTake::cmdTake, &CommandInterpreter::silenceDummyAction}},
        {"Set", {Command::SET, 7, &cmd::CmdSet::cmdSet, &CommandInterpreter::silenceDummyAction}},
        {"Incantation", {Command::INCANTATION, 300, &CommandInterpreter::dummyAction, &CommandInterpreter::silenceDummyAction}},
    };

    void CommandInterpreter::dummyAction(std::weak_ptr<Player> player, World& world, const std::string& args)
    {
        std::cout << "Dummy action due to request: " << args << std::endl;
    }

    void CommandInterpreter::silenceDummyAction([[maybe_unused]] std::weak_ptr<Player>
        player, [[maybe_unused]] World& world, [[maybe_unused]] const std::string& args)
    {
    }
}
