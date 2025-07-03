/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** NetworkServer.hpp
*/

#pragma once
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>
#include <sys/poll.h>
#include <sys/socket.h>

#include "../engine/Player.hpp"
#include "../engine/graphical/Graphical.hpp"

namespace zappy
{
    class ZappyServer;
}

namespace generic
{
    class NetworkServer
    {
        public:
            class Client
            {
                public:
                    Client(const int ID, const int fd)
                        : clientID(ID), connectionFD(fd)
                    {
                        this->inputBuffer.resize(BUFSIZ);
                    }
                    ~Client()
                    {
                        if (connectionFD != -1)
                            close(connectionFD);
                    }

                    Client(const Client&) = delete;
                    Client& operator=(const Client&) = delete;

                    Client(Client&& o) noexcept
                      : clientID(o.clientID)
                      , connectionFD(o.connectionFD)
                      , inputBuffer(std::move(o.inputBuffer))
                    {
                        o.connectionFD = -1;
                    }

                    Client& operator=(Client&& o) noexcept {
                        if (this != &o) {
                            if (connectionFD >= 0) ::close(connectionFD);
                            clientID = o.clientID;
                            connectionFD = o.connectionFD;
                            inputBuffer = std::move(o.inputBuffer);
                            o.connectionFD = -1;
                        }
                        return *this;
                    }

                    unsigned int clientID;
                    int connectionFD;

                    bool managedByGameEngine = false;
                    bool isGraphical = false;
                    bool alive = true;
                    std::weak_ptr<zappy::engine::Player> _gameEnginePlayer;
                    std::weak_ptr<zappy::engine::GraphicalClient> _gameEngineGraphicalClient;

                    std::string inputBuffer;
            };
            class NetworkException final : public std::runtime_error
                { public: explicit NetworkException(const std::string& what) : std::runtime_error(what) {} };

            explicit NetworkServer(unsigned short port);
            ~NetworkServer() = default;

            void pollNetworkActivity(zappy::ZappyServer& zappyServer, int timeoutMs);
            void writeToClient(const std::string& message, unsigned int clientID) const;
            void markConnectionAsDead(unsigned int clientID) const;
	        [[nodiscard]] bool isClientDead(unsigned int id) const;

        private:
            constexpr static size_t QUEUE_SIZE = 8;

            int _serverFD;
            unsigned short _port;
            std::map<unsigned int, std::unique_ptr<Client>> _clients;
            unsigned int clientIDCount;

            void acceptNewClient();
            void parseClientInput(int clientIdx, zappy::ZappyServer& zappyServer);
    };
} // generic
