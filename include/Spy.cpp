// Spy.cpp
#include "Spy.hpp"
#include "../Game.hpp"

Spy::Spy(const std::string &name, Game &game) : Player(name, game) {
    this->type = PlayerType::SPY;
}

Spy::Spy(const Player &other) : Player(other) {}


int Spy::seeCoinsOf(Player* target)  {
    try{
        game.seeCoinsOf(this,target);
    }
    catch (const std::exception &e) {
        throw std::runtime_error("Spy cannot see coins of another player: " + std::string(e.what()));
    }
    return target->getCoins();
}
void Spy::disabel_arrest(Player* target)  {
    
    try{
        game.disabel_arrest(this,target);
    }
    catch (const std::exception &e) {
        throw std::runtime_error("Spy cannot disabel arrest: " + std::string(e.what()));
    }
}

Spy &Spy::operator=(const Spy &other) {
    if (this == &other) return *this;
    Player::operator=(other);
    return *this;
}

