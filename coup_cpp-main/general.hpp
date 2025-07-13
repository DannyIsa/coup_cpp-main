// daniisakov@gmail.com

#pragma once

#include "player.hpp"

class General : public Player {
public:
  General(Game &game, string name);

  void preventCoup(Player &target);
};