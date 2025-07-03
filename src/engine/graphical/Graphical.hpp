/*
** EPITECH PROJECT, 2025
** zappySERVER
** File description:
** Graphical.hpp
*/

#pragma once

#include <memory>
#include <queue>
#include <string>

#include "../../utils/ZappyConfig.hpp"
#include "../World.hpp"

namespace zappy::engine
{
    class GraphicalClient : public std::enable_shared_from_this<GraphicalClient>
    {
        public:
            GraphicalClient() = default;
            ~GraphicalClient() = default;


            void addCommandToBuffer(const std::string& command);
            std::queue<std::string>& getCommandsBuffer();
            unsigned int getID() const;
            void setID(unsigned int id);

            void sendGreetings(zappy::utils::ZappyConfig& config, const zappy::engine::World& world,
                               const std::string& args);

            static void sendSuc(unsigned int graphic_id, const zappy::engine::World& world);

            static void sendEnw(GraphicalClient& graphic, zappy::utils::ZappyConfig& config, const World& world);
            static void sendMsz(GraphicalClient& graphic, zappy::utils::ZappyConfig& config, const World& world,
                                const std::string& args);
            static void sendSgt(GraphicalClient& graphic, zappy::utils::ZappyConfig& config, const World& world,
                                const std::string& args);
            static void sendTna(GraphicalClient& graphic, zappy::utils::ZappyConfig& config, const World& world,
                                const std::string& args);
            static void sendMct(GraphicalClient& graphic, zappy::utils::ZappyConfig& config, const World& world,
                                const std::string& args);
            static void sendBct(GraphicalClient& graphic, zappy::utils::ZappyConfig& config, const World& world,
                                const std::string& args);
            static void sendPpo(GraphicalClient& graphic, zappy::utils::ZappyConfig& config, const World& world,
                                const std::string& args);
            static void sendPlv(GraphicalClient& graphic, zappy::utils::ZappyConfig& config, const World& world,
                                const std::string& args);
            static void sendPin(GraphicalClient& graphic, zappy::utils::ZappyConfig& config, const World& world,
                                const std::string& args);
            static void sendSst(GraphicalClient& graphic, zappy::utils::ZappyConfig& config, const World& world,
                                const std::string& args);

            [[nodiscard]] bool isDead() const { return !this->alive; }
            void markAsDead() { this->alive = false; }

            static void sendPinProxy(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                     zappy::utils::ZappyConfig& config, const World& world, unsigned int unused,
                                     unsigned int pl_id);
            static void sendPnw(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world, bool firsttime);
            static void sendPnw_proxy(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                      zappy::utils::ZappyConfig& config, const World& world);
            static void sendPdr(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world, unsigned int res_id,
                                unsigned int pl_id);
            static void sendPex(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world, unsigned int pl_id);
            static void sendPpoProxy(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                     zappy::utils::ZappyConfig& config, const World& world, unsigned int pl_id);
            static void sendPbc(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world, unsigned int pl_id,
                                std::string msg);
            static void sendPfk(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world, unsigned int pl_id);
            static void sendMctProxy(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                     zappy::utils::ZappyConfig& config, const World& world);
            static void sendEnwProxy(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                     zappy::utils::ZappyConfig& config, const World& world,
                                     std::weak_ptr<engine::entities::Egg> egg);
            static void sendEbo(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world, unsigned int egg_id);
            static void sendEdi(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world, unsigned int egg_id);
            static void sendPdi(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world, unsigned int pl_id);
            static void sendPgt(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world, unsigned int res_id,
                                unsigned int pl_id);
            static void sendPic(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world,
                                std::vector<std::weak_ptr<engine::entities::Player>> players);
            static void sendPie(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world,
                                std::weak_ptr<engine::entities::Player> player, int success);


            static void sendSmg(const std::vector<std::shared_ptr<GraphicalClient>>& graphics,
                                zappy::utils::ZappyConfig& config, const World& world, const std::string& message);

        private:
            bool alive = true;
            std::queue<std::string> _commandsBuffer;
            unsigned int ID = -1;
    };
}
