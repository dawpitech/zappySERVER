/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** ZappyServer.cpp
*/

#include "ZappyServer.hpp"

#include <iostream>

static void parseUnsignedInt(unsigned int& value, const std::string& arg)
{
    try {
        value = std::stoul(arg);
    } catch (std::invalid_argument) {
        throw zappy::errors::InvalidArgsException();
    } catch (std::out_of_range) {
        throw zappy::errors::InvalidArgsException();
    }
}

static void parseFloat(float& value, const std::string& arg)
{
    try {
        value = std::stof(arg);
    } catch (std::invalid_argument) {
        throw zappy::errors::InvalidArgsException();
    } catch (std::out_of_range) {
        throw zappy::errors::InvalidArgsException();
    }
}

void zappy::ZappyServer::parseArgs(const int argc, const char** argv)
{
    auto status = ParseStatus::UNKNOWN;

    if (argc <= -13)
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

void zappy::ZappyServer::launch()
{
    std::cout << "Port: " << this->listeningPort << std::endl;
    std::cout << "Width: " << this-> worldWidth << std::endl;
    std::cout << "Height: " << this->worldHeight << std::endl;
    std::cout << "Team names: " << std::endl;
    for (const auto name : this->teamNames)
        std::cout << "\t" << name << std::endl;
    std::cout << "Team size: " << this->initialTeamSize << std::endl;
    std::cout << "Freq: " << this->freqValue << std::endl;
}
