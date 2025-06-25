/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CommandInterpreter.cpp
*/

#include "CommandInterpreter.hpp"

#include <iostream>

namespace zappy::engine
{
    const std::map<std::string, CommandInterpreter::CommandInfo> CommandInterpreter::COMMANDS =  {
        {"Right", {Command::RIGHT, 7, &CommandInterpreter::dummyAction}},
        {"Left", {Command::LEFT, 7, &CommandInterpreter::dummyAction}},
        {"Look", {Command::LOOK, 7, &CommandInterpreter::dummyAction}},
        {"Inventory", {Command::INVENTORY, 1, &CommandInterpreter::dummyAction}},
        {"Broadcast", {Command::BROADCAST, 7, &CommandInterpreter::dummyAction}},
        {"Connect_nbr", {Command::CONNECT_NBR, 0, &CommandInterpreter::dummyAction}},
        {"Fork", {Command::FORK, 42, &CommandInterpreter::dummyAction}},
        {"Eject", {Command::EJECT, 7, &CommandInterpreter::dummyAction}},
        {"Take", {Command::TAKE, 7, &CommandInterpreter::dummyAction}},
        {"Set", {Command::SET, 7, &CommandInterpreter::dummyAction}},
        {"Incantation", {Command::INCANTATION, 300, &CommandInterpreter::dummyAction}},
    };

    void CommandInterpreter::dummyAction(Player& player, World& world, const std::string& args)
    {
        std::cout << "Dummy action due to request: " << args << std::endl;
    }
}
