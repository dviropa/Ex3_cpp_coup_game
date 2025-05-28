#pragma once

#include <string>
#include <stdexcept>
#include "PlayerType.hpp"

class Game; // forward declaration
class Action; // forward declaration
class Player
{
protected:
    std::string name;
    Game &game; // reference to the game object
    int coins;
    PlayerType type;
    bool profit_block;
    bool arrest_block;

    Player(const std::string &name, Game &game);
    Player(const Player &other);
    virtual ~Player() = default; // virtual destructor
    Player &operator=(const Player &other);

    bool operator==(const Player &other) const;

    // game functions

  
public:
    bool getProfitBlock() const;
    bool getArrestBlock() const;
    std::string getName() const;

    void addCoins(int amount);
    void removeCoins(int amount);
  
    void gather();
    virtual void tax();
    void bribe();
    void arrest(Player *target);
    void sanction(Player *target);
    void coup(Player *target);
    virtual void undo(Player *target);
    virtual void disabel_tax(Player *target);

    virtual void disabel_arrest(Player *target);
    virtual int seeCoinsOf(Player *target) ;
    virtual void invest();
        virtual void undo_coup(Player *target);
    virtual void BlockBribe(Player *target);
    virtual void undo_tax(Player *target);
    
    int getCoins() const;
    PlayerType getType() const;

    void setProfitBlock(bool block);
    void setArrestBlock(bool block);
    friend class Game;
};
