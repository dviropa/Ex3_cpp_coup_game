#pragma once
#include <string>

struct PlayerDetails {
    std::string name;
    std::string role;
    int coins;
    
        PlayerDetails(const std::string& name, const std::string& role, int coins)
        : name(name), role(role), coins(coins) {}
};
