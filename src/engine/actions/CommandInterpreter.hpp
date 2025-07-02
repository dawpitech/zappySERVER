/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** CommandInterpreter.hpp
*/

#pragma once

#include <map>
#include <memory>
#include <stdexcept>

namespace zappy::utils {
    class ZappyConfig;
}

namespace zappy::engine {
    class World;
}

namespace zappy::engine {
    class Player;
    class GraphicalClient;
}

namespace zappy::engine
{
    enum class Command
    {
        FORWARD,
        RIGHT,
        LEFT,
        LOOK,
        INVENTORY,
        BROADCAST,
        CONNECT_NBR,
        FORK,
        EJECT,
        TAKE,
        SET,
        INCANTATION,
    };

    enum class GraphicCommand
    {
	MSZ,
	BCT,
	MCT,
	TNA,
	PNW,
	PPO,
	PLV,
	PIN,
	PEX,
	PBC,
	PIC,
	PIE,
	PFK,
	PDR,
	PGT,
	PDI,
	ENW,
	EBO,
	EDI,
	SGT,
	SST,
	SEG,
	SMG,
	SUC,
	SBP,
    };

    class CommandInterpreter
    {
        public:
            class UnknownCommandException final : public std::runtime_error
            {
                public:
                    UnknownCommandException() : std::runtime_error("Unknown command given") {}
            };

            struct CommandInfo
            {
                Command command;
                unsigned int duration;
                void (*handler)(std::weak_ptr<Player> player, World& world, const std::string& args);
            	void (*preHandler)(std::weak_ptr<Player> player, World& world, const std::string& args);
            };

            struct GraphicCommandInfo
            {
                GraphicCommand command;
                void (*handler)(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, World& world, const std::string& args);
            };

            CommandInterpreter() = delete;
            static void dummyAction(std::weak_ptr<Player> player, World& world, const std::string& args);
    		static void silenceDummyAction(std::weak_ptr<Player> player, World& world, const std::string& args);

            const static std::map<std::string, CommandInfo> COMMANDS;
            const static std::map<std::string, GraphicCommandInfo> GRAPHIC_COMMANDS;

    };
}
