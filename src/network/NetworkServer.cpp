/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** NetworkServer.cpp
*/

#include <iostream>
#include <ranges>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/socket.h>

#include "NetworkServer.hpp"
#include "../ZappyServer.hpp"
#include "../utils/Debug.hpp"

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
        std::cout << debug::getTS() << "[INFO] Listening on 0.0.0.0:" << port << std::endl;
    }

    NetworkServer::~NetworkServer()
    {
        for (const auto& client : this->_clients | std::views::values)
            close(client->connectionFD);
    }

    void NetworkServer::pollNetworkActivity(zappy::ZappyServer& zappyServer, const int timeoutMs)
    {
        const size_t POLLS_SIZE = this->_clients.size() + 1;
        std::vector<pollfd> polls(POLLS_SIZE);
        std::vector<unsigned int> pollsCorrespondingClient(POLLS_SIZE);

        polls.at(0).events = POLLIN;
        polls.at(0).fd = this->_serverFD;
        pollsCorrespondingClient.at(0) = -1;

        int x = 1;
        for (const auto& [ID, client] : this->_clients)
        {
            pollsCorrespondingClient.at(x) = ID;
            auto& poll = polls.at(x++);
            poll.events = POLLIN;
            poll.fd = client->connectionFD;
        }

        if (poll(polls.data(), POLLS_SIZE, timeoutMs) == 0)
            return;

        for (int i = static_cast<int>(POLLS_SIZE) - 1; i >= 0; --i)
        {
            if (polls.at(i).revents == 0)
                continue;
            if (i == 0)
                this->acceptNewClient();
            else
            {
                this->parseClientInput(pollsCorrespondingClient.at(i), zappyServer);
            }
        }
    }

    void NetworkServer::writeToClient(const std::string& message, const unsigned int clientID) const
    {
        try
        {
            const auto& client = this->_clients.at(clientID);
            if (client == nullptr) return;

            auto msg = message;
            msg.append("\n");

            pollfd pollConfig{};
            pollConfig.fd = client->connectionFD;
            pollConfig.events = POLLOUT;

            poll(&pollConfig, 1, 0);
            if (pollConfig.revents == 0)
            {
                this->markConnectionAsDead(clientID);
                std::cout << debug::getTS() << "[ERROR] Couldn't write message to client FD" << std::endl;
            }

            if (write(client->connectionFD, msg.c_str(), msg.size()) != msg.size())
            {
                this->markConnectionAsDead(clientID);
                std::cout << debug::getTS() << "[ERROR] Couldn't write message to client FD" << std::endl;
            }
        }
        catch (std::out_of_range&)
        {
            throw NetworkException("Unknown client ID");
        }
    }

    void NetworkServer::markConnectionAsDead(const unsigned int clientID) const
    {
        try
        {
            const auto& client = this->_clients.at(clientID);
            client->alive = false;
        }
        catch (std::out_of_range&) {}
    }

    void NetworkServer::acceptNewClient()
    {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);
        const auto fd = accept(this->_serverFD, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);

        unsigned int clientID = this->clientIDCount++;
        this->_clients.emplace(clientID, std::make_unique<Client>(clientID, fd));

        this->writeToClient("WELCOME", clientID);

        std::cout << debug::getTS() << "[INFO] New client connected (ID " << clientID << ")" << std::endl;
    }

    void NetworkServer::parseClientInput(const unsigned int clientID, zappy::ZappyServer& zappyServer)
    {
        const auto& client = this->_clients.at(clientID);

        if (!client->alive)
        {
            close(client->connectionFD);
            this->_clients.erase(clientID);
            return;
        }

        client->inputBuffer.clear();
        client->inputBuffer.resize(BUFSIZ);

        const long readable_bytes = read(client->connectionFD, client->inputBuffer.data(), BUFSIZ);
        if (readable_bytes <= 0)
        {
            std::cout << debug::getTS() << "[WARN] CLIENT CONNECTION (ID " << client->clientID <<
                ") LOST (NETWORK CLIENT DELETED)" << std::endl;
            if (this->_clients.at(clientID)->managedByGameEngine)
            {
                if (!this->_clients.at(clientID)->isGraphical)
                {
                    this->_clients.at(clientID)->_gameEnginePlayer.lock()->markAsDead();
                }
                else
                {
                    this->_clients.at(clientID)->_gameEngineGraphicalClient.lock()->markAsDead();
                }
            }
            this->_clients.at(clientID)->alive = false;
            return;
        }

        client->inputBuffer.resize(readable_bytes);
        client->messageBuffer.append(client->inputBuffer);

        size_t pos = 0;
        while ((pos = client->messageBuffer.find('\n')) != std::string::npos)
        {
            std::string command = client->messageBuffer.substr(0, pos);
            client->messageBuffer.erase(0, pos + 1);

            if (!command.empty() && command.back() == '\r')
                command.pop_back();
            if (command.empty())
                continue;

            std::cout << debug::getTS() << "[TRACE] (CLIENT ID" << client->clientID << ") SEND:" << command <<
                std::endl;
            this->processCompleteCommand(command, client, zappyServer);
        }
    }

    void NetworkServer::processCompleteCommand(const std::string& command, const std::unique_ptr<Client>& client,
                                               zappy::ZappyServer& zappyServer) const
    {
        if (client->managedByGameEngine)
        {
            if (client->isGraphical)
            {
                const auto graphic = client->_gameEngineGraphicalClient.lock();
                graphic->addCommandToBuffer(command);
            }
            else
            {
                const auto player = client->_gameEnginePlayer.lock();
                player->addCommandToBuffer(command);
            }
        }
        else
        {
            if (command == "GRAPHIC")
            {
                client->managedByGameEngine = true;
                client->isGraphical = true;
                client->_gameEngineGraphicalClient = zappyServer.createNewGraphicalClient(client->clientID);
                auto graphic = client->_gameEngineGraphicalClient.lock();
                std::cout << debug::getTS() << "[TRACE] CLIENT ID " << client->clientID << " JOINED THE GRAPHIC TEAM" <<
                    std::endl;
                return;
            }
            try
            {
                client->_gameEnginePlayer = zappyServer.createNewPlayerInTeam(command, client->clientID);
                client->managedByGameEngine = true;
                std::cout << debug::getTS() << "[TRACE] SWITCHING PROCESSING OF CLIENT ID" << client->clientID <<
                    " TO GAME ENGINE" << std::endl;
            }
            catch (std::runtime_error&)
            {
                this->writeToClient("ko", client->clientID);
            }
        }
    }
}
