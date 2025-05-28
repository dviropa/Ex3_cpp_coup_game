#pragma once
#include "Player.hpp"

class General : public Player {
public:
    General(const std::string &name, Game &game);
    General(const Player &other) ;
void undo(Player *target)override;
    void undo_coup(Player* target)override ;

};
