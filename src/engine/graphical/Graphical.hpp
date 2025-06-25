#pragma once
#include "../../utils/ZappyConfig.hpp"

namespace zappy::engine {
    class GraphicalClient {
    public:
	GraphicalClient() = default;
	~GraphicalClient() = default;

	void sendInitData(unsigned int fd, const zappy::utils::ZappyConfig &config);
    private:
	void sendMsz(unsigned int fd, const zappy::utils::ZappyConfig &config);
	void sendSgt(unsigned int fd, const zappy::utils::ZappyConfig &config);
	void sendTna(unsigned int fd, const zappy::utils::ZappyConfig &config);
    };
}
