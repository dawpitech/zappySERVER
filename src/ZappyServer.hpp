/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** ZappyServer.hpp
*/

#pragma once

#include <vector>

#include "ZappyErrors.hpp"

namespace zappy
{
    class ZappyServer
    {
        public:
            ZappyServer() = default;
            ~ZappyServer() = default;

            void parseArgs(int argc, const char **argv);

            void launch();

        private:
            enum class ParseStatus
            {
                UNKNOWN,
                PORT,
                WIDTH,
                HEIGHT,
                TEAM_NAME,
                TEAM_SIZE,
                FREQ,
            };

            unsigned int listeningPort;
            unsigned int worldWidth;
            unsigned int worldHeight;
            std::vector<std::string> teamNames;
            unsigned int initialTeamSize;
            float freqValue;
    };
}
