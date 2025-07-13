// daniisakov@gmail.com

#pragma once

#include "player.hpp"

class Spy : public Player {
public:
  Spy(Game &game, string name);

  int seeCoins(Player &target);
  void preventArrest(Player &target);
};