#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "./include/Governor.hpp"
#include "./include/Spy.hpp"
#include "./include/Baron.hpp"
#include "./include/General.hpp"
#include "./include/Judge.hpp"
#include "./include/Merchant.hpp"

TEST_CASE("Basic game flow and actions") {


    SUBCASE("Turn order advances correctly") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    Baron baron("Baron", g);
    General gen("Gen", g);
    Judge judge("Judge", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.add_player(&baron);
    g.add_player(&gen);
    g.add_player(&judge);
    g.start_game();
        CHECK(g.turn() == "Gov");
        gov.gather();
        CHECK(g.turn() == "Spy");
        spy.gather();
        CHECK(g.turn() == "Baron");
    }

    SUBCASE("Governor must coup at 10 coins") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    Baron baron("Baron", g);
    General gen("Gen", g);
    Judge judge("Judge", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.add_player(&baron);
    g.add_player(&gen);
    g.add_player(&judge);
    g.start_game();
        gov.addCoins(10);
        CHECK_THROWS_WITH(gov.gather(), "Player cannot gather: Player has 10 coins and must coup.");
    }

    
    SUBCASE("Spy cannot use seeCoinsOf out of turn") {
        Game g;
        Governor gov("Gov", g);
        Spy spy("Spy", g);
        Baron baron("Baron", g);
        General gen("Gen", g);
        Judge judge("Judge", g);

        g.add_player(&gov);
        g.add_player(&spy);
        g.add_player(&baron);
        g.add_player(&gen);
        g.add_player(&judge);
        g.start_game();
        CHECK_THROWS(spy.seeCoinsOf(&gov));
    }

    SUBCASE("Baron can invest with 3 coins") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    Baron baron("Baron", g);
    General gen("Gen", g);
    Judge judge("Judge", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.add_player(&baron);
    g.add_player(&gen);
    g.add_player(&judge);
    g.start_game();
        baron.addCoins(3);
        gov.gather();
        spy.gather();
        baron.invest();
        CHECK(baron.getCoins() == 6);/////////////////////////////////////////////////////////////
    }

    SUBCASE("Cannot perform actions when not in turn") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    Baron baron("Baron", g);
    General gen("Gen", g);
    Judge judge("Judge", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.add_player(&baron);
    g.add_player(&gen);
    g.add_player(&judge);
    g.start_game();
        CHECK_THROWS_WITH(spy.gather(), "Player cannot gather: Not Spy's turn.");
    }
}

TEST_CASE("Edge cases and error handling") {


    SUBCASE("Cannot arrest same player twice") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    g.add_player(&gov);
    g.add_player(&spy);
    g.start_game();
        gov.gather();
        spy.addCoins(3);
        spy.arrest(&gov);
        gov.gather();
        CHECK_THROWS_WITH(spy.arrest(&gov), "Player cannot arrest: cenot arrest the same player twice in a row.");
    }

    SUBCASE("Tax action and disabel_tax") {
    Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    g.add_player(&gov);
    g.add_player(&spy);
    g.start_game();
    gov.addCoins(3);
    CHECK(spy.getCoins() == 0);

    gov.tax();
    spy.tax();
    CHECK(spy.getCoins() == 2);
    gov.disabel_tax(&spy);

        CHECK(spy.getCoins() == 0);
    }

    SUBCASE("Attempting disabel_arrest when not allowed") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    g.add_player(&gov);
    g.add_player(&spy);
    g.start_game();
        CHECK_THROWS_WITH(spy.disabel_arrest(&gov), "Spy cannot disabel arrest: Not Spy's turn.");
    }

    SUBCASE("Player cannot undo") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    g.add_player(&gov);
    g.add_player(&spy);
    g.start_game();
        CHECK_THROWS_WITH(spy.undo(&gov), "Player cannot undo");
    }

}

TEST_CASE("Victory condition") {
    Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.start_game();

    gov.addCoins(7);

    gov.coup(&spy);

    CHECK(g.players().size() == 1);
    CHECK(g.winner() == "Gov");
}
TEST_CASE(" condition") {
    Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.start_game();

    gov.addCoins(20);
    CHECK(g.get_current_player()->getName() == "Gov");

    gov.gather();
    spy.gather();
    gov.bribe();
    gov.gather();
        gov.bribe();



    CHECK(g.players().size() == 2);
}

TEST_CASE("Additional action tests") {

    SUBCASE("Bribe adds a turn") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    Baron baron("Baron", g);
    General gen("Gen", g);
    Judge judge("Judge", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.add_player(&baron);
    g.add_player(&gen);
    g.add_player(&judge);
    g.start_game();

        gov.addCoins(5);
        gov.bribe();
        gov.gather();
        CHECK(g.get_current_player()== &gov);
    }

    SUBCASE("Sanction blocks profit actions") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    Baron baron("Baron", g);
    General gen("Gen", g);
    Judge judge("Judge", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.add_player(&baron);
    g.add_player(&gen);
    g.add_player(&judge);
    g.start_game();

        gov.addCoins(4);
        spy.addCoins(3);
        gov.sanction(&spy);
        CHECK_THROWS(spy.gather());
    }

    SUBCASE("General undoes coup") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    Baron baron("Baron", g);
    General gen("Gen", g);
    Judge judge("Judge", g);

    g.add_player(&gov);
    g.add_player(&spy);//
    g.add_player(&baron);
    g.add_player(&gen);
    g.add_player(&judge);
    g.start_game();

        gov.addCoins(10);
        gov.coup(&spy);
        CHECK(g.is_player_in_game(&spy)==false);
        CHECK(g.get_current_player()->getName()=="Baron");

        baron.gather();
        gen.addCoins(5);
        gen.undo_coup(&gov);
        CHECK(g.is_player_in_game(&spy));
    }

    SUBCASE("Kill and revive player") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    Baron baron("Baron", g);
    General gen("Gen", g);
    Judge judge("Judge", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.add_player(&baron);
    g.add_player(&gen);
    g.add_player(&judge);
    g.start_game();

        g.kill(&spy);
        CHECK_FALSE(g.is_player_in_game(&spy));
        g.revive(&spy);
        CHECK(g.is_player_in_game(&spy));
    }

    SUBCASE("Spy uses seeCoinsOf in turn") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    Baron baron("Baron", g);
    General gen("Gen", g);
    Judge judge("Judge", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.add_player(&baron);
    g.add_player(&gen);
    g.add_player(&judge);
    g.start_game();

        gov.gather();
        spy.addCoins(1);
        CHECK_NOTHROW(spy.seeCoinsOf(&gov));
    }

    SUBCASE(" chain that removes players") {
            Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    Baron baron("Baron", g);
    General gen("Gen", g);
    Judge judge("Judge", g);

    g.add_player(&gov);
    g.add_player(&spy);//
    g.add_player(&baron);//
    g.add_player(&gen);//
    g.add_player(&judge);//
    g.start_game();

        gov.addCoins(50);
        spy.addCoins(50);
        baron.addCoins(50);
        gen.addCoins(50);
        judge.addCoins(50);

        gov.coup(&spy);
        baron.coup(&gen);
        judge.coup(&baron);
        gov.coup(&judge);
        CHECK(g.players().size() == 1);
        CHECK(g.winner() == "Gov");
    }




    SUBCASE("Cannot act after being killed") {
    Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);

    g.add_player(&gov);
    g.add_player(&spy);
    g.start_game();

    g.kill(&spy); 
    CHECK_FALSE(g.is_player_in_game(&spy));
    CHECK_THROWS_WITH(spy.gather(), "Player cannot gather: Not Spy's turn.");
}


SUBCASE("Merchant gets bonus at start of turn") {
    Game g;
    Merchant m("Merchant", g);
    m.addCoins(3);
    Spy spy("Spy", g);
    g.add_player(&m);
    g.add_player(&spy);
    g.start_game();

    CHECK(m.getCoins() == 4);  // Should be 0+1 due to Merchant bonus
}



SUBCASE("Cannot coup without enough coins") {
    Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    g.add_player(&gov);
    g.add_player(&spy);
    g.start_game();

    gov.addCoins(5); // less than 7
    CHECK_THROWS_WITH(gov.coup(&spy),"Player cannot coup: Player is not allowed to coup.");
}

SUBCASE("Cannot coup without enough coins") {
    Game g;
    Governor gov("Gov", g);
    Spy spy("Spy", g);
    g.add_player(&gov);
    g.add_player(&spy);
    g.start_game();

    gov.addCoins(10); 
    gov.coup(&spy);
}



SUBCASE(""){
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
    

    // Expected output: Moshe
    governor->gather();
    spy->gather();

    baron->gather();

    general->gather();

    judge->gather();


    governor->tax();
    spy->tax();

    baron->tax();

    general->tax();

    judge->tax();


    // Expected exception - Not spy's turn
    CHECK_THROWS(spy->gather());



    governor->addCoins(2); 

    governor->disabel_tax(spy); // Governor undo tax


    spy->gather();


    baron->tax();


    general->gather();
        // Expected exception - Judge cannot undo tax
    CHECK_THROWS( judge->disabel_tax(governor));



    judge->gather(); 

    governor->tax();
    spy->gather();
    baron->invest(); // Baron traded its 3 coins and got 6 
    general->gather();
    judge->gather();
    

    governor->tax();
    spy->gather();
    baron->gather();
    general->gather();
    judge->gather();

    governor->tax();
    spy->gather();
    baron->coup(governor); // Coup against governor
    general->gather();
    judge->gather();
    
delete governor;
delete spy;
delete baron;
delete general;
delete judge;


}








}
TEST_CASE("Additional action tests") {

    // ... בדיקות אחרות

    SUBCASE("Two bribes followed by three tax actions") {
        Game g;
        Governor gov("Gov", g);
        Spy spy("Spy", g);

        g.add_player(&gov);
        g.add_player(&spy);
        g.start_game();

        gov.addCoins(20);
        CHECK(g.turn() == "Gov");

        CHECK_NOTHROW(gov.bribe());
        CHECK_NOTHROW(gov.bribe());

        CHECK_NOTHROW(gov.tax());
        CHECK_NOTHROW(gov.tax());
        CHECK_NOTHROW(gov.tax());

        CHECK(g.turn() == "Spy");
    }

}
