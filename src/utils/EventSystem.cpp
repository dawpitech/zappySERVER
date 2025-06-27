#include "EventSystem.hpp"

std::unordered_map<std::string, std::vector<EventSystem::EventCallback>> EventSystem::callbacks;
