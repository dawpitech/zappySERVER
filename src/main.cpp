/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** main.cpp
*/

#include <iostream>

#include "ZappyServer.hpp"

int main(const int argc, const char **argv)
{
    auto server = zappy::ZappyServer();

    server.parseArgs(argc, argv);
    server.launch();

    return 0;
}
