// GameGUI.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "ActionType.hpp"
#include "GameController.hpp"
#include "PlayerDetails.h"
#include "GameObserver.hpp"
#include "GameUpdate.hpp"
struct ActionButton {
    std::string action;
    sf::RectangleShape box;
    sf::Text label;
};





class GameGUI : public GameObserver {
    GameController * controller;

public:
    explicit GameGUI(GameController* controller);
    void run();
    void onGameStateChanged(const GameUpdate& update) override;
    void displayMessage(const std::string& message);
    void showErrorMessage(const std::string& message);
    void showWinnerWindow(const std::string& winnerName);
    void updatePlayerDetails();
    void setMessageCallback(std::function<void(const std::string &)> callback);
 static GameGUI* instance; 
void updateActionButtons() ;
private:
std::function<void(const std::string &)> messageCallback;


    std::string findTarget();

    sf::Text messageText;
    std::string currentMessage;
    sf::Clock messageTimer;          
    sf::Time messageDisplayDuration;
    
    // std::string actionTypeToString(ActionType type);


    enum class State { Welcome, Game };
    void initPlayerPanel();
    void drawPlayerPanel();
    void drawActionPanel();
    std::string getActionClicked(sf::Vector2f mousePos);

    sf::RenderWindow window;
    sf::Font font;

    // Welcome screen components
    State currentState;
    sf::RectangleShape inputBox;
    sf::Text inputText;
    std::string playerInput;

    sf::RectangleShape addButton;
    sf::Text addLabel;

    sf::RectangleShape startButton;
    sf::Text startLabel;

    std::vector<PlayerDetails> players;
    std::vector<sf::Text> playerTexts;
    std::vector<ActionButton> actionButtons;

    bool bribe_used = false;
    std::string blocker_player;
    std::vector<ActionButton> UndoBribeButtons;

};
