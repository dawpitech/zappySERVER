#pragma once
#include "../../utils/ZappyConfig.hpp"
#include "../World.hpp"
#include <memory>
#include <queue>
#include <string>

namespace zappy::engine {
    class GraphicalClient : public std::enable_shared_from_this<GraphicalClient> {
    public:
        GraphicalClient() = default;
        ~GraphicalClient() = default;


        void addCommandToBuffer(const std::string& command);
        std::queue<std::string>& getCommandsBuffer();
	unsigned int getID();
	void setID(unsigned int id);

        void sendGreetings(zappy::utils::ZappyConfig &config, zappy::engine::World &world, const std::string& args);

        static void sendMsz(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, zappy::engine::World &world, const std::string& args);
        static void sendSgt(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendTna(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendMct(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendBct(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendPpo(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendPlv(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendPin(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendSst(GraphicalClient& graphic, zappy::utils::ZappyConfig &config, World &world, const std::string& args);

        static void sendPnw(std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world, bool firsttime);
        static void sendPnw_proxy(std::vector<std::shared_ptr<GraphicalClient>>& graphics, zappy::utils::ZappyConfig &config, const World &world);
    private:
        std::queue<std::string> _commandsBuffer;
	unsigned int _ID;
    };
}
