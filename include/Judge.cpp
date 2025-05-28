#include "Judge.hpp"
#include "../Game.hpp"

#include <stdexcept>

Judge::Judge( const std::string &name, Game &game) : Player(name, game) {
    type = PlayerType::JUDGE;
}
Judge::Judge(const Player &other) : Player(other) {

    type = PlayerType::JUDGE;
}


void Judge::BlockBribe(Player* bribingPlayer)  {
    try{
        game.undo_bribe(this,bribingPlayer);
    }
    catch (const std::exception &e) {
        throw std::runtime_error("Judge cannot block bribe: " + std::string(e.what()));
    }
}

    

