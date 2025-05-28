#include "Baron.hpp"
#include "Player.hpp"
#include "../Game.hpp"

#include <stdexcept>

Baron::Baron(const std::string &name, Game &game) : Player(name, game) {
    type = PlayerType::BARON;
}
Baron::Baron(const Player &other) : Player(other) {
    type = PlayerType::BARON;
}

void Baron::invest() {
    try{
        game.invest(this);
    }
    catch (const std::exception &e) {
        throw std::runtime_error("Baron cannot invest: " + std::string(e.what()));
    }
}


