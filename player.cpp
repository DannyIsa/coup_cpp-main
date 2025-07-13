// daniisakov@gmail.com

#include "player.hpp"
#include <stdexcept>

Player::Player(Game &game, string name)
    : game(game), name(name), coins(0), is_alive(true),
      lastAction(ActionType::NONE), lastArrested(nullptr), sanctioned(false),
      arrestPrevented(false), coupPrevented(false) {
  game.addPlayer(*this);
}

int Player::getCoins() { return coins; }

bool Player::isAlive() { return is_alive; }

string Player::getName() { return name; }

Player *Player::getLastArrested() { return lastArrested; }

void Player::setLastArrested(Player *target) { lastArrested = target; }

bool Player::isSanctioned() { return sanctioned; }

void Player::setSanctioned(bool value) { sanctioned = value; }

bool Player::isArrestPrevented() { return arrestPrevented; }

void Player::setArrestPrevented(bool value) { arrestPrevented = value; }

bool Player::isCoupPrevented() { return coupPrevented; }

void Player::setCoupPrevented(bool value) { coupPrevented = value; }

void Player::removePlayer() {
  if (!is_alive) {
    throw std::invalid_argument("Player is already dead");
  }
  is_alive = false;
  game.removePlayer(*this);
}

void Player::addCoins(int amount) {
  if (amount < 0) {
    throw std::invalid_argument("Amount must be positive");
  }
  coins += amount;
}

void Player::removeCoins(int amount) {
  if (amount < 0) {
    throw std::invalid_argument("Amount must be positive");
  }
  if (amount > coins) {
    throw std::invalid_argument("Amount must be less than or equal to coins");
  }
  coins -= amount;
}

ActionType Player::getLastAction() const { return lastAction; }

void Player::clearLastAction() { lastAction = ActionType::NONE; }

void Player::setLastAction(ActionType action) { lastAction = action; }

void Player::gather() {
  game.validatePlayer(*this, ActionType::GATHER);

  addCoins(1);
  lastAction = ActionType::GATHER;

  game.consumeAction();
}

void Player::tax() {
  game.validatePlayer(*this, ActionType::TAX);

  addCoins(2);
  lastAction = ActionType::TAX;

  game.consumeAction();
}

void Player::bribe() {
  game.validatePlayer(*this, ActionType::BRIBE, 4);

  removeCoins(4);
  lastAction = ActionType::BRIBE;

  game.addActions(1);
}

void Player::arrest(Player &target) {
  game.validatePlayer(*this, ActionType::ARREST);
  game.validateTarget(target);

  if (getLastArrested() == &target) {
    throw std::invalid_argument("Player has already arrested this target");
  }

  if (target.getCoins() < 1) {
    throw std::invalid_argument("Target has no coins to arrest");
  }

  bool specialHandled = game.handleSpecialArrest(target);

  if (!specialHandled) {
    // Normal arrest: target loses 1 coin, attacker gains 1 coin
    target.removeCoins(1);
    addCoins(1);
  }

  setLastArrested(&target);
  lastAction = ActionType::ARREST;

  game.consumeAction();
}

void Player::sanction(Player &target) {
  game.validatePlayer(*this, ActionType::SANCTION, 3);
  game.validateTarget(target);

  game.handleSpecialSanction(*this, target);

  removeCoins(3);
  target.setSanctioned(true);
  lastAction = ActionType::SANCTION;

  game.consumeAction();
}

void Player::coup(Player &target) {
  game.validatePlayer(*this, ActionType::COUP, 7);
  game.validateTarget(target);

  if (target.isCoupPrevented()) {
    throw std::invalid_argument("Target has coup prevented");
  }

  removeCoins(7);
  target.removePlayer();
  lastAction = ActionType::COUP;

  game.consumeAction();
}