#include "General.hpp"
#include "../Game.hpp"
#include "Player.hpp"
#include <stdexcept>

General::General(const std::string &name, Game &game) : Player(name, game) {
    type = PlayerType::GENERAL;
}
General::General(const Player &other) : Player(other) {
    type = PlayerType::GENERAL;
}

void General::undo_coup(Player* target) {
    try{
        game.undo_coup(this,target);
    }
    catch (const std::runtime_error &e) {
        throw std::runtime_error("General cannot undo coup: " + std::string(e.what()));
    }

}
void General::undo(Player *target){
    this->undo_coup(target);
}


