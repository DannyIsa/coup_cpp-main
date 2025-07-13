// daniisakov@gmail.com

#include "judge.hpp"
#include <stdexcept>
#include <algorithm>
Judge::Judge(Game &game, string name) : Player(game, name) {}

void Judge::cancelBribe(Player &target) {
  game.validateTarget(target);
  game.validatePlayerTurn(target);
  game.validatePlayerHasActions();

  if (target.getLastAction() != ActionType::BRIBE) {
    throw std::invalid_argument("Target's last action was not bribe");
  }

  game.consumeAction();
}