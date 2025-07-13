// daniisakov@gmail.com

#include "game.hpp"
#include "baron.hpp"
#include "general.hpp"
#include "judge.hpp"
#include "merchant.hpp"
#include "player.hpp"
#include <stdexcept>
#include <algorithm>

Game::Game() : playerTurn(nullptr), remainingActions(1) {}

string Game::turn() {
  if (playerTurn == nullptr) {
    throw std::invalid_argument("No player turn");
  }
  return playerTurn->getName();
}

vector<string> Game::players() {
  vector<string> names;
  for (Player *player : allPlayers) {
    if (player->isAlive()) {
      names.push_back(player->getName());
    }
  }
  return names;
}

string Game::winner() {
  int aliveCount = 0;
  Player *lastAlive = nullptr;

  for (Player *player : allPlayers) {
    if (player->isAlive()) {
      aliveCount++;
      lastAlive = player;
    }
  }

  if (aliveCount == 1) {
    return lastAlive->getName();
  }
  throw std::invalid_argument("No winner");
}

void Game::addPlayer(Player &player) {
  if (playerTurn == nullptr) {
    playerTurn = &player;
  }
  allPlayers.push_back(&player);
}

void Game::removePlayer(Player &player) {
  allPlayers.erase(std::find(allPlayers.begin(), allPlayers.end(), &player));
}

void Game::validatePlayer(Player &player, ActionType action, int price) {
  validatePlayerAlive(player);
  validatePlayerTurn(player);
  validatePlayerHasActions();
  validatePlayerHasEnoughCoins(player, price);
  validatePlayerHasLessThan10Coins(player, action);
  validatePlayerIsntSanctioned(player, action);
  validateCanArrest(player, action);
}

void Game::validateTarget(Player &target) {
  validatePlayerAlive(target);
  validateTargetInGame(target);
}

void Game::handleSpecialSanction(Player &sanctioner, Player &target) {
  Baron *baron = dynamic_cast<Baron *>(&target);
  if (baron != nullptr) {
    baron->addCoins(1);
  }

  Judge *judge = dynamic_cast<Judge *>(&target);
  if (judge != nullptr) {
    if (sanctioner.getCoins() >= 4) {
      sanctioner.removeCoins(1);
    } else {
      throw std::invalid_argument(
          "Player does not have enough coins to pay for sanction");
    }
  }
}

bool Game::handleSpecialArrest(Player &target) {
  General *general = dynamic_cast<General *>(&target);
  if (general != nullptr) {
    general->addCoins(1); // General gets back the coin
    return false;
  }

  Merchant *merchant = dynamic_cast<Merchant *>(&target);
  if (merchant != nullptr) {
    if (merchant->getCoins() >= 2) {
      merchant->removeCoins(2);
    } else {
      merchant->removeCoins(merchant->getCoins());
    }
    return true;
  }

  return false; // Normal arrest
}

void Game::handleStartOfTurn(Player &player) {
  Merchant *merchant = dynamic_cast<Merchant *>(&player);
  if (merchant != nullptr && merchant->getCoins() >= 3) {
    merchant->addCoins(1); // Merchant bonus
  }
}

void Game::validateCanArrest(Player &player, ActionType action) {
  if (player.isArrestPrevented() && action == ActionType::ARREST) {
    throw std::invalid_argument("Player cannot perform arrest");
  }
}

void Game::validatePlayerIsntSanctioned(Player &player, ActionType action) {
  if (player.isSanctioned() &&
      (action == ActionType::GATHER || action == ActionType::TAX)) {
    throw std::invalid_argument("Player is sanctioned");
  }
}

void Game::validatePlayerHasEnoughCoins(Player &player, int price) {
  if (player.getCoins() < price) {
    throw std::invalid_argument("Player does not have enough coins");
  }
}

void Game::validatePlayerHasLessThan10Coins(Player &player, ActionType action) {
  if (player.getCoins() >= 10 && action != ActionType::COUP) {
    throw std::invalid_argument("Player has more than 10 coins, must perform coup");
  }
}

void Game::validatePlayerTurn(Player &player) {
  if (playerTurn != &player) {
    throw std::invalid_argument("It's not your turn");
  }
}

void Game::validateTargetInGame(Player &target) {
  bool targetInGame =
      std::find(allPlayers.begin(), allPlayers.end(), &target) != allPlayers.end();

  if (!targetInGame) {
    throw std::invalid_argument("Target player must be in the same game");
  }
}

void Game::validatePlayerAlive(Player &player) {
  if (!player.isAlive()) {
    throw std::invalid_argument("Dead players cannot perform actions");
  }
}

void Game::validatePlayerHasActions() {
  if (remainingActions <= 0) {
    throw std::invalid_argument("No actions remaining in this turn");
  }
}

void Game::consumeAction() {
  remainingActions--;

  // If no actions left, automatically end turn
  if (remainingActions <= 0) {
    nextTurn();
  }
}

void Game::resetPlayer(Player &player) {
  player.setSanctioned(false);
  player.setArrestPrevented(false);
  player.setCoupPrevented(false);
  remainingActions = 1;

  handleStartOfTurn(player);
}

void Game::addActions(int amount) { remainingActions += amount; }

void Game::nextTurn() {
  if (allPlayers.empty())
    return;

  // Find current player position
  auto currentIt = std::find(allPlayers.begin(), allPlayers.end(), playerTurn);
  if (currentIt == allPlayers.end()) {
    throw std::invalid_argument("Current player not found in game");
  }

  Player *startingPlayer = playerTurn;

  // Loop through players to std::find next alive one
  do {
    currentIt++;
    if (currentIt == allPlayers.end()) {
      currentIt = allPlayers.begin();
    }

    playerTurn = *currentIt;
    resetPlayer(*playerTurn);
    return;
  } while (*currentIt != startingPlayer);

  // If we got back to starting player, game is finished
  throw std::invalid_argument("Game is finished - no other alive players");
}
