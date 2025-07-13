// daniisakov@gmail.com

#include "general.hpp"

General::General(Game &game, string name) : Player(game, name) {}

void General::preventCoup(Player &target) {
  game.validatePlayer(*this, ActionType::NONE, 5);
  game.validateTargetInGame(target);

  removeCoins(5);
  target.setCoupPrevented(true);
}