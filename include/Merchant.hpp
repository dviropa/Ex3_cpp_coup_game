#pragma once
#include "Player.hpp"

class Game;   // forward declaration
class Merchant : public Player {
public:

    Merchant(const std::string &name, Game &game);
    Merchant(const Player &other);

    Merchant &operator=(const Merchant &other);
    friend class Game;
};
