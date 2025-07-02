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
        {"Forward", {Command::FORWARD, 7, &cmd::CmdForward::cmdForward}},
        {"Right", {Command::RIGHT, 7, &cmd::CmdRight::cmdRight}},
        {"Left", {Command::LEFT, 7, &cmd::CmdLeft::cmdLeft}},
        {"Look", {Command::LOOK, 7, &cmd::CmdLook::cmdLook}},
        {"Inventory", {Command::INVENTORY, 1, &cmd::CmdInventory::cmdInventory}},
        {"Broadcast", {Command::BROADCAST, 7, &CommandInterpreter::dummyAction}},
        {"Connect_nbr", {Command::CONNECT_NBR, 0, &cmd::CmdConNbr::cmdConNbr}},
        {"Fork", {Command::FORK, 42, &cmd::CmdFork::cmdFork}},
        {"Eject", {Command::EJECT, 7, &CommandInterpreter::dummyAction}},
        {"Take", {Command::TAKE, 7, &cmd::CmdTake::cmdTake}},
        {"Set", {Command::SET, 7, &cmd::CmdSet::cmdSet}},
        {"Incantation", {Command::INCANTATION, 300, &CommandInterpreter::dummyAction}},
    };

    void CommandInterpreter::dummyAction(std::weak_ptr<Player> player, World& world, const std::string& args)
    {
        std::cout << "Dummy action due to request: " << args << std::endl;
    }
}
