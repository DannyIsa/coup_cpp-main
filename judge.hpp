// daniisakov@gmail.com

#pragma once

#include "player.hpp"

class Judge : public Player {
public:
  Judge(Game &game, string name);

  void cancelBribe(Player &target);
};