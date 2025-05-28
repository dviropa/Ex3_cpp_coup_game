#include "Game.hpp"
#include <memory>

void Game::start_game()
{
    if (num_players < MIN_PLAYERS || num_players > MAX_PLAYERS)
    {
        throw std::invalid_argument("Number of players must be between 2 and 6.");
    }
    game_started = true;

    current_turn = 0;
    num_actions = 1;
    actionsList.resize(num_players);
    for (size_t i = 0; i < num_players; ++i)
    {
        actionsList[i].resize(2);
        actionsList[i][0] = std::vector<Action *>();
        actionsList[i][1] = std::vector<Action *>();
    }
    apdate_available_actions();
 
}

void Game::updateBlocks(size_t plyer_index)
{
    bool ProfitBlock = false;
    bool arrestBlocked = false;

    int previousRound = round_number - 1;

    for (size_t i = 0; i < num_players; ++i)
    {
        int rund=previousRound;
        if(actionsList[i][rund+1].size()!=0){
            rund+=1;
        }
        for (Action *a : actionsList[i][rund])
        {
            switch (a->getType())
            {
            case ActionType::SANCTION:
            {
                auto *block = dynamic_cast<sanctionAction *>(a);
                if (block && block->getTarget() == get_player(plyer_index))
                    ProfitBlock = true;
                break;
            }
            case ActionType::DISABLE_ARREST:
            {
                auto *block = dynamic_cast<disabel_arrestAction *>(a);
                if (block && block->getTarget() == get_player(plyer_index))
                    arrestBlocked = true;
                break;
            }
            default:
                break;
            }
        }
    }
    Player *p = get_player(plyer_index);
    p->setProfitBlock(ProfitBlock);
    p->setArrestBlock(arrestBlocked);
}

void Game::end_turn()
{

    num_actions--;

    if (num_actions > 0)
    {
        return;
    }

    current_turn = (current_turn + 1) % num_players;
    actionsList[current_turn].push_back(std::vector<Action *>());

    while (active_players[current_turn] == false)
    {
        current_turn = (current_turn + 1) % num_players;
        actionsList[current_turn].push_back(std::vector<Action *>());
    }
        while (players_vec[current_turn]->getCoins() < 0 && active_players[current_turn])
    {
        remove_player_if_nececery(current_turn);
        current_turn = (current_turn + 1) % num_players; 
        actionsList[current_turn].push_back(std::vector<Action *>());
    }

    num_actions = 1;

    apdate_available_actions();
}
void Game::apdate_available_actions()
{
        available_actions.clear();

    if (get_current_player()->getType() == PlayerType::GOVERNOR )
    {
        available_actions.push_back(ActionType::DISABLE_TAX);
    }
    if (get_current_player()->getType() == PlayerType::BARON && players_vec[current_turn]->getCoins() >= 3)
    {
        available_actions.push_back(ActionType::INVEST);
    }
    if (get_current_player()->getType() == PlayerType::GENERAL && players_vec[current_turn]->getCoins() >= 5)
    {
        available_actions.push_back(ActionType::UNDO_COUP);
    }
    if (get_current_player()->getType() == PlayerType::JUDGE)
    {
        available_actions.push_back(ActionType::UNDO_BRIBE);
    }
    if (get_current_player()->getType() == PlayerType::SPY)
    {
        available_actions.push_back(ActionType::SEE_COINS_OF);
        available_actions.push_back(ActionType::DISABLE_ARREST);
    }

    if (get_current_player()->getType() == PlayerType::MERCHANT && players_vec[current_turn]->getCoins() >= 3)
    {
        players_vec[current_turn]->coins += 1;
    }
    updateBlocks(current_turn);
    if (get_current_player()->profit_block == false)
    {
        available_actions.push_back(ActionType::GATHER);
        available_actions.push_back(ActionType::TAX);
    }
    if (get_current_player()->arrest_block == false)
    {
        available_actions.push_back(ActionType::ARREST);
    }
    if (players_vec[current_turn]->getCoins() >= 7)
    {
        available_actions.push_back(ActionType::COUP);
    }
    if (players_vec[current_turn]->getCoins() >= 4)
    {
        available_actions.push_back(ActionType::BRIBE);
    }
    if (players_vec[current_turn]->getCoins() >= 3)
    {
        available_actions.push_back(ActionType::SANCTION);
    }


    if (get_current_player()->getCoins() == 10)
    {
        available_actions.clear();
        available_actions.push_back(ActionType::COUP);
    }
}

Game::Game() : num_players(0), num_actions(1), current_turn(0), round_number(1) {}

std::vector<Action *> Game::get_actions_list(Player *player) const
{
    return actionsList[getPlayerIndex(player)][round_number];
}

void Game::add_player(Player *player)
{
    if (num_players > MAX_PLAYERS)
    {
        throw std::runtime_error("Maximum number of players reached.");
    }
    for (Player* p : players_vec) {
        if (p->getName() == player->getName()) {
            throw std::runtime_error("Cannot have the same name twice");
        }
    }


    if (game_started)
    {
        throw std::runtime_error("Cannot add players after the game has started.");
    }

    players_vec.push_back(player);
    num_players++;
    active_players.push_back(true); 
    actionsList.push_back(std::vector<std::vector<Action *>>());
}

void Game::add_random_player(std::string name)
{
    if (num_players >= MAX_PLAYERS)
    {
        throw std::runtime_error("Maximum number of players reached.");
    }
    for (Player* p : players_vec) {
        if (p->getName() == name) {
            throw std::runtime_error("Cannot have the same name twice");
        }
    }

    if (game_started)
    {
        throw std::runtime_error("Cannot add players after the game has started.");
    }

    PlayerType randomType = getRandomPlayerType();
    Player *newPlayer = nullptr;
    switch (randomType)
    {
    case PlayerType::MERCHANT:
        newPlayer = new Merchant(name, *this);
        break;
    case PlayerType::SPY:
        newPlayer = new Spy(name, *this);
        break;
    case PlayerType::GENERAL:
        newPlayer = new General(name, *this);
        break;
    case PlayerType::JUDGE:
        newPlayer = new Judge(name, *this);
        break;
    case PlayerType::BARON:
        newPlayer = new Baron(name, *this);
        break;
    case PlayerType::GOVERNOR:
        newPlayer = new Governor(name, *this);
        break;
    }

    players_vec.push_back(newPlayer);
    num_players++;
    active_players.push_back(true);
    actionsList.push_back(std::vector<std::vector<Action *>>());
}

Game::~Game()
{
    for(Action* action : aList){
        delete action;
    }
    players_vec.clear();
    active_players.clear();
    actionsList.clear();
    num_players = 0;
    current_turn = 0;
    num_actions = 0;
    round_number = 0;
    // another_turn = false;
    game_started = false;
    available_actions.clear();
}

bool Game::is_player_in_game(size_t player_index) const
{
    if (player_index >= num_players)
    {
        throw std::out_of_range("Player index out of range.");
    }
    return active_players[player_index];
}
bool Game::is_player_in_game(Player *player) const
{
    return active_players[getPlayerIndex(player)];
}
int Game::get_num_players() const
{
    return num_players;
}

Player *Game::get_current_player() const
{
    return players_vec[current_turn];
}

int Game::get_current_turn() const
{
    return current_turn;
}
Player *Game::get_player(size_t index) const
{
    if (index >= num_players)
    {
        throw std::out_of_range("Player index out of range.");
    }
    return players_vec[index];
}
const std::vector<ActionType>& Game::getAvailableActions() const {
    return available_actions;
}

Player* Game::get_player(std::string name) const{
    for (size_t i = 0; i < num_players; ++i)
    {
        if (players_vec[i]->getName() == name)
        {
            return players_vec[i];
        }
    }
    throw std::invalid_argument(name+" not found in the game.");
}

int Game::getPlayerIndex(Player *player) const

{
    for (size_t i = 0; i < num_players; ++i)
    {
        if (players_vec[i] == player)
        {
            return i;
        }
    }
    throw std::invalid_argument("Player not found in the game.");
}
void Game::invest(Player *player)
{
    try{
        if (!isAllowed(ActionType::INVEST, player))
        {
            throw std::runtime_error("Player is not allowed to invest.");
        }
        if (!another_turn())
        {
            throw std::runtime_error("Player is aut of action for this turn.");
        }
        auto *action = new investAction(player, *this);
        aList.push_back(action);
        action->execute();
        actionsList[getPlayerIndex(player)][round_number].push_back(action);
        this->end_turn();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }


}

void Game::remove_player_if_nececery(Player *player)
{
    if (player->getCoins() < 0)
    {
        active_players[getPlayerIndex(player)] = false;
    }
}
void Game::remove_player_if_nececery(size_t player_index)
{
    
        if (player_index > num_players)
        {
            throw std::out_of_range("Player index out of range.");
        }
        remove_player_if_nececery(get_player(player_index));

}

void Game::gather(Player *player)
{
    try
    {
        if (!isAllowed(ActionType::GATHER, player))
        {
            throw std::runtime_error("Player is not allowed to gather.");
        }
        if (num_actions <= 0)
        {
            throw std::runtime_error("Player is aut of action for this turn.");
        }

        auto *action = new GatherAction(player, *this);
        aList.push_back(action);

        action->execute();
        actionsList[getPlayerIndex(player)][round_number].push_back(action);


        
        this->end_turn();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(e.what());
        std::cerr << e.what() << '\n';
    }
}
bool Game::another_turn()
{
    return num_actions > 0;
}
void Game::bribe(Player *player)
{
    try
    {
        if (!isAllowed(ActionType::BRIBE, player))
        {
            throw std::runtime_error("Player is not allowed to bribe.");
        }
        if (!another_turn())
        {
            throw std::runtime_error("Player is aut of action for this turn.");
        }

        auto *action = new bribeAction(player, *this);
        aList.push_back(action);

        action->execute();
        actionsList[getPlayerIndex(player)][round_number].push_back(action);
        num_actions++;
        this->end_turn();
    }
    catch (const std::exception &e)
    {
                throw std::runtime_error(e.what());
        std::cerr << e.what() << '\n';
    }
}
int Game::seeCoinsOf(Player *player,Player *target)
{
    try
    {
        if (isAllowed(ActionType::SEE_COINS_OF, player))
        {
            auto *action = new seeCoinsOfAction(player, target, *this);
                    aList.push_back(action);

            action->execute();
            actionsList[getPlayerIndex(player)][round_number].push_back(action);
        }
        else
            throw std::runtime_error("Player is not allowed to see coins of another player.");
    }
    catch (const std::exception &e)
    {
                throw std::runtime_error(e.what());

        std::cerr << e.what() << '\n';
    }

    return target->getCoins();
}

void Game::arrest(Player *player,Player *target)
{
    try
    {
        if (!isAllowed(ActionType::ARREST, player))
        {
            throw std::runtime_error("Player is not allowed to arrest.");
        }

        if (!another_turn())
        {
            throw std::runtime_error("Player is aut of action for this turn.");
        }
        if (actionsList[getPlayerIndex(player)][round_number].size() == 0)
            for (Action *a : actionsList[getPlayerIndex(player)][round_number - 1])
            {
                if (a->getType() == ActionType::ARREST)
                {
                    auto *arrest = dynamic_cast<arrestAction *>(a);
                    if (arrest->getTarget() == target)
                        throw std::runtime_error("cenot arrest the same player twice in a row.");
                }
            }
        else
            for (Action *a : actionsList[getPlayerIndex(player)][round_number])
            {
                if (a->getType() == ActionType::ARREST)
                {
                    auto *arrest = dynamic_cast<arrestAction *>(a);
                    if (arrest->getTarget() == target)
                        throw std::runtime_error("cenot arrest the same player twice in a row.");
                }
            }

        auto *action = new arrestAction(player, target, *this);
                aList.push_back(action);

        action->execute();
        actionsList[getPlayerIndex(player)][round_number].push_back(action);
        this->end_turn();
    }
    catch (const std::exception &e)
    {
                throw std::runtime_error(e.what());

        std::cerr << e.what() << '\n';
    }
}
void Game::sanction(Player *player,Player *target)
{
    try
    {
        if (!isAllowed(ActionType::SANCTION, player))
        {
            throw std::runtime_error("Player is not allowed to sanction.");
        }

        if (!another_turn())
        {
            throw std::runtime_error("Player is aut of action for this turn.");
        }
        auto *action = new sanctionAction(player, target, *this);
                aList.push_back(action);

        action->execute();
        actionsList[getPlayerIndex(player)][round_number].push_back(action);
        this->end_turn();
    }
    catch (const std::exception &e)
    {
                throw std::runtime_error(e.what());

        std::cerr << e.what() << '\n';
    }
}

void Game::coup(Player *player,Player *target)
{

    try
    {
        if (!isAllowed(ActionType::COUP, player))
        {
            throw std::runtime_error("Player is not allowed to coup.");
        }

        if (!another_turn())
        {
            throw std::runtime_error("Player is aut of action for this turn.");
        }
        auto *action = new coupAction(player, target, *this);
                aList.push_back(action);

        action->execute();
        actionsList[getPlayerIndex(player)][round_number].push_back(action);
        this->end_turn();
    }
    catch (const std::exception &e)
    {
                throw std::runtime_error(e.what());

        std::cerr << e.what() << '\n';
    }
}

std::string Game::turn() const
{
    return players_vec[current_turn]->getName();
}

std::vector<std::string> Game::players() const
{
    std::vector<std::string> playerNames;
    for (size_t i = 0; i < num_players; ++i)
    {
        if (active_players[i])
        {
            playerNames.push_back(players_vec[i]->getName());
        }
    }
    return playerNames;
}

std::string Game::winner() const
{
    int alive_count = 0;
    int index = -1;
    for (size_t i = 0; i < num_players; ++i)
    {
        if (active_players[i])
        {
            alive_count++;
            index = i;
        }
    }
    if (alive_count == 1)
    {
        return players_vec[index]->getName();
    }
    else if (alive_count == 0)
    {
        throw std::runtime_error("No players left in the game.");
    }
    
    return ""; 
}
void Game::disabel_tax(Player *player,Player *target)
{
    try
    {
        if (!isAllowed(ActionType::DISABLE_TAX, player))
        {
            throw std::runtime_error("Player is not allowed to disabel tax.");
        }
        if (!another_turn())
        {
            throw std::runtime_error("Player is aut of action for this turn.");
        }

        if (actionsList[getPlayerIndex(target)][round_number].size() == 0)
        {
            for (Action *a : actionsList[getPlayerIndex(target)][round_number - 1])
            {
                if (a->getType() == ActionType::TAX)
                {
                    TaxAction *tax = dynamic_cast<TaxAction *>(a);
                    if (tax != nullptr)
                    {
                        auto *action = new disabel_taxAction(player, *this, tax);
                                aList.push_back(action);

                        action->execute();
                        auto &vec = actionsList[getPlayerIndex(target)][round_number];
                        auto it = std::find(vec.begin(), vec.end(), a);
                        if (it != vec.end())
                        {
                            vec.erase(it);
                        }
                        actionsList[getPlayerIndex(player)][round_number].push_back(action);
                        this->end_turn();
                    }
                }
            }
        }
        else if (actionsList[getPlayerIndex(target)][round_number].size() != 0)
        {
            for (Action *a : actionsList[getPlayerIndex(target)][round_number])
            {
                if (a->getType() == ActionType::TAX)
                {
                    TaxAction *tax = dynamic_cast<TaxAction *>(a);
                    if (tax != nullptr)
                    {
                        auto *action = new disabel_taxAction(player, *this, tax);
                                aList.push_back(action);

                        action->execute();
                        auto &vec = actionsList[getPlayerIndex(target)][round_number];
                        auto it = std::find(vec.begin(), vec.end(), a);
                        if (it != vec.end())
                        {
                            vec.erase(it);
                        }
                        actionsList[getPlayerIndex(player)][round_number].push_back(action);
                        this->end_turn();
                    }
                }
            }
        }

        else
            throw std::runtime_error("Player has not used the tax action.");
    }
    catch (const std::exception &e)
    {
                throw std::runtime_error(e.what());
        std::cerr << e.what() << '\n';
    }
}
std::vector<Player*> Game::get_players() const{
    return players_vec;
}
void Game::tax(Player *player,int amount)
{
    
    try
    {
        if (!isAllowed(ActionType::TAX, player))
        {
            throw std::runtime_error("Player is not allowed to tax.");
        }


        if (!another_turn())
        {
            throw std::runtime_error("Player is aut of action for this turn.");
        }

        auto *action = new TaxAction(player, *this, amount);
                aList.push_back(action);

                        try {
            action->execute();  
        } catch (...) {
            delete action;     
            throw;
        }
        actionsList[getPlayerIndex(player)][round_number].push_back(action);

        this->end_turn();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(e.what());

    }
}
void Game::undo_coup(Player *player,Player *target)
{
    
    try
    {
        if (!isAllowed(ActionType::UNDO_COUP, player))
        {
            throw std::runtime_error("cenot UNDO COUP .");
        }
        bool found = false;
        if (actionsList[getPlayerIndex(target)][round_number].size() == 0)
            for (Action *a : actionsList[getPlayerIndex(target)][round_number - 1])
            {
            if (a->getType() == ActionType::COUP)
            {
                coupAction *tax = dynamic_cast<coupAction *>(a);
                if (tax != nullptr)
                {
                    auto *action = new disabel_coupAction(player, *this, tax);
                            aList.push_back(action);

                    action->execute();
                    found = true;
                    actionsList[getPlayerIndex(player)][round_number].push_back(action);
                    this->end_turn();
                }
            }
            }
        else
            for (Action *a : actionsList[getPlayerIndex(target)][round_number])
            {
           if (a->getType() == ActionType::COUP)
            {
                coupAction *tax = dynamic_cast<coupAction *>(a);
                if (tax != nullptr)
                {
                    auto *action = new disabel_coupAction(player, *this, tax);
                            aList.push_back(action);

                    action->execute();
                    found = true;
                    actionsList[getPlayerIndex(player)][round_number].push_back(action);
                    this->end_turn();
                }
            }
            }
        if (!found)
        {
            throw std::runtime_error("Player has not used the coup action.");
        }
    }
    catch (const std::exception &e)
    {
                throw std::runtime_error(e.what());

        std::cerr << e.what() << '\n';
    }
}

void Game::undo_bribe(Player *player,Player *target)
{
    try
    {
        if (player->getType() != PlayerType::JUDGE)
        {
            throw std::runtime_error("Player is not a Judge.");
        }
        if(target!= get_current_player())
        {
            throw std::runtime_error("Player is not allowed to undo bribe for another player.");
        }

        bool found = false;
        auto &vec = actionsList[getPlayerIndex(target)][round_number];

        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            if ((*it)->getType() == ActionType::BRIBE)
            {
                if (&(*it) == &vec.back()) {
                    found = true;
                    (*it)->undo();
                    delete *it;
                    vec.erase(it);
                    return;
                } else {
                    found = true;
                    throw std::runtime_error("Cannot undo bribe: other actions were already performed after it.");
                }
            }
        }

        if (!found)
            throw std::runtime_error("Player has not used the bribe action.");
    }
    catch (const std::exception &e)
    {
                throw std::runtime_error(e.what());

        std::cerr << e.what() << '\n';
    }
}

void Game::disabel_arrest(Player *player,Player *target)
{
    try
    {
        if (!isAllowed(ActionType::DISABLE_ARREST, player))
        {
            throw std::runtime_error("Player is not allowed to disabel arrest.");
        }
        if (!another_turn())
        {
            throw std::runtime_error("Player is aut of action for this turn.");
        }
        auto *action = new disabel_arrestAction(player, target, *this);
                aList.push_back(action);

        action->execute();
        actionsList[getPlayerIndex(player)][round_number].push_back(action);
    }
    catch (const std::exception &e)
    {
                throw std::runtime_error(e.what());

        std::cerr << e.what() << '\n';
    }
}


PlayerType Game::getRandomPlayerType()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    constexpr int numTypes = 6; 
    std::uniform_int_distribution<> dist(0, numTypes - 1);

    return static_cast<PlayerType>(dist(gen));
}
bool Game::isBlocked(ActionType a)
{
    return std::find(available_actions.begin(), available_actions.end(), a) != available_actions.end();
}

int Game::getRoundNumber() const
{
    return round_number;
}

void Game::setAnotherTurn(Action *action, bool another_turn = false)
{

        if (another_turn)
        {
            this->num_actions++;
        }
        else
        {
            this->num_actions--;
        }

}

std::vector<std::vector<Action *>> &Game::getAllActionsList(Player *player)
{
    return actionsList[getPlayerIndex(player)];
}

void Game::kill(Player *player)
{

    active_players[getPlayerIndex(player)] = false;
}

void Game::revive(Player *player)
{
    active_players[getPlayerIndex(player)] = true;
}

bool Game::isAllowed(ActionType a, Player *player)
{
    if (get_current_player() != player)
    {
        throw std::runtime_error("Not " + player->getName() + "'s turn.");
    }
    if (player->getType() == PlayerType::MERCHANT && players_vec[getPlayerIndex(player)]->getCoins() >= 3)
    {
        player->addCoins(1);
    }
    if (player->getCoins() == 10 && actionsList[getPlayerIndex(player)][round_number].size() == 0)
    {
        if (a != ActionType::COUP)
        {
            throw std::runtime_error("Player has 10 coins and must coup.");
        }
        else
        {
            return true;
        }
    }

    switch (a)
    {
    case ActionType::GATHER:
    {
        updateBlocks(getPlayerIndex(player));
        if (player->profit_block == false)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    case ActionType::TAX:
    {
        updateBlocks(getPlayerIndex(player));
        if (player->profit_block == false)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    case ActionType::ARREST:
    {
        updateBlocks(getPlayerIndex(player));
        if (player->arrest_block == false)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    case ActionType::DISABLE_TAX:
    {
        if ((player->getType() == PlayerType::GOVERNOR ))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    case ActionType::INVEST:
    {
        if (player->getType() == PlayerType::BARON && players_vec[getPlayerIndex(player)]->getCoins() >= 3)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    case ActionType::COUP:
    {
        if (player->getCoins() >= 7)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    case ActionType::UNDO_COUP:
    {
        if (player->getType() == PlayerType::GENERAL && players_vec[getPlayerIndex(player)]->getCoins() >= 5)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    case ActionType::BRIBE:
    {
        if (player->getCoins() >= 4)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    case ActionType::UNDO_BRIBE:
    {
        if (player->getType() == PlayerType::JUDGE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    case ActionType::SEE_COINS_OF:
    {
        if (player->getType() == PlayerType::SPY)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    case ActionType::SANCTION:
    {
        if (player->getCoins() >= 3)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    case ActionType::DISABLE_ARREST:
    {
        if (player->getType() == PlayerType::SPY)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    default:
        return false;
    }
}


