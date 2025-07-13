// daniisakov@gmail.com

#pragma once

#include "player.hpp"

class Baron : public Player {
public:
  Baron(Game &game, string name);

  void invest();
};