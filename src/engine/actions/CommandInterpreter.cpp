/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CommandInterpreter.cpp
*/

#include <iostream>

#include "CommandInterpreter.hpp"
#include "../graphical/Graphical.hpp"
#include "handlers/CmdBroadcast.hpp"
#include "handlers/CmdConNbr.hpp"
#include "handlers/CmdEject.hpp"
#include "handlers/CmdFork.hpp"
#include "handlers/CmdForward.hpp"
#include "handlers/CmdIncantation.hpp"
#include "handlers/CmdInventory.hpp"
#include "handlers/CmdLeft.hpp"
#include "handlers/CmdLook.hpp"
#include "handlers/CmdRight.hpp"
#include "handlers/CmdSet.hpp"
#include "handlers/CmdTake.hpp"

namespace zappy::engine
{
    const std::map<std::string, CommandInterpreter::GraphicCommandInfo> CommandInterpreter::GRAPHIC_COMMANDS = {
        {"msz", {GraphicCommand::MSZ, &GraphicalClient::sendMsz}},
        {"sgt", {GraphicCommand::SGT, &GraphicalClient::sendSgt}},
        {"tna", {GraphicCommand::TNA, &GraphicalClient::sendTna}},
        {"mct", {GraphicCommand::MCT, &GraphicalClient::sendMct}},
        {"bct", {GraphicCommand::BCT, &GraphicalClient::sendBct}},
        {"ppo", {GraphicCommand::PPO, &GraphicalClient::sendPpo}},
        {"plv", {GraphicCommand::PLV, &GraphicalClient::sendPlv}},
        {"pin", {GraphicCommand::PIN, &GraphicalClient::sendPin}},
        {"sgt", {GraphicCommand::SGT, &GraphicalClient::sendSgt}},
        {"sst", {GraphicCommand::SST, &GraphicalClient::sendSst}},
    };

    const std::map<std::string, CommandInterpreter::CommandInfo> CommandInterpreter::COMMANDS =  {
        {"Forward", {Command::FORWARD, 7, &cmd::CmdForward::cmdForward, &CommandInterpreter::silenceDummyPreAction}},
        {"Right", {Command::RIGHT, 7, &cmd::CmdRight::cmdRight, &CommandInterpreter::silenceDummyPreAction}},
        {"Left", {Command::LEFT, 7, &cmd::CmdLeft::cmdLeft, &CommandInterpreter::silenceDummyPreAction}},
        {"Look", {Command::LOOK, 7, &cmd::CmdLook::cmdLook, &CommandInterpreter::silenceDummyPreAction}},
        {"Inventory", {Command::INVENTORY, 1, &cmd::CmdInventory::cmdInventory, &CommandInterpreter::silenceDummyPreAction}},
        {"Broadcast", {Command::BROADCAST, 7, &cmd::CmdBroadcast::cmdBroadcast, &CommandInterpreter::silenceDummyPreAction}},
        {"Connect_nbr", {Command::CONNECT_NBR, 0, &cmd::CmdConNbr::cmdConNbr, &CommandInterpreter::silenceDummyPreAction}},
        {"Fork", {Command::FORK, 42, &cmd::CmdFork::cmdFork, &cmd::CmdFork::cmdPreFork}},
        {"Eject", {Command::EJECT, 7, &cmd::CmdEject::cmdEject, &CommandInterpreter::silenceDummyPreAction}},
        {"Take", {Command::TAKE, 7, &cmd::CmdTake::cmdTake, &CommandInterpreter::silenceDummyPreAction}},
        {"Set", {Command::SET, 7, &cmd::CmdSet::cmdSet, &CommandInterpreter::silenceDummyPreAction}},
        {"Incantation", {Command::INCANTATION, 300, &cmd::CmdIncantation::cmdIncantation, &cmd::CmdIncantation::cmdPreIncantation}},
    };

    void CommandInterpreter::dummyAction([[maybe_unused]] std::weak_ptr<entities::Player> player, World& world, const std::string& args) // NOLINT(*-unnecessary-value-param)
    {
        std::cout << "Dummy action due to request: " << args << std::endl;
    }

    bool CommandInterpreter::silenceDummyPreAction([[maybe_unused]] std::weak_ptr<entities::Player> player, [[maybe_unused]] World& world, [[maybe_unused]] const std::string& args) // NOLINT(*-unnecessary-value-param)
    {
        return true;
    }
}
