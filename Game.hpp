#pragma once

#include <vector>
#include <list>
#include <map>
#include <stdexcept>
#include <random>
#include <memory>
#include "./include/Player.hpp"
#include "Action.hpp"
#include "./include/Merchant.hpp"
#include "./include/Spy.hpp"
#include "./include/General.hpp"
#include "./include/Judge.hpp"
#include "./include/Baron.hpp"
#include "./include/Governor.hpp"


#include "GameObserver.hpp"
#include "GameUpdate.hpp"

#define MAX_PLAYERS 6
#define MIN_PLAYERS 2

class Player;       
class Action;         
enum class PlayerType; 
enum ActionType;       


class Game
{
private:
    size_t num_players;
    int num_actions;

    std::vector<ActionType> available_actions;
    std::vector<Player *> players_vec;
    std::vector<bool> active_players;
    std::vector<std::vector<std::vector<Action *>>> actionsList;
    std::vector<Action *> aList;

    int current_turn;
    int round_number;


    bool game_started = false;

    PlayerType getRandomPlayerType();
    void updateBlocks(size_t plyer_index);
    bool  another_turn();
    void apdate_available_actions();
public:
const std::vector<ActionType>& getAvailableActions() const;
std::vector<Player*> get_players() const;
    std::string turn() const;
    std::vector<std::string> players() const;
    std::string winner() const;
    Game();
    ~Game();

    void disabel_tax(Player* player, Player* target);

    void tax(Player *player,int amount);

    std::vector<Action *> get_actions_list(Player *player) const;
    void add_player(Player *player);
    void add_random_player(std::string name);
    void remove_player(Player *player);
    void end_turn();
    void start_game();
    bool is_player_in_game(size_t player_index) const;
    bool is_player_in_game(Player *player) const;
    int get_num_players() const;
    int get_current_turn() const;
    Player *get_current_player() const;
    Player *get_player(size_t index) const;
    Player *get_player(std::string name) const;
    int getPlayerIndex(Player *player) const;

    int seeCoinsOf(Player *player,Player *target);
    void remove_player_if_nececery(Player *player);
    void remove_player_if_nececery(size_t player_index);
    void invest(Player *player);
    void gather(Player *player);
    void bribe(Player *player);
    void arrest(Player *player,Player *target);
    void disabel_arrest(Player *player,Player *target);
    void sanction(Player *player,Player *target);
    void coup(Player *player,Player *target);
    void undo_coup(Player *player,Player *target);
    void undo(Player *player,Player *target);
    void undo_bribe(Player *player,Player *target);
    void kill(Player *player);
    void revive(Player *player);

    int getRoundNumber() const;

    std::vector<std::vector<std::vector<Action *>>> &getActionsList();
    std::vector<std::vector<Action *>> &getAllActionsList(Player *player);

    bool isBlocked(ActionType a);
    void setAnotherTurn(Action *action, bool another_turn);
bool isAllowed(ActionType a ,Player *player);
std::vector<ActionType> getPlayersActionsList(Player *player);
};
