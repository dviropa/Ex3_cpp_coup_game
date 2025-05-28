#include "Player.hpp"
#include "../Game.hpp"
#include "../Action.hpp"

Player::Player(const std::string &name, Game &game) : name(name), game(game), coins(0) , profit_block(false), arrest_block(false)
{
}

Player::Player(const Player &other)
    : name(other.name),
      game(other.game), 
      coins(other.coins),
      type(other.type),
      profit_block(other.profit_block),
      arrest_block(other.arrest_block)
{
}

Player &Player::operator=(const Player &other)
{
    if (this == &other)
        return *this;
    this->name = other.name;
    coins = other.coins;
    type = other.type;
    profit_block = other.profit_block;
    arrest_block = other.arrest_block;
    return *this;
}


void Player::gather()
{
    try{
        game.gather(this);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Player cannot gather: " + std::string(e.what()));
    }
}
bool Player::getProfitBlock() const{
    return profit_block;
}
    bool Player::getArrestBlock()const {
    return arrest_block;
    }

void Player::bribe()
{
    try{
        game.bribe(this);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Player cannot bribe: " + std::string(e.what()));
    }
}

void Player::arrest(Player *target)
{
    try{
        game.arrest(this,target);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Player cannot arrest: " + std::string(e.what()));
    }
}

void Player::sanction(Player *target)
{
    try{
        game.sanction(this,target);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Player cannot sanction: " + std::string(e.what()));
    }
}

void Player::coup(Player *target)
{
    try{
        game.coup(this,target);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Player cannot coup: " + std::string(e.what()));
    }
}

bool Player::operator==(const Player &other) const
{
    return this->name == other.name && this->type == other.type;
}
std::string Player::getName() const
{
    return name;
}

void Player::tax()
{
    try{
        game.tax(this,2);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Player cannot tax: " + std::string(e.what()));
    }
}

void Player::undo(Player *target)
{
    throw std::runtime_error("Player cannot undo");
}


void Player::disabel_arrest(Player *target){
    throw std::runtime_error("Player cannot disabel arrest");
}
int Player::seeCoinsOf(Player *target){
    throw std::runtime_error("Player cannot see coins of other players");
}
void Player::invest(){
    throw std::runtime_error("Player cannot invest");
}
void Player::undo_coup(Player *target){
    throw std::runtime_error("Player cannot undo coup");
}
void Player::BlockBribe(Player *target){
    throw std::runtime_error("Player cannot block bribe");
}
void Player::undo_tax(Player *target){
    throw std::runtime_error("Player cannot undo tax");
}

void Player::addCoins(int amount) { coins += amount; }
void Player::removeCoins(int amount) { coins -= amount; }

int Player::getCoins() const { return coins; }
PlayerType Player::getType() const { return type; }

void Player::setProfitBlock(bool block) { profit_block = block; }
void Player::setArrestBlock(bool block) { arrest_block = block; }

void Player::disabel_tax(Player *target)
{
    throw std::runtime_error("Only Governer can disabel tax");
}