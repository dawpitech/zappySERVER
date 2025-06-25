/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** main.cpp
*/

#include "ZappyServer.hpp"

int main(const int argc, const char **argv)
{
    const auto config = zappy::utils::ZappyConfig(argc, argv);
    auto server = zappy::ZappyServer(config);

    server.launch();
}
