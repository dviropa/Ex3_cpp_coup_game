#include "Governor.hpp"
#include "../Game.hpp"

Governor::Governor(const std::string &name, Game &game) : Player(name, game)
{
    type = PlayerType::GOVERNOR;
}
Governor::Governor(const Player &other) : Player(other)
{
    type = PlayerType::GOVERNOR;
}

void Governor::tax()
{
    try{
        game.tax(this,3);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Governor cannot tax: " + std::string(e.what()));
    }   
}

void Governor::disabel_tax(Player *target)
{
    try{
        game.disabel_tax(this,target);
    }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Governor cannot disabel tax: " + std::string(e.what()));
        }
}
