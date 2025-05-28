#include "GameController.hpp"
#include "Game.hpp"
#include <stdexcept>
#include <iostream>

GameController::GameController(Game &g) : game(g) {}

void GameController::gather(const std::string &PlayerName)
{
    Player *current = get_player(PlayerName);
    try
    {
        current->gather();
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}
std::string actionTypeToString(ActionType type)
{
    switch (type)
    {
    case ActionType::GATHER:
        return "Gather";
    case ActionType::TAX:
        return "Tax";
    case ActionType::COUP:
        return "Coup";
    case ActionType::BRIBE:
        return "Bribe";
    case ActionType::SANCTION:
        return "Sanction";
    case ActionType::ARREST:
        return "Arrest";
    case ActionType::DISABLE_ARREST:
        return "Disable Arrest";
    case ActionType::DISABLE_TAX:
        return "Disable Tax";
    case ActionType::SEE_COINS_OF:
        return "See Coins";
    case ActionType::INVEST:
        return "Invest";
    case ActionType::UNDO_COUP:
        return "Undo Coup";
    case ActionType::UNDO_BRIBE:
        return "Undo Bribe";
    case ActionType::UNDO_TAX:
        return "Undo Tax";
    case ActionType::DISABLE_GATHER:
        return "Disable Gather";
    default:
        return "Unknown";
    }
}
void GameController::tax(const std::string &PlayerName)
{
    Player *current = get_player(PlayerName);
    try
    {
        current->tax();
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}
void GameController::setMessageCallback(std::function<void(const std::string &)> callback)
{
    messageCallback = std::move(callback);
}

std::string GameController::winner()
{
    std::string winner;
    try
    {
        winner = game.winner();
        if (winner!="")
        {
            // messageCallback("Winner: " + winner);
        }

    }
    catch (const std::exception &e)
    {
        if (std::string(e.what()) == "No players left in the game.")
        {
            messageCallback("No players left in the game.");
            return "";
        }

    }

    return winner;
}

std::string GameController::get_current_player()
{
    std::string currentPlayer;
    try
    {
        currentPlayer = game.get_current_player()->getName();
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }

    return currentPlayer;
}
bool GameController::is_player_in_game(int player_index)
{
    if (player_index < 0 || player_index >= game.get_num_players())
    {
        messageCallback("Player index out of range.");
        return false;
    }

    bool res;
    try
    {
        res =  game.is_player_in_game(player_index);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }

    return res;
}

void GameController::add_player(const std::string &PlayerName)
{
        try
    {
    game.add_random_player(PlayerName);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}
void GameController::coup(const std::string &PlayerName, const std::string &targetName)
{
    Player *current = get_player(PlayerName);
    Player *target = get_player(targetName);
    if (!target || !game.is_player_in_game(target))
    {
        messageCallback("Invalid coup target");
    }
    try
    {
        current->coup(target);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}

void GameController::bribe(const std::string &PlayerName)
{
    Player *current = get_player(PlayerName);
    try
    {
        current->bribe();
        bribe_used = true; 
        
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}

void GameController::sanction(const std::string &PlayerName, const std::string &targetName)
{
    Player *current = get_player(PlayerName);
    Player *target = get_player(targetName);
    try
    {
        current->sanction(target);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}


void GameController::arrest(const std::string &PlayerName, const std::string &targetName)
{
    Player *current = get_player(PlayerName);
    Player *target = get_player(targetName);
    try
    {
        current->arrest(target);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}

void GameController::disable_tax(const std::string &PlayerName, const std::string &targetName)
{
    Player *current = get_player(PlayerName);
    Player *target = get_player(targetName);
    try
    {
        current->disabel_tax(target);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}

void GameController::disable_arrest(const std::string &PlayerName, const std::string &targetName)
{
    Player *current = get_player(PlayerName);
    Player *target = get_player(targetName);
    try
    {
        current->disabel_arrest(target);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}

int GameController::see_coins(const std::string &PlayerName, const std::string &targetName)
{
    Player *current = get_player(PlayerName);
    Player *target = get_player(targetName);
    try
    {
        return current->seeCoinsOf(target);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
    return -1;
}

void GameController::invest(const std::string &PlayerName)
{
    Player *current = get_player(PlayerName);
    try
    {
        current->invest();
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}

void GameController::undo_coup(const std::string &PlayerName, const std::string &targetName)
{
    Player *current = get_player(PlayerName);
    Player *target = get_player(targetName);
    try
    {
        current->undo_coup(target);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}

void GameController::undo_bribe(const std::string &PlayerName, const std::string &targetName)
{
    Player *current = get_player(PlayerName);
    Player *target = get_player(targetName);
    try
    {
        current->BlockBribe(target);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}

void GameController::undo_tax(const std::string &PlayerName, const std::string &targetName)
{
    Player *current = get_player(PlayerName);
    Player *target = get_player(targetName);
    try
    {
        current->undo_tax(target);
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}

std::vector<std::string> GameController::getAvailableActions() const
{

    std::vector<std::string> actions;

    for (const auto &action : game.getAvailableActions())
    {
        std::cout << "Available action: " << actionTypeToString(action) << std::endl;
        actions.push_back(actionTypeToString(action));
    }
    return actions;
}

Player *GameController::get_player(const std::string &PlayerName) const
{
    return game.get_player(PlayerName);
}

void GameController::start_game()
{
    try
    {
        game.start_game();
    }
    catch (const std::exception &e)
    {
        messageCallback(e.what());
    }
}

std::vector<PlayerDetails> GameController::getPlayersDetails() const
{
    std::vector<PlayerDetails> details;
    for (const auto &player : game.get_players())
    {
        Player *p = player;
        if (p)
        {
            std::cout << "Player: " << p->getName() << ", Type: " << toString(p->getType()) << ", Coins: " << p->getCoins() << std::endl;
            details.emplace_back(p->getName(), toString(p->getType()), p->getCoins());
        }
    }

    return details;
}

std::string toString(const PlayerType &type) {
    switch (type) {
        case PlayerType::MERCHANT: return "Merchant";
        case PlayerType::SPY: return "Spy";
        case PlayerType::GENERAL: return "General";
        case PlayerType::JUDGE: return "Judge";
        case PlayerType::BARON: return "Baron";
        case PlayerType::GOVERNOR: return "Governor";
        default: return "Unknown";
    }
}