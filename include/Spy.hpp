#pragma once
#include "Player.hpp"

class Spy : public Player
{
public:
    Spy(const std::string &name, Game &game);
    Spy(const Player &other);
    Spy &operator=(const Spy &other);
    int seeCoinsOf(Player *target)  override;
    void disabel_arrest(Player *target)  override;

    friend class Game;
};
