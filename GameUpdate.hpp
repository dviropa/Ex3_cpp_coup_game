#pragma once

#include <iostream>
#include <vector>


enum class GameUpdates{
    PLAYER_ADDED,
    COINS_UPDATED,
    PLAYER_REMOVED,
    GAME_STARTED,
    GAME_ENDED, 
    GAME_PAUSED,
};

class GameUpdate{
    private:
        std::string player;
        GameUpdates update;
        std::vector<std::string> details;
        std::string msg;
        
    public:
        GameUpdate(std::string player, GameUpdates update);
        GameUpdate(std::string player, GameUpdates update, std::vector<std::string> details);
        std::string getPlayer() const;
        GameUpdates getUpdate() const;
        std::vector<std::string> getDetails() const;    
};

