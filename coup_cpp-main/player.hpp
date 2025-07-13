// daniisakov@gmail.com

#pragma once

#include "game.hpp"
#include <string>

using namespace std;

class Player {
protected:
  Game &game;
  string name;
  int coins;
  bool is_alive;
  ActionType lastAction;
  Player *lastArrested;
  bool sanctioned;
  bool arrestPrevented;
  bool coupPrevented;

public:
  Player(Game &game, string name);
  virtual ~Player() = default;

  int getCoins();
  bool isAlive();
  string getName();
  Player *getLastArrested();
  void setLastArrested(Player *target);
  bool isSanctioned();
  void setSanctioned(bool sanctioned);
  bool isArrestPrevented();
  void setArrestPrevented(bool prevented);
  void setCoupPrevented(bool prevented);
  bool isCoupPrevented();
  void removePlayer();
  void addCoins(int amount);
  void removeCoins(int amount);

  // Action tracking
  ActionType getLastAction() const;
  void clearLastAction();
  void setLastAction(ActionType action);

  virtual void gather();
  virtual void tax();
  virtual void bribe();
  virtual void arrest(Player &target);
  virtual void sanction(Player &target);
  virtual void coup(Player &target);
};