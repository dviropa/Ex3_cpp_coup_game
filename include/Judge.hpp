#pragma once
#include "Player.hpp"

class Judge : public Player{

    public:
    Judge( const std::string &name, Game &game);
    Judge(const Player &other);
    
        

    void BlockBribe(Player* bribingPlayer) override;
    };
    