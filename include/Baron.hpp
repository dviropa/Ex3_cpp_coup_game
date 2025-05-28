#pragma once
#include "Player.hpp"

class Baron : public Player {
public:
    Baron(const std::string &name, Game &game);
    Baron(const Player &other);
    void invest()override;
};
