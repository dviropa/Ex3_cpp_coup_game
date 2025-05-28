
#include "./include/Player.hpp"
#include "./include/Governor.hpp"
#include "./include/Spy.hpp"
#include "./include/Baron.hpp"
#include "./include/General.hpp"
#include "./include/Judge.hpp"
#include "./include/Merchant.hpp"
#include "Game.hpp"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

int main() {
    Game game_1;

    Governor *governor = new Governor("Moshe" , game_1);
    Spy *spy = new Spy("Yossi", game_1);
    Baron *baron = new Baron("Meirav", game_1);
    General *general= new General("Reut", game_1);
    Judge *judge = new Judge("Gilad", game_1);
    
    game_1.add_player(governor);
    game_1.add_player(spy);
    game_1.add_player(baron);
    game_1.add_player(general);
    game_1.add_player(judge);
    game_1.start_game();
    vector<string> players = game_1.players();
    
    // Expected output:
    // Moshe
    // Yossi
    // Meirav
    // Reut
    // Gilad
    for(string name : players){
        cout << name << endl;
    }

    // Expected output: Moshe
    std::cout << "Before governor->gather()" << std::endl;
    cout << game_1.turn() << endl;
    governor->gather();
    cout << game_1.turn() << endl;
std::cout << "a governor->gather()" << std::endl;
    spy->gather();
    cout << game_1.turn() << endl;

    baron->gather();
    cout << game_1.turn() << endl;

    general->gather();
    cout << game_1.turn() << endl;

    judge->gather();
    cout << game_1.turn() << endl;


    cout << game_1.turn() << endl;
    governor->tax();
    cout << game_1.turn() << endl;
std::cout << "a governor->gather()" << std::endl;
    spy->tax();
    cout << game_1.turn() << endl;

    baron->tax();
    cout << game_1.turn() << endl;

    general->tax();
    cout << game_1.turn() << endl;

    judge->tax();
    cout << game_1.turn() << endl;


        cout << game_1.turn() << endl;
    // Expected exception - Not spy's turn
    try{    
            cout <<"????????????????????????????????????????????????????" << endl;

        cout << game_1.turn() << endl;

        spy->gather();
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n';
    }
    cout << game_1.turn() << endl;
            cout <<"????????????????????????????????????????????????????" << endl;


    governor->addCoins(2); // Governor add 2 coins


    cout << game_1.turn() << endl;
    cout << "governor:disabel_tax" << endl; // Expected: 3
    governor->disabel_tax(spy); // Governor undo tax

        cout << game_1.turn() << endl;

    cout << "spy:gather" << endl; // Expected: 3
    spy->gather();

        cout << game_1.turn() << endl;

    cout << "baron:tax" << endl; // Expected: 3
    baron->tax();

        cout << game_1.turn() << endl;

    cout << "general:gather" << endl; // Expected: 3
    general->gather();
        // Expected exception - Judge cannot undo tax

    try{
            cout << game_1.turn() << endl;

        cout << "try judge:disabel_tax" << endl; // Expected: 3

        judge->disabel_tax(governor);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
        cout << game_1.turn() << endl;

        cout << "judge:gather" << endl; // Expected: 3
    judge->gather(); 

    governor->tax();
    spy->gather();
    baron->invest(); // Baron traded its 3 coins and got 6 
    general->gather();
    judge->gather();
    
    cout << baron->getCoins() << endl; // Expected: 6

    governor->tax();
    spy->gather();
    baron->gather();
    general->gather();
    judge->gather();

    governor->tax();
    spy->gather();
    cout << baron->getCoins() << endl; // Expected: 7
    baron->coup(governor); // Coup against governor
    general->gather();
    judge->gather();
    
    players = game_1.players();
    // Since no one blocked the Baron, the expected output is:
    // Yossi
    // Meirav
    // Reut
    // Gilad
    for (string name : players) {
        cout << name << endl;
    }

}
