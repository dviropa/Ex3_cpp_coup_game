#include "Merchant.hpp"
#include "../Game.hpp"

Merchant::Merchant(const std::string &name, Game &game) : Player(name, game) {
    type = PlayerType::MERCHANT;
}
Merchant::Merchant(const Player &other) : Player(other) {
    type = PlayerType::MERCHANT;
}

Merchant &Merchant::operator=(const Merchant &other) {
    if (this != &other) {
        Player::operator=(other);
        type = PlayerType::MERCHANT;
    }
    return *this;
}

