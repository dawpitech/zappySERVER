#pragma once
#include "../../utils/ZappyConfig.hpp"
#include "../World.hpp"
#include <queue>
#include <string>

namespace zappy::engine {
    class GraphicalClient {
    public:
        GraphicalClient() = default;
        ~GraphicalClient() = default;


        void addCommandToBuffer(const std::string& command);
        std::queue<std::string>& getCommandsBuffer();
	unsigned int getID();
	void setID(unsigned int id);

        void sendGreetings(const zappy::utils::ZappyConfig &config, zappy::engine::World &world, const std::string& args);

        static void sendMsz(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, zappy::engine::World &world, const std::string& args);
        static void sendSgt(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendTna(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendMct(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendBct(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendPpo(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args);
        static void sendPlv(GraphicalClient& graphic, const zappy::utils::ZappyConfig &config, World &world, const std::string& args);

    private:
        std::queue<std::string> _commandsBuffer;
	unsigned int _ID;
    };
}
