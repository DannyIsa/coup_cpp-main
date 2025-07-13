// daniisakov@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "game.hpp"
#include "governor.hpp"
#include "baron.hpp"
#include "general.hpp"

TEST_CASE("Test 1: Basic Player Creation and Initial State") {
    Game game;
    Governor player(game, "TestPlayer");
    
    CHECK(player.getName() == "TestPlayer");
    CHECK(player.getCoins() == 0);
    CHECK(player.isAlive() == true);
    CHECK(player.isSanctioned() == false);
    CHECK(player.getLastAction() == ActionType::NONE);
}

TEST_CASE("Test 2: Game Player Management") {
    Game game;
    Governor player1(game, "Player1");
    Baron player2(game, "Player2");
    
    vector<string> players = game.players();
    CHECK(players.size() == 2);
    CHECK(players[0] == "Player1");
    CHECK(players[1] == "Player2");
    
    CHECK(game.turn() == "Player1");
}

TEST_CASE("Test 3: Basic Gather Action") {
    Game game;
    Governor player(game, "TestPlayer");
    
    int initialCoins = player.getCoins();
    player.gather();
    
    CHECK(player.getCoins() == initialCoins + 1);
    CHECK(player.getLastAction() == ActionType::GATHER);
}

TEST_CASE("Test 4: Basic Tax Action") {
    Game game;
    Governor player(game, "TestPlayer");
    
    int initialCoins = player.getCoins();
    player.tax();
    
    CHECK(player.getCoins() == initialCoins + 3);
    CHECK(player.getLastAction() == ActionType::TAX);
}

TEST_CASE("Test 5: Coin Management - Add and Remove") {
    Game game;
    Governor player(game, "TestPlayer");
    
    player.addCoins(5);
    CHECK(player.getCoins() == 5);
    
    player.removeCoins(2);
    CHECK(player.getCoins() == 3);
    
    CHECK_THROWS_AS(player.removeCoins(10), invalid_argument);
}

TEST_CASE("Test 6: Player Elimination") {
    Game game;
    Governor player1(game, "Player1");
    Baron player2(game, "Player2");
    
    game.addPlayer(player1);
    game.addPlayer(player2);
    
    CHECK(player1.isAlive() == true);
    player1.removePlayer();
    CHECK(player1.isAlive() == false);
    
    CHECK_THROWS_AS(player1.removePlayer(), invalid_argument);
}

TEST_CASE("Test 7: Baron Investment Ability") {
    Game game;
    Baron baron(game, "BaronPlayer");
    
    baron.addCoins(3);
    int initialCoins = baron.getCoins();
    
    baron.invest();
    
    CHECK(baron.getCoins() == initialCoins + 3);
}

TEST_CASE("Test 8: Sanction Mechanism") {
    Game game;
    Governor player1(game, "Player1");
    Baron player2(game, "Player2");
    
    player1.addCoins(3);
    
    CHECK(player2.isSanctioned() == false);
    
    player2.setSanctioned(true);
    CHECK(player2.isSanctioned() == true);
    
    game.nextTurn();
    
    CHECK(player2.isSanctioned() == false);
    
    player2.gather();
    CHECK(player2.getCoins() == 1);
}

TEST_CASE("Test 9: Arrest Action and Prevention") {
    Game game;
    Baron player1(game, "Player1");
    General player2(game, "Player2");
    
    player2.addCoins(5);
    int player1InitialCoins = player1.getCoins();
    int player2InitialCoins = player2.getCoins();
    
    player1.arrest(player2);
    
    CHECK(player1.getCoins() == player1InitialCoins + 1);
    CHECK(player2.getCoins() == player2InitialCoins);
    CHECK(player1.getLastArrested() == &player2);
    
    game.nextTurn();
    game.nextTurn();
    
    CHECK_THROWS(player1.arrest(player2));
}

TEST_CASE("Test 10: Game Winner Detection") {
    Game game;
    Governor player1(game, "Player1");
    Baron player2(game, "Player2");
    General player3(game, "Player3");
    
    CHECK_THROWS_AS(game.winner(), invalid_argument);
    
    player2.removePlayer();
    player3.removePlayer();
    
    CHECK(game.winner() == "Player1");
}

TEST_CASE("Test 11: Turn Management") {
    Game game;
    Governor player1(game, "Player1");
    Baron player2(game, "Player2");
    
    game.addPlayer(player1);
    game.addPlayer(player2);
    
    CHECK(game.turn() == "Player1");
    
    game.nextTurn();
    CHECK(game.turn() == "Player2");
    
    game.nextTurn();
    CHECK(game.turn() == "Player1");
}

TEST_CASE("Test 12: Bribe Action - Extra Turn") {
    Game game;
    Governor player(game, "TestPlayer");
    game.addPlayer(player);
    
    player.addCoins(4);
    int initialCoins = player.getCoins();
    
    player.bribe();
    
    CHECK(player.getCoins() == initialCoins - 4);
    CHECK(player.getLastAction() == ActionType::BRIBE);
    
    CHECK_THROWS(player.bribe());
} 