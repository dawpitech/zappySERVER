/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** ZappyErrors.hpp
*/

#pragma once

#include <stdexcept>

namespace zappy::errors
{
    class ZappyException : public std::runtime_error
    {
        public:
            explicit ZappyException(const std::string& what) : std::runtime_error(what) {}
    };
    class InvalidArgsException final : public ZappyException
    {
        public:
            InvalidArgsException() : ZappyException("Invalid arguments given") {}
    };
}
