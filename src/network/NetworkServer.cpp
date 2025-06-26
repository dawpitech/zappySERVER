/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** NetworkServer.cpp
*/

#include "NetworkServer.hpp"

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/socket.h>

#include "../ZappyServer.hpp"

namespace generic
{
    NetworkServer::NetworkServer(const unsigned short port)
        : _serverFD(0), _port(port), clientIDCount(0)
    {
        constexpr int reuse = 1;
        sockaddr_in socket_addr{};
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_port = htons(port);
        socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        this->_serverFD = socket(AF_INET, SOCK_STREAM, 0);
        if (this->_serverFD == -1)
            throw NetworkException("Cannot open socket");
        if (setsockopt(this->_serverFD, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
            throw NetworkException("Cannot set socket options");
        if (bind(this->_serverFD, reinterpret_cast<sockaddr*>(&socket_addr), sizeof(socket_addr)) == -1)
            throw NetworkException("Cannot bind socket to given port");
        if (listen(this->_serverFD, QUEUE_SIZE) == -1)
            throw NetworkException("Cannot listen on socket");
        std::cout << "[INFO] Listening on 0.0.0.0:" << port << std::endl;
    }

    void NetworkServer::pollNetworkActivity(zappy::ZappyServer& zappyServer)
    {
        const size_t POLLS_SIZE = this->_clients.size() + 1;
        std::vector<pollfd> polls(POLLS_SIZE);

        polls.at(0).events = POLLIN;
        polls.at(0).fd = this->_serverFD;

        for (size_t i = 0; i < this->_clients.size(); i++) {
            polls.at(i + 1).events = POLLIN;
            polls.at(i + 1).fd = this->_clients.at(i)->connectionFD;
        }

        const int pollResult = poll(polls.data(), POLLS_SIZE, static_cast<int>(1000 / zappyServer.getConfig().freqValue));

        if (pollResult == 0)
            return;

        for (int i = static_cast<int>(POLLS_SIZE) - 1; i >= 0; --i) {
            if (polls.at(i).revents == 0)
                continue;
            if (i == 0)
                this->acceptNewClient();
            else {
                this->parseClientInput(i - 1, zappyServer);
            }
        }
    }

    void NetworkServer::writeToClient(const std::string& message, const unsigned int clientID) const
    {
        try {
            const auto& client = this->_clients.at(clientID);

            auto msg = message;
            msg.append("\n");

            if (write(client->connectionFD, msg.c_str(), msg.size()) != msg.size())
                throw NetworkException("Error while writing message");
        } catch (std::out_of_range&) {
            throw NetworkException("Unknown client ID");
        }
    }

    void NetworkServer::acceptNewClient()
    {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);
        const auto fd = accept(this->_serverFD, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);

        this->_clients.emplace_back(std::make_unique<Client>(this->clientIDCount++, fd));
        const auto& newClient = this->_clients.back();

        this->writeToClient("WELCOME", newClient->clientID);

        std::cout << "[INFO] New client connected (ID " << newClient->clientID << ")" << std::endl;
    }

    void NetworkServer::parseClientInput(const int clientIdx, zappy::ZappyServer& zappyServer)
    {
        const auto& client = this->_clients.at(clientIdx);
        client->inputBuffer.clear();
        client->inputBuffer.resize(BUFSIZ);

        const long readable_bytes = read(client->connectionFD, client->inputBuffer.data(), BUFSIZ);
        if (readable_bytes <= 0) {
            std::cout << "[WARN] CLIENT CONNECTION (ID " << client->clientID << ") LOST (NETWORK CLIENT DELETED)" << std::endl;
            this->_clients.erase(this->_clients.begin() + clientIdx);
            //TODO: notify the game engine of the player death
            return;
        }

        client->inputBuffer.resize(readable_bytes);

        // TODO: remove the bellow line, end of line char should be handle in the buffer management system
        while (!client->inputBuffer.empty() && (client->inputBuffer.back() == '\n' || client->inputBuffer.back() == '\r'))
            client->inputBuffer.pop_back();
        std::cout << "[TRACE] (CLIENT ID" << client->clientID << ") SEND:" << client->inputBuffer << std::endl;

        if (client->managedByGameEngine) {
            if (client->isGraphical) {
                const auto graphic = client->_gameEngineGraphicalClient.lock();
                graphic->addCommandToBuffer(client->inputBuffer);
            } else {
                const auto player = client->_gameEnginePlayer.lock();
                player->addCommandToBuffer(client->inputBuffer);
            }
        } else {
            if (client->inputBuffer == "GRAPHIC") {
                client->managedByGameEngine = true;
                client->isGraphical = true;
                client->_gameEngineGraphicalClient = zappyServer.createNewGraphicalClient(client->clientID);
                auto graphic = client->_gameEngineGraphicalClient.lock();
                std::cout << "[TRACE] CLIENT ID " << client->clientID << " JOINED THE GRAPHIC TEAM" << std::endl;
                return;
            }
            try {
                client->_gameEnginePlayer = zappyServer.createNewPlayerInTeam(client->inputBuffer, clientIdx);
                client->managedByGameEngine = true;
                std::cout << "[TRACE] SWITCHING PROCESSING OF CLIENT ID" << client->clientID << " TO GAME ENGINE" << std::endl;
            } catch (std::runtime_error&) {
                this->writeToClient("ko", client->clientID);
            }
        }
    }
} // generic
