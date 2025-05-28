#include "GameGUI.hpp"
#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <queue>
#include "GameGUI.hpp"
#include "GameController.hpp"

int main()
{
    Game game;
    GameController gameController(game);
    GameGUI gameGUI(&gameController);

    gameController.setMessageCallback([&gameGUI](const std::string &msg) {
        gameGUI.displayMessage(msg);
    });

    gameGUI.run();
    return 0;
}
