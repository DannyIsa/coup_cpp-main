// daniisakov@gmail.com

#pragma once

#include "player.hpp"

class Governor : public Player {
public:
  Governor(Game &game, string name);

  void tax() override;
  void cancelTax(Player &target);
};