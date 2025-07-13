// daniisakov@gmail.com

#pragma once

#include <string>
#include <vector>
using namespace std;

class Player;

enum class ActionType { NONE, GATHER, TAX, BRIBE, ARREST, SANCTION, COUP };

class Game {
protected:
  vector<Player *> allPlayers;
  Player *playerTurn;
  int remainingActions; // Number of actions left

public:
  Game();
  ~Game() = default;

  string turn();
  vector<string> players();
  string winner();
  void addPlayer(Player &player);
  void removePlayer(Player &player);

  void nextTurn();
  void validatePlayer(Player &player, ActionType action, int price = 0);
  void validateTarget(Player &target);
  void handleSpecialSanction(Player &sanctioner, Player &target);
  bool handleSpecialArrest(Player &target);
  void handleStartOfTurn(Player &player);

  // Action management methods
  void consumeAction();
  void addActions(int amount);

  // public validations
  void validateTargetInGame(Player &target);
  void validatePlayerHasEnoughCoins(Player &player, int price);
  void validatePlayerTurn(Player &player);
  void validatePlayerHasActions();

private:
  void resetPlayer(Player &player);

  // private validations
  void validatePlayerAlive(Player &player);
  void validatePlayerHasLessThan10Coins(Player &player, ActionType action);
  void validatePlayerIsntSanctioned(Player &player, ActionType action);
  void validateCanArrest(Player &player, ActionType action);
};