// daniisakov@gmail.com

#include "baron.hpp"

Baron::Baron(Game &game, string name) : Player(game, name) {}

void Baron::invest() {
  game.validatePlayer(*this, ActionType::NONE, 3);

  removeCoins(3);
  addCoins(6);

  game.consumeAction();
}