// daniisakov@gmail.com

#include "governor.hpp"

Governor::Governor(Game &game, string name) : Player(game, name) {}

void Governor::tax() {
  game.validatePlayer(*this, ActionType::TAX);

  addCoins(3); // gets 3 coins instead of 2
  lastAction = ActionType::TAX;

  game.consumeAction();
}

void Governor::cancelTax(Player &target) {
  game.validateTarget(target);

  if (target.getLastAction() != ActionType::TAX) {
    throw invalid_argument("Target's last action was not tax");
  }

  if (target.getCoins() >= 2) {
    target.removeCoins(2);
  } else {
    target.removeCoins(target.getCoins());
  }

  target.setLastAction(ActionType::NONE);
}