#include "GameGUI.hpp"
#include <iostream>

GameGUI::GameGUI(GameController *controller) : controller(controller), window(sf::VideoMode(800, 600), "Coup Game"), currentState(State::Welcome)
{
    GameGUI::instance = this;

        if (!font.loadFromFile("ARIAL.TTF"))
    {
        std::cerr << "Failed to load font!\n";
    }

    // Prepare welcome screen input
    inputBox.setSize(sf::Vector2f(200, 30));
    inputBox.setPosition(300, 200);
    inputBox.setFillColor(sf::Color::White);

    inputText.setFont(font);
    inputText.setCharacterSize(16);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(305, 205);

    addButton.setSize(sf::Vector2f(100, 30));
    addButton.setPosition(510, 200);
    addButton.setFillColor(sf::Color::Green);

    addLabel.setFont(font);
    addLabel.setCharacterSize(14);
    addLabel.setString("Add Player");
    addLabel.setFillColor(sf::Color::Black);
    addLabel.setPosition(515, 205);

    startButton.setSize(sf::Vector2f(100, 30));
    startButton.setPosition(350, 400);
    startButton.setFillColor(sf::Color::Yellow);

    startLabel.setFont(font);
    startLabel.setCharacterSize(14);
    startLabel.setString("Start Game");
    startLabel.setFillColor(sf::Color::Black);
    startLabel.setPosition(360, 405);
}

void GameGUI::initPlayerPanel()
{
    UndoBribeButtons.clear();

    playerTexts.clear();
    int y = 20;

    std::string currentPlayerName = controller->get_current_player();

    for (size_t i = 0; i < players.size(); ++i)
    {
        auto &p = players[i];

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        text.setPosition(20, y);
        bool isActive = controller->is_player_in_game(i);



        if (p.name == currentPlayerName)
        {
            text.setString(p.name + " (" + p.role + ") - Coins: " + std::to_string(p.coins));
            playerTexts.push_back(text);

        }
        else if (!isActive)
        {
            text.setString(p.name +"     - Not in the game");
            playerTexts.push_back(text);

        }
        else{
            text.setString(p.name);
            playerTexts.push_back(text);

        }



        if (bribe_used)
        {
            ActionButton undoBribeBtn;
            undoBribeBtn.action = "Undo Bribe";
            undoBribeBtn.box.setSize(sf::Vector2f(120, 30));
            undoBribeBtn.box.setPosition(200, y);
            undoBribeBtn.box.setFillColor(sf::Color::Red);
            undoBribeBtn.label.setFont(font);
            undoBribeBtn.label.setCharacterSize(14);
            undoBribeBtn.label.setString(undoBribeBtn.action);
            undoBribeBtn.label.setFillColor(sf::Color::White);
            undoBribeBtn.label.setPosition(210, y + 5);

            UndoBribeButtons.push_back(undoBribeBtn);
        }

        y += 40;
    }
}

void GameGUI::setMessageCallback(std::function<void(const std::string &)> callback) {
    messageCallback = std::move(callback);
}
GameGUI* GameGUI::instance = nullptr;


void GameGUI::updateActionButtons()
{
    actionButtons.clear();
    auto available = controller->getAvailableActions();

    int y = 20;

    for (const auto &act : available)
    {
        ActionButton btn;
        btn.action = act;
        btn.box.setSize(sf::Vector2f(150, 30));
        btn.box.setPosition(600, y);
        btn.box.setFillColor(sf::Color::Blue);

        btn.label.setFont(font);
        btn.label.setCharacterSize(14);
        btn.label.setString(btn.action);
        btn.label.setFillColor(sf::Color::White);
        btn.label.setPosition(610, y + 5);

        actionButtons.push_back(btn);
        y += 40;
    }
}

void GameGUI::drawPlayerPanel()
{
    for (const auto &t : playerTexts)
    {
        window.draw(t);
    }

    for (const auto &btn : UndoBribeButtons)
    {
        window.draw(btn.box);
        window.draw(btn.label);
    }
}

void GameGUI::drawActionPanel()
{
    for (const auto &btn : actionButtons)
    {
        window.draw(btn.box);
        window.draw(btn.label);
    }
}

std::string GameGUI::getActionClicked(sf::Vector2f mousePos)
{
    for (const auto &btn : actionButtons)
    {
        if (btn.box.getGlobalBounds().contains(mousePos))
            return btn.action;
    }

    int count = 0;
    for (const auto &btn : UndoBribeButtons)
    {
        if (btn.box.getGlobalBounds().contains(mousePos))
        {
            blocker_player = players[count].name; 
            return btn.action;
        }
        count++;
    }

    return "";
}

void GameGUI::onGameStateChanged(const GameUpdate &update)
{
    // Handle game state changes here
}

void GameGUI::run()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (currentState == State::Welcome)
            {
                if (event.type == sf::Event::TextEntered && event.text.unicode < 128)
                {
                    if (event.text.unicode == '\b' && !playerInput.empty())
                    {
                        playerInput.pop_back();
                    }
                    else if (event.text.unicode != '\b')
                    {
                        playerInput += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(playerInput);
                }
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (addButton.getGlobalBounds().contains(mousePos) && !playerInput.empty())
                    {
                        if (players.size() >= 6)
                        {
                            std::cerr << "Cannot add more than 6 players.\n";
                        }
                        else
                        {

                            try
                            {
                                controller->add_player(playerInput);
                            }
                            catch (const std::exception &e)
                            {
                                std::cerr << "Error adding player: " << e.what() << "\n";
                            }

                            catch (const std::exception &e)
                            {
                                std::cerr << "Error adding player: " << e.what() << "\n";
                            }
                            playerInput.clear();
                            inputText.setString("");
                        }
                    }

                    else if (startButton.getGlobalBounds().contains(mousePos))
                    {
                        updatePlayerDetails();

                        if (players.size() < 2)
                        {
                            std::cerr << "At least 2 players required to start the game.\n";
                        }
                        else
                        {

                            try
                            {
                                controller->start_game();
                            }
                            catch (const std::exception &e)
                            {
                                std::cerr << "Error starting game: " << e.what() << "\n";
                            }
                            currentState = State::Game;
                            initPlayerPanel();
                            updateActionButtons();
                        }
                    }
                }
            }
            else if (currentState == State::Game)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    std::string action = getActionClicked(mousePos);
                    if (!action.empty())
                    {
                        std::cout << "Action Clicked: " << action << std::endl;
                        bribe_used = false; 

                        if (controller != nullptr)
                        {
                            std::string current = controller->get_current_player();
                            if (action == "Gather")
                            {
                                std::cout << "Gathering coins for " << current << std::endl;
                                controller->gather(current);
                            }
                            else if (action == "Tax")
                            {
                                controller->tax(current);
                            }
                            else if (action == "Coup")
                            {

                                std::string target = findTarget();
                                controller->coup(current, target);
                            }
                            else if (action == "Bribe")
                            {
                                controller->bribe(current);
                                bribe_used = true; // Set the flag to true when bribe is used
                            }
                            else if (action == "Sanction")
                            {
                                std::string target = findTarget();
                                controller->sanction(current, target);
                            }
                            else if (action == "Arrest")
                            {
                                std::string target = findTarget();
                                controller->arrest(current, target);
                            }
                            else if (action == "Disable Tax")
                            {
                                std::string target = findTarget();
                                controller->disable_tax(current, target);
                            }
                            else if (action == "Disable Arrest")
                            {
                                std::string target = findTarget();
                                controller->disable_arrest(current, target);
                            }
                            else if (action == "See Coins")
                            {
                                std::string target = findTarget();
                                int coins = controller->see_coins(current, target);
                                displayMessage("Coins of " + target + ": " + std::to_string(coins));
                            }
                            else if (action == "Invest")
                            {
                                controller->invest(current);
                            }
                            else if (action == "Undo Coup")
                            {
                                std::string target = findTarget();
                                controller->undo_coup(current, target);
                            }
                            else if (action == "Undo Bribe")
                            {
                                std::cout << "Undoing bribe for " << current << std::endl;
                                controller->undo_bribe(blocker_player, current);
                            }
                            else if (action == "Undo Tax")
                            {
                                std::string target = findTarget();
                                controller->undo_tax(current, target);
                            }

                            updatePlayerDetails();

                            try
                            {
                                std::string winnerName = controller->winner();
                                    if (winnerName!="")
                                    {
                                        showWinnerWindow("Winner: " + winnerName);
                                    }
                            }
                            catch (const std::exception &e)
                            {
                                if (std::string(e.what()) == "No winner yet")
                                {
                                    // No action needed
                                }
                                else
                                {
                                    // displayMessage(e.what());
                                }
                            }
                            updateActionButtons();
                            initPlayerPanel();
                        }
                    }
                }
            }
        }

        window.clear();
        if (currentState == State::Welcome)
        {
            window.draw(inputBox);
            window.draw(inputText);
            window.draw(addButton);
            window.draw(addLabel);
            window.draw(startButton);
            window.draw(startLabel);
        }
        else if (currentState == State::Game)
        {
            drawPlayerPanel();
            drawActionPanel();
        }
        window.display();
    }
}
void GameGUI::showWinnerWindow(const std::string &winnerName)
{
    sf::RenderWindow winnerWindow(sf::VideoMode(400, 200), "Game Over");

    sf::Text winText;
    winText.setFont(font);
    winText.setCharacterSize(24);
    winText.setFillColor(sf::Color::Green);
    winText.setString("Winner: " + winnerName);
    winText.setPosition(50, 80);

    while (winnerWindow.isOpen())
    {
        sf::Event event;
        while (winnerWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                winnerWindow.close();
        }

        winnerWindow.clear(sf::Color::Black);
        winnerWindow.draw(winText);
        winnerWindow.display();
    }

    messageText.setFont(font);
    messageText.setCharacterSize(18);
    messageText.setFillColor(sf::Color::Red);
    messageText.setPosition(20, 560);

    messageDisplayDuration = sf::seconds(3); // תצוגה ל־3 שניות

    window.close(); // סוגר גם את חלון המשחק הראשי
}

void GameGUI::displayMessage(const std::string &msg)
{

    sf::RenderWindow MessageWindow(sf::VideoMode(400, 200), "Message");

    sf::Text errorText;
    errorText.setFont(font);
    errorText.setCharacterSize(24);
    errorText.setFillColor(sf::Color::Red);
    errorText.setString(msg);
    errorText.setPosition(50, 80);

    while (MessageWindow.isOpen())
    {
        sf::Event event;
        while (MessageWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                MessageWindow.close();
        }

        MessageWindow.clear(sf::Color::Black);
        MessageWindow.draw(errorText);
        MessageWindow.display();
    }
}

std::string GameGUI::findTarget()
{
    sf::RenderWindow targetWindow(sf::VideoMode(300, 50 + 40 * players.size()), "Choose Target");

    std::vector<sf::RectangleShape> boxes;
    std::vector<sf::Text> labels;

    for (size_t i = 0; i < players.size(); ++i)
    {
        if (!controller->is_player_in_game(i))
            continue;
        if (players[i].name == controller->get_current_player())
            continue; // לא לבחור את עצמי

        sf::RectangleShape box(sf::Vector2f(200, 30));
        box.setFillColor(sf::Color::Cyan);
        box.setPosition(50, 20 + 40 * boxes.size());

        sf::Text label;
        label.setFont(font);
        label.setString(players[i].name);
        label.setCharacterSize(16);
        label.setFillColor(sf::Color::Black);
        label.setPosition(box.getPosition().x + 10, box.getPosition().y + 5);

        boxes.push_back(box);
        labels.push_back(label);
    }

    while (targetWindow.isOpen())
    {
        sf::Event event;
        while (targetWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                targetWindow.close();
                return ""; // ביטול
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos = targetWindow.mapPixelToCoords(sf::Mouse::getPosition(targetWindow));
                for (size_t i = 0; i < boxes.size(); ++i)
                {
                    if (boxes[i].getGlobalBounds().contains(mousePos))
                    {
                        targetWindow.close();
                        return labels[i].getString();
                    }
                }
            }
        }

        targetWindow.clear(sf::Color::White);
        for (size_t i = 0; i < boxes.size(); ++i)
        {
            targetWindow.draw(boxes[i]);
            targetWindow.draw(labels[i]);
        }
        targetWindow.display();
    }

    return "";
}

void GameGUI::showErrorMessage(const std::string &message)
{
    sf::RenderWindow errorWindow(sf::VideoMode(400, 200), "Error");

    sf::Text errorText;
    errorText.setFont(font);
    errorText.setCharacterSize(24);
    errorText.setFillColor(sf::Color::Red);
    errorText.setString(message);
    errorText.setPosition(50, 80);

    while (errorWindow.isOpen())
    {
        sf::Event event;
        while (errorWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                errorWindow.close();
        }

        errorWindow.clear(sf::Color::Black);
        errorWindow.draw(errorText);
        errorWindow.display();
    }
}

void GameGUI::updatePlayerDetails()
{
    players = controller->getPlayersDetails();
}
