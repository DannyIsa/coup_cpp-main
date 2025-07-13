// daniisakov@gmail.com

#include "judge.hpp"

Judge::Judge(Game &game, string name) : Player(game, name) {}

void Judge::cancelBribe(Player &target) {
  game.validateTarget(target);
  game.validatePlayerTurn(target);
  game.validatePlayerHasActions();

  if (target.getLastAction() != ActionType::BRIBE) {
    throw invalid_argument("Target's last action was not bribe");
  }

  game.consumeAction();
}