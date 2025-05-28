#pragma once
#include "Player.hpp"

class Player;

class Governor : public Player{
    public:
        Governor(const std::string &name, Game &game);
        Governor(const Player &other);

        void tax() override;
        void disabel_tax(Player *target) override;
};

