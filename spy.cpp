// daniisakov@gmail.com

#include "spy.hpp"

Spy::Spy(Game &game, string name) : Player(game, name) {}

int Spy::seeCoins(Player &target) {
  game.validateTarget(target);

  return target.getCoins();
}

void Spy::preventArrest(Player &target) {
  game.validateTarget(target);

  target.setArrestPrevented(true);
}