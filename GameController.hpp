#pragma once
#include "Game.hpp"
#include "ActionType.hpp"
#include <string>
#include <vector>
#include "./include/Player.hpp"
#include "PlayerDetails.h"
#include <functional>
class GameController
{

private:
    Game &game;

    bool bribe_used = false; 
    Player *get_player(const std::string &playerName) const;
    std::function<void(const std::string &)> messageCallback;

public:
    GameController(Game &game);


    void start_game();
    void add_player(const std::string &playerName);
    void gather(const std::string &playerName);
    void tax(const std::string &playerName);
    void coup(const std::string &playerName, const std::string &targetName);
    void bribe(const std::string &playerName);
    void sanction(const std::string &playerName, const std::string &targetName);
    void arrest(const std::string &playerName, const std::string &targetName);
    void disable_tax(const std::string &playerName, const std::string &targetName);
    void disable_arrest(const std::string &playerName, const std::string &targetName);
    int see_coins(const std::string &playerName, const std::string &targetName);
    void invest(const std::string &playerName);
    void undo_coup(const std::string &playerName, const std::string &targetName);
    void undo_bribe(const std::string &playerName, const std::string &targetName);
    void undo_tax(const std::string &playerName, const std::string &targetName);

    std::string winner();
    std::string get_current_player();
    bool is_player_in_game(int player_index);

    std::vector<std::string> getAvailableActions() const;
    std::vector<PlayerDetails> getPlayersDetails() const;

    void setMessageCallback(std::function<void(const std::string &)> callback);
};

std::string toString(const PlayerType &type);
