#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <iostream>
#include "../engine/graphical/Graphical.hpp"
#include "Debug.hpp"
#include "ZappyConfig.hpp"

class EventSystem {
public:

    using EventCallback = std::function<void(std::vector<std::shared_ptr<zappy::engine::GraphicalClient>> &graphics, zappy::utils::ZappyConfig &config, const zappy::engine::World &world)>;

    static void subscribe(const std::string& eventName, EventCallback callback) {
	std::cout << debug::getTS() << "[TRACE][EVENT] Subscribed to event " << eventName << std::endl;
        callbacks[eventName].push_back(callback);
    }
    
    static void trigger(const std::string& eventName, std::vector<std::shared_ptr<zappy::engine::GraphicalClient>> &graphics, zappy::utils::ZappyConfig &config, const zappy::engine::World &world) {
	std::cout << debug::getTS() << "[TRACE][EVENT] Triggered event " << eventName << std::endl;
        auto it = callbacks.find(eventName);
        if (it != callbacks.end()) {
            for (auto& callback : it->second) {
                callback(graphics, config, world);
            }
        }
    }
    
private:
    static std::unordered_map<std::string, std::vector<EventCallback>> callbacks;
};
