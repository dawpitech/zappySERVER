/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** ZappyServer.hpp
*/

#pragma once

#include <chrono>
#include <memory>

#include "engine/Player.hpp"
#include "engine/World.hpp"
#include "network/NetworkServer.hpp"
#include "utils/ZappyConfig.hpp"

namespace zappy
{
    class ZappyServer
    {
        public:
            explicit ZappyServer(const utils::ZappyConfig& config);
            ~ZappyServer() = default;

            void launch();

            std::weak_ptr<engine::Player> createNewPlayerInTeam(const std::string& teamName, unsigned int clientID);
	        std::weak_ptr<engine::GraphicalClient> createNewGraphicalClient(unsigned int id);

            void sendMessageToClient(const std::string& message, unsigned int clientID) const;
            void markClientAsDead(unsigned int clientID) const;

            [[nodiscard]] const utils::ZappyConfig& getConfig() const;
            utils::ZappyConfig& getConfig();

        private:
            utils::ZappyConfig _config;
            std::unique_ptr<generic::NetworkServer> _networkServer;
            std::unique_ptr<engine::World> _world;

            std::chrono::time_point<std::chrono::steady_clock> _lastTickTime;
            std::chrono::milliseconds _tickDuration;
    };
}
