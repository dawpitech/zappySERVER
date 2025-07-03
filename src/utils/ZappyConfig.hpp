/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** ZappyConfig.hpp
*/

#pragma once

#include <string>
#include <vector>

namespace zappy::utils
{
    class ZappyConfig
    {
        public:
            explicit ZappyConfig(int argc, const char** argv);
            ~ZappyConfig() = default;

            unsigned int listeningPort = 0;
            unsigned int worldWidth = 0;
            unsigned int worldHeight = 0;
            std::vector<std::string> teamNames;
            unsigned int initialTeamSize = 0;
            float freqValue = 0;

            static void printHelp();

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

            static void parseUnsignedInt(unsigned int& value, const std::string& arg);
            static void parseFloat(float& value, const std::string& arg);
    };
}
