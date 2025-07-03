/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** ZappyConfig.cpp
*/

#include "ZappyErrors.hpp"
#include "ZappyConfig.hpp"

#include <iostream>

zappy::utils::ZappyConfig::ZappyConfig(const int argc, const char** argv)
{
    auto status = ParseStatus::UNKNOWN;

    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        printHelp();
        throw errors::EarlyExit();
    }

    if (argc < 13 || argv == nullptr)
        throw errors::InvalidArgsException();

    for (int i = 1; i < argc; i++)
    {
        const auto arg = std::string(argv[i]);

        if (arg.at(0) == '-')
            status = ParseStatus::UNKNOWN;

        switch (status)
        {
            case ParseStatus::PORT:
                parseUnsignedInt(this->listeningPort, arg);
                break;
            case ParseStatus::WIDTH:
                parseUnsignedInt(this->worldWidth, arg);
                break;
            case ParseStatus::HEIGHT:
                parseUnsignedInt(this->worldHeight, arg);
                break;
            case ParseStatus::TEAM_NAME:
                this->teamNames.push_back(arg);
                break;
            case ParseStatus::TEAM_SIZE:
                parseUnsignedInt(this->initialTeamSize, arg);
                break;
            case ParseStatus::FREQ:
                parseFloat(this->freqValue, arg);
                break;
            default:
            case ParseStatus::UNKNOWN:
                {
                    if (arg.at(0) != '-') {
                        std::cout << "Here" << std::endl;
                        throw errors::InvalidArgsException();
                    }

                    switch (arg.at(1))
                    {
                        case 'p': status = ParseStatus::PORT; break;
                        case 'x': status = ParseStatus::WIDTH; break;
                        case 'y': status = ParseStatus::HEIGHT; break;
                        case 'n': status = ParseStatus::TEAM_NAME; break;
                        case 'c': status = ParseStatus::TEAM_SIZE; break;
                        case 'f': status = ParseStatus::FREQ; break;
                        default: throw errors::InvalidArgsException();
                    }
                    break;
                }
        }
    }
}

void zappy::utils::ZappyConfig::printHelp()
{
    std::cout << "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq\n"
        << "port\t\tis the port number\n"
        << "width\t\tis the width of the world\n"
        << "height\t\tis the height of the world\n"
        << "nameX\t\tis the name of the team X\n"
        << "clientsNb\tis the number of authorized clients per team\n"
        << "freq\t\tis the reciprocal of time unit for execution of actions"
        << std::endl;
}

void zappy::utils::ZappyConfig::parseUnsignedInt(unsigned int& value, const std::string& arg)
{
    try {
        value = std::stoul(arg);
    } catch (std::invalid_argument&) {
        throw errors::InvalidArgsException();
    } catch (std::out_of_range&) {
        throw errors::InvalidArgsException();
    }
}

void zappy::utils::ZappyConfig::parseFloat(float& value, const std::string& arg)
{
    try {
        value = std::stof(arg);
    } catch (std::invalid_argument) {
        throw errors::InvalidArgsException();
    } catch (std::out_of_range) {
        throw errors::InvalidArgsException();
    }
}
