/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** EventRayDirectionInterpreter.hpp
*/

#pragma once

#include <cmath>
#include <stdexcept>

#include "../engine/Directions.hpp"

namespace zappy::engine::utils
{
    inline int getRelativeDirection(
        int receiverX,
        int receiverY,
        Directions receiverFacing,
        int sourceX,
        int sourceY,
        const int worldWidth,
        const int worldHeight,
        const bool diagonals = true)
    {
        auto norm = [](const int v, const int max) { return (v + max) % max; };
        receiverX = norm(receiverX, worldWidth);
        receiverY = norm(receiverY, worldHeight);
        sourceX = norm(sourceX, worldWidth);
        sourceY = norm(sourceY, worldHeight);

        int dx = sourceX - receiverX;
        int dy = sourceY - receiverY;
        if (dx > worldWidth / 2) dx -= worldWidth;
        if (dx < -worldWidth / 2) dx += worldWidth;
        if (dy > worldHeight / 2) dy -= worldHeight;
        if (dy < -worldHeight / 2) dy += worldHeight;

        if (dx == 0 && dy == 0)
            return 0;

        if (!diagonals)
        {
            if (std::abs(dx) > std::abs(dy)) dy = 0;
            else if (std::abs(dy) > std::abs(dx)) dx = 0;
        }

        const int facing = static_cast<int>(receiverFacing);
        for (int i = 0; i < facing; ++i)
        {
            const int tmp = dx;
            dx = -dy;
            dy = tmp;
        }

        if (dx == 0 && dy < 0) return 1;
        if (dx > 0 && dy < 0) return 2;
        if (dx > 0 && dy == 0) return 3;
        if (dx > 0 && dy > 0) return 4;
        if (dx == 0 && dy > 0) return 5;
        if (dx < 0 && dy > 0) return 6;
        if (dx < 0 && dy == 0) return 7;
        if (dx < 0 && dy < 0) return 8;

        throw std::runtime_error("Couldn't compute eject facing value");
    }
}
