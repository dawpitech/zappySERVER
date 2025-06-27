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
    template<typename... Args>
    using EventCallback = std::function<void(const std::vector<std::shared_ptr<zappy::engine::GraphicalClient>>& graphics, zappy::utils::ZappyConfig& config, const zappy::engine::World& world, Args... args)>;

    template<typename... Args>
    static void subscribe(const std::string& eventName, EventCallback<Args...> callback) {
        std::cout << debug::getTS() << "[TRACE][EVENT] Subscribed to event " << eventName << std::endl;
        callbacks<Args...>[eventName].push_back(callback);
    }

    template<typename... Args>
    static void trigger(const std::string& eventName, const std::vector<std::shared_ptr<zappy::engine::GraphicalClient>>& graphics, zappy::utils::ZappyConfig& config, const zappy::engine::World& world, Args... args) {
        std::cout << debug::getTS() << "[TRACE][EVENT] Triggered event " << eventName << std::endl;
        auto it = callbacks<Args...>.find(eventName);
        if (it != callbacks<Args...>.end()) {
            for (auto& callback : it->second) {
                callback(graphics, config, world, args...);
            }
        }
    }

private:
    template<typename... Args>
    static std::unordered_map<std::string, std::vector<EventCallback<Args...>>> callbacks;
};

template<typename... Args>
std::unordered_map<std::string, std::vector<typename EventSystem::EventCallback<Args...>>> EventSystem::callbacks;

