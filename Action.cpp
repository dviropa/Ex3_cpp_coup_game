#include "Action.hpp"

Action::Action(Player *player, Game &game, ActionType actionT) : game(game), player(player), actionType(actionT) {}
Action::~Action()
{
}

ActionType Action::getType() const
{
    return actionType;
}

bool Action::equals(const Action *other) const
{
    return this->actionType == other->getType();
}

TaxAction::TaxAction(Player *player, Game &game, int amount) : Action(player, game, ActionType::TAX), amount(amount) {}
Player *TaxAction::getPlayer()
{
    return player;
}
void TaxAction::execute()
{
    if (player->getType() == PlayerType::GOVERNOR)
    {
        amount = 3;
    }
    else
    {
        amount = 2;
    }

    player->addCoins(amount);
}

void TaxAction::undo()
{

    if (player->getType() == PlayerType::GOVERNOR)
    {
        amount = 3;
    }
    else
    {
        amount = 2;
    }
    player->removeCoins(amount);
    game.remove_player_if_nececery(player);
}
// GatherAction

GatherAction::GatherAction(Player *player, Game &game) : Action(player, game, ActionType::GATHER) {}

void GatherAction::execute()
{

    player->addCoins(1);
}

void GatherAction::undo()
{
    player->removeCoins(1);
    game.remove_player_if_nececery(player);
}

arrestAction::arrestAction(Player *player, Player *target, Game &game) : Action(player, game, ActionType::ARREST), target(target) {}
Player *arrestAction::getTarget()
{
    return target;
}

void arrestAction::execute()
{
    if (!game.is_player_in_game(target))
    {
        throw std::runtime_error("Player is already out of the game.");
    }

    if (target->getType() == PlayerType::MERCHANT)
    {
        target->removeCoins(2);
        game.remove_player_if_nececery(target);
    }
    else if (target->getType() != PlayerType::GENERAL)
    {
        player->addCoins(1);
    }

    else
    {
        target->removeCoins(1);
        game.remove_player_if_nececery(target);
        player->addCoins(1);
    }
}

void arrestAction::undo()
{
    if (!game.is_player_in_game(target))
    {
        throw std::runtime_error("Player is already out of the game.");
    }
    if (target->getType() == PlayerType::MERCHANT)
    {
        target->addCoins(2);
    }
    else if (target->getType() != PlayerType::GENERAL)
    {
        player->removeCoins(1);
        game.remove_player_if_nececery(target);
    }

    else
    {
        target->addCoins(1);
        player->removeCoins(1);
        game.remove_player_if_nececery(target);
    }
}

sanctionAction::sanctionAction(Player *player, Player *target, Game &game) : Action(player, game, ActionType::SANCTION), target(target) {}

Player *sanctionAction::getTarget()
{
    return target;
}
void sanctionAction::execute()
{
    if (game.is_player_in_game(target) == false)
    {
        throw std::runtime_error("target is already out of the game.");
    }

    if (target->getType() == PlayerType::BARON)
    {
        target->addCoins(1);
    }
    if (target->getType() == PlayerType::JUDGE)
    {
        player->removeCoins(1);
    }
    player->removeCoins(3);
    game.remove_player_if_nececery(player);
    target->setProfitBlock(true);
}

void sanctionAction::undo()
{
}

coupAction::coupAction(Player *player, Player *target, Game &game) : Action(player, game, ActionType::COUP), target(target) {}
Player *coupAction::getTarget()
{
    return target;
}
void coupAction::execute()
{
    if (game.is_player_in_game(target) == false)
    {
        throw std::runtime_error("Player is already out of the game.");
    }
    player->removeCoins(7);
    game.kill(target);
}

void coupAction::undo()
{
    if (game.get_actions_list(player).empty())
    {
        throw std::runtime_error("No coup action found to undo.");
    }
    if (game.is_player_in_game(game.getPlayerIndex(target)) == true)
    {
        throw std::runtime_error("Player is already in the game.");
    }

    game.revive(target);
}

Player *disabel_arrestAction::getTarget()
{
    return target;
}
disabel_arrestAction::disabel_arrestAction(Player *player, Player *target, Game &game) : Action(player, game, ActionType::DISABLE_ARREST), target(target) {}

void disabel_arrestAction::execute()
{
    if (game.is_player_in_game(target) == false)
    {
        throw std::runtime_error("Player is already out of the game.");
    }
    if (player->getType() != PlayerType::SPY)
    {
        throw std::runtime_error("Player is not a Spy.");
    }
    target->setArrestBlock(true);
}

void disabel_arrestAction::undo()
{
}

bribeAction::bribeAction(Player *player, Game &game) : Action(player, game, ActionType::BRIBE) {}

void bribeAction::execute()
{
    game.setAnotherTurn(this, true);
    player->removeCoins(4);
}

void bribeAction::undo()
{
    if (game.get_actions_list(player).empty())
    {
        throw std::runtime_error("No bribe action found to undo.");
    }
    if (game.get_actions_list(player).size() > 1)
    {
        throw std::runtime_error("plyerolredy plaid his turncenot undo bribe action.");
    }

    game.setAnotherTurn(this, false);

}

investAction::investAction(Player *player, Game &game) : Action(player, game, ActionType::INVEST) {}

void investAction::execute()
{
    if (player->getCoins() < 3)
    {
        throw std::runtime_error("Not enough coins to invest.");
    }
    player->addCoins(3);
}

void investAction::undo()
{
}

disabel_coupAction::disabel_coupAction(Player *player, Game &game, coupAction *action) : Action(player, game, ActionType::UNDO_COUP), action(action) {}

void disabel_coupAction::execute()
{
    if (action)
        action->undo();
}

void disabel_coupAction::undo()
{
}

Player *seeCoinsOfAction::getTarget()
{
    return target;
}
seeCoinsOfAction::seeCoinsOfAction(Player *player, Player *target, Game &game) : Action(player, game, ActionType::SEE_COINS_OF), target(target) {}

void seeCoinsOfAction::execute()
{
    std::cout << target->getCoins() << std::endl;
}

void seeCoinsOfAction::undo()
{
}
