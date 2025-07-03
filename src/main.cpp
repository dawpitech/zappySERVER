/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** main.cpp
*/

#include "ZappyServer.hpp"
#include "utils/ZappyErrors.hpp"

int main(const int argc, const char** argv)
{
    try
    {
        const auto config = zappy::utils::ZappyConfig(argc, argv);
        auto server = zappy::ZappyServer(config);

        server.launch();
    }
    catch (zappy::errors::InvalidArgsException&)
    {
        zappy::utils::ZappyConfig::printHelp();
        return 84;
    } catch (zappy::errors::EarlyExit&)
    {
        return 0;
    }
}
