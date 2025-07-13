// daniisakov@gmail.com

#include "baron.hpp"
#include "game.hpp"
#include "general.hpp"
#include "governor.hpp"
#include "judge.hpp"
#include "merchant.hpp"
#include "spy.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// create a text with a given string, font, size, color, and position
sf::Text makeText(const std::string &str, sf::Font &font, int size,
                  sf::Color color, float x, float y) {
  sf::Text t(str, font, size);
  t.setFillColor(color);
  t.setPosition(x, y);
  return t;
}

// create a button with a given x, y, width, height, and color
sf::RectangleShape makeButton(float x, float y, float w, float h,
                              sf::Color color) {
  sf::RectangleShape button(sf::Vector2f(w, h));
  button.setPosition(x, y);
  button.setFillColor(color);
  button.setOutlineThickness(1);
  button.setOutlineColor(sf::Color::Black);
  return button;
}

int main() {
  Game game;
  std::vector<std::unique_ptr<Player>> players; // all player types

  sf::RenderWindow window(sf::VideoMode(1200, 800), "Coup Game - Full Demo");
  sf::Font font;

  // Try multiple font paths to find one that works
  std::vector<std::string> fontPaths = {
      "/System/Library/Fonts/Arial.ttf", 
      "/System/Library/Fonts/Helvetica.ttc",
      "/System/Library/Fonts/Times.ttc", 
      "/System/Library/Fonts/Courier.ttc",
      "/System/Library/Fonts/Supplemental/Arial.ttf",
      "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
      "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"};

  bool fontLoaded = false;
  for (const auto &path : fontPaths) {
    if (font.loadFromFile(path)) {
      fontLoaded = true;
      std::cout << "Loaded font: " << path << std::endl;
      break;
    } else {
      std::cout << "Failed to load font: " << path << std::endl;
    }
  }

  if (!fontLoaded) {
    std::cerr << "Could not load any font. Using default font.\n";
    // Try to load from built-in font or create a minimal font
    if (!font.loadFromFile("")) {
      std::cerr << "Warning: No font available, text may not display correctly.\n";
    }
  }

  // Player creation section
  sf::RectangleShape addButton =
      makeButton(370, 100, 120, 35, sf::Color(50, 150, 70));
  sf::Text addButtonText =
      makeText("Add Player", font, 18, sf::Color::White, 385, 110);

  // Player type dropdown
  std::vector<std::string> playerTypes = {"Governor", "Baron",    "General",
                                          "Judge",    "Merchant", "Spy"};
  int selectedType = 0;
  bool dropdownOpen = false;
  sf::RectangleShape dropdownButton =
      makeButton(200, 100, 150, 35, sf::Color(100, 100, 200));
  sf::Text dropdownButtonText =
      makeText(playerTypes[selectedType], font, 16, sf::Color::White, 220, 110);

  // Dropdown items
  std::vector<sf::RectangleShape> dropdownItems;
  std::vector<sf::Text> dropdownTexts;
  for (size_t i = 0; i < playerTypes.size(); i++) {
    sf::RectangleShape item =
        makeButton(200, 140 + i * 30, 150, 30, sf::Color(120, 120, 220));
    sf::Text text =
        makeText(playerTypes[i], font, 14, sf::Color::White, 220, 145 + i * 30);
    dropdownItems.push_back(item);
    dropdownTexts.push_back(text);
  }

  // Action buttons
  sf::RectangleShape gatherButton =
      makeButton(400, 170, 80, 30, sf::Color(70, 130, 180));
  sf::Text gatherButtonText =
      makeText("Gather", font, 14, sf::Color::White, 420, 180);

  sf::RectangleShape taxButton =
      makeButton(490, 170, 80, 30, sf::Color(255, 140, 0));
  sf::Text taxButtonText =
      makeText("Tax", font, 14, sf::Color::White, 515, 180);

  sf::RectangleShape investButton =
      makeButton(580, 170, 80, 30, sf::Color(50, 100, 180));
  sf::Text investButtonText =
      makeText("Invest", font, 14, sf::Color::White, 600, 180);

  sf::RectangleShape bribeButton =
      makeButton(400, 210, 80, 30, sf::Color(200, 50, 50));
  sf::Text bribeButtonText =
      makeText("Bribe", font, 14, sf::Color::White, 420, 220);

  sf::RectangleShape arrestButton =
      makeButton(490, 210, 80, 30, sf::Color(150, 50, 150));
  sf::Text arrestButtonText =
      makeText("Arrest", font, 14, sf::Color::White, 505, 220);

  sf::RectangleShape sanctionButton =
      makeButton(580, 210, 80, 30, sf::Color(180, 100, 50));
  sf::Text sanctionButtonText =
      makeText("Sanction", font, 14, sf::Color::White, 590, 220);

  sf::RectangleShape coupButton =
      makeButton(400, 250, 80, 30, sf::Color(200, 0, 0));
  sf::Text coupButtonText =
      makeText("Coup", font, 14, sf::Color::White, 425, 260);

  sf::RectangleShape nextTurnButton =
      makeButton(490, 250, 80, 30, sf::Color(100, 200, 100));
  sf::Text nextTurnButtonText =
      makeText("End Turn", font, 14, sf::Color::White, 500, 260);

  sf::RectangleShape seeCoinsButton =
      makeButton(700, 170, 90, 30, sf::Color(128, 0, 128));
  sf::Text seeCoinsButtonText =
      makeText("See Coins", font, 12, sf::Color::White, 715, 180);

  sf::RectangleShape preventArrestButton =
      makeButton(800, 170, 90, 30, sf::Color(128, 64, 0));
  sf::Text preventArrestButtonText =
      makeText("Prev Arrest", font, 11, sf::Color::White, 810, 180);

  sf::RectangleShape preventCoupButton =
      makeButton(700, 210, 90, 30, sf::Color(64, 128, 64));
  sf::Text preventCoupButtonText =
      makeText("Prev Coup", font, 12, sf::Color::White, 715, 220);

  sf::RectangleShape cancelBribeButton =
      makeButton(800, 210, 90, 30, sf::Color(128, 128, 0));
  sf::Text cancelBribeButtonText =
      makeText("Cancel Bribe", font, 10, sf::Color::White, 805, 220);

  sf::RectangleShape cancelTaxButton =
      makeButton(700, 250, 90, 30, sf::Color(0, 128, 128));
  sf::Text cancelTaxButtonText =
      makeText("Cancel Tax", font, 12, sf::Color::White, 715, 260);

  // Target selection
  int selectedTarget = -1;
  sf::Text targetText =
      makeText("Target: None (Click a player below to select)", font, 16,
               sf::Color::Red, 400, 300);

  // Player Name Input
  std::string inputName;
  bool inputActive = false;
  sf::RectangleShape inputBox(sf::Vector2f(150, 35));
  inputBox.setPosition(30, 100);
  inputBox.setFillColor(sf::Color(230, 230, 230));
  inputBox.setOutlineThickness(2);
  inputBox.setOutlineColor(sf::Color::Black);
  sf::Text inputText = makeText("", font, 18, sf::Color::Black, 40, 110);

  std::string statusMsg;
  sf::Text statusText = makeText("", font, 16, sf::Color::Blue, 30, 650);

  // Main Loop
  // Ensure proper cleanup on exit
  auto cleanup = [&]() {
    // Clear all players before game destruction
    players.clear();
    std::cout << "Cleanup completed." << std::endl;
  };

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::MouseButtonPressed) {
        auto mouse = sf::Mouse::getPosition(window);

        if (inputBox.getGlobalBounds().contains(mouse.x, mouse.y)) {
          inputActive = true;
          inputBox.setOutlineColor(sf::Color::Blue);
          dropdownOpen = false; // Close dropdown when clicking outside
        } else {
          inputActive = false;
          inputBox.setOutlineColor(sf::Color::Black);
        }

        // Dropdown button click
        if (dropdownButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
          dropdownOpen = !dropdownOpen;
        } else if (dropdownOpen) {
          bool itemClicked = false;
          for (size_t i = 0; i < dropdownItems.size(); i++) {
            if (dropdownItems[i].getGlobalBounds().contains(mouse.x, mouse.y)) {
              selectedType = i;
              dropdownButtonText.setString(playerTypes[selectedType]);
              dropdownOpen = false;
              itemClicked = true;
              break;
            }
          }
          if (!itemClicked) {
            dropdownOpen = false; // Close dropdown if clicked outside
          }
        }

        // Add player button
        if (addButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
          if (!inputName.empty()) {
            bool exists = false;
            for (auto &p : players) {
              if (p->getName() == inputName)
                exists = true;
            }
            if (!exists) {
              // Create player based on selected type
              switch (selectedType) {
              case 0:
                players.push_back(std::make_unique<Governor>(game, inputName));
                break;
              case 1:
                players.push_back(std::make_unique<Baron>(game, inputName));
                break;
              case 2:
                players.push_back(std::make_unique<General>(game, inputName));
                break;
              case 3:
                players.push_back(std::make_unique<Judge>(game, inputName));
                break;
              case 4:
                players.push_back(std::make_unique<Merchant>(game, inputName));
                break;
              case 5:
                players.push_back(std::make_unique<Spy>(game, inputName));
                break;
              }
              players.back()->addCoins(2); // Starting coins
              statusMsg = "Added " + playerTypes[selectedType] + " " +
                          inputName + " with 2 starting coins!";
              inputName.clear();
              inputActive = false;
              inputBox.setOutlineColor(sf::Color::Black);
              dropdownOpen = false;
            } else {
              statusMsg = "Player name already exists!";
            }
          } else {
            statusMsg = "Enter a name to add player.";
          }
        }

        // Target selection (click on player names)
        if (!dropdownOpen) {
          float playerY = 380;
          for (size_t i = 0; i < players.size(); i++) {
            sf::FloatRect playerRect(35, playerY, 500, 25);
            if (playerRect.contains(mouse.x, mouse.y)) {
              selectedTarget = i;
              targetText.setString("Target: " + players[i]->getName() +
                                   " (Selected)");
              targetText.setFillColor(sf::Color::Blue);
              break;
            }
            playerY += 30;
          }
        }

        // Action buttons
        if (!dropdownOpen) { // Only allow actions when dropdown is closed
          try {
            if (!players.empty()) {
              // Find current player
              Player *currentPlayer = nullptr;
              for (auto &p : players) {
                if (game.turn() == p->getName()) {
                  currentPlayer = p.get();
                  break;
                }
              }

              if (currentPlayer) {
                bool actionTaken = false;

                if (gatherButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                  currentPlayer->gather();
                  statusMsg = currentPlayer->getName() + " gathered 1 coin!";
                  actionTaken = true;
                } else if (taxButton.getGlobalBounds().contains(mouse.x,
                                                                mouse.y)) {
                  currentPlayer->tax();
                  statusMsg = currentPlayer->getName() + " collected tax!";
                  actionTaken = true;
                } else if (investButton.getGlobalBounds().contains(mouse.x,
                                                                   mouse.y)) {
                  Baron *baron = dynamic_cast<Baron *>(currentPlayer);
                  if (baron) {
                    baron->invest();
                    statusMsg = baron->getName() + " invested successfully!";
                    actionTaken = true;
                  } else {
                    statusMsg = "Only Barons can invest!";
                  }
                } else if (bribeButton.getGlobalBounds().contains(mouse.x,
                                                                  mouse.y)) {
                  currentPlayer->bribe();
                  statusMsg = currentPlayer->getName() +
                              " used bribe to get extra actions!";
                  actionTaken = true;
                } else if (arrestButton.getGlobalBounds().contains(mouse.x,
                                                                   mouse.y)) {
                  if (selectedTarget >= 0) {
                    currentPlayer->arrest(*players[selectedTarget]);
                    statusMsg = currentPlayer->getName() + " arrested " +
                                players[selectedTarget]->getName() + "!";
                    actionTaken = true;
                  } else {
                    statusMsg = "Select a target player first to arrest!";
                  }
                } else if (sanctionButton.getGlobalBounds().contains(mouse.x,
                                                                     mouse.y)) {
                  if (selectedTarget >= 0) {
                    currentPlayer->sanction(*players[selectedTarget]);
                    statusMsg = currentPlayer->getName() + " sanctioned " +
                                players[selectedTarget]->getName() + "!";
                    actionTaken = true;
                  } else {
                    statusMsg = "Select a target player first to sanction!";
                  }
                } else if (coupButton.getGlobalBounds().contains(mouse.x,
                                                                 mouse.y)) {
                  if (selectedTarget >= 0) {
                    currentPlayer->coup(*players[selectedTarget]);
                    statusMsg = currentPlayer->getName() + " couped " +
                                players[selectedTarget]->getName() + "!";
                    selectedTarget = -1; // Clear target after coup
                    targetText.setString(
                        "Target: None (Click a player below to select)");
                    targetText.setFillColor(sf::Color::Red);
                    actionTaken = true;
                  } else {
                    statusMsg = "Select a target player first to coup!";
                  }
                } else if (nextTurnButton.getGlobalBounds().contains(mouse.x,
                                                                     mouse.y)) {
                  game.nextTurn();
                  selectedTarget = -1;
                  targetText.setString(
                      "Target: None (Click a player below to select)");
                  targetText.setFillColor(sf::Color::Red);
                  statusMsg =
                      "Turn ended. Now it's " + game.turn() + "'s turn.";
                }

                // Special ability buttons
                else if (seeCoinsButton.getGlobalBounds().contains(mouse.x,
                                                                   mouse.y)) {
                  if (selectedTarget >= 0) {
                    Spy *spy = dynamic_cast<Spy *>(currentPlayer);
                    if (spy) {
                      int coins = spy->seeCoins(*players[selectedTarget]);
                      statusMsg = spy->getName() + " sees that " +
                                  players[selectedTarget]->getName() + " has " +
                                  std::to_string(coins) + " coins!";
                    } else {
                      statusMsg = "Only Spies can see coins!";
                    }
                  } else {
                    statusMsg =
                        "Select a target player first to see their coins!";
                  }
                } else if (preventArrestButton.getGlobalBounds().contains(
                               mouse.x, mouse.y)) {
                  if (selectedTarget >= 0) {
                    Spy *spy = dynamic_cast<Spy *>(currentPlayer);
                    if (spy) {
                      spy->preventArrest(*players[selectedTarget]);
                      statusMsg = spy->getName() + " prevented " +
                                  players[selectedTarget]->getName() +
                                  " from being arrested next turn!";
                    } else {
                      statusMsg = "Only Spies can prevent arrest!";
                    }
                  } else {
                    statusMsg =
                        "Select a target player first to prevent their arrest!";
                  }
                } else if (preventCoupButton.getGlobalBounds().contains(
                               mouse.x, mouse.y)) {
                  if (selectedTarget >= 0) {
                    General *general = dynamic_cast<General *>(currentPlayer);
                    if (general) {
                      general->preventCoup(*players[selectedTarget]);
                      statusMsg = general->getName() +
                                  " prevented coup against " +
                                  players[selectedTarget]->getName() + "!";
                      actionTaken = true;
                    } else {
                      statusMsg = "Only Generals can prevent coup!";
                    }
                  } else {
                    statusMsg = "Select a target player first to prevent coup "
                                "against them!";
                  }
                } else if (cancelBribeButton.getGlobalBounds().contains(
                               mouse.x, mouse.y)) {
                  if (selectedTarget >= 0) {
                    Judge *judge = dynamic_cast<Judge *>(currentPlayer);
                    if (judge) {
                      judge->cancelBribe(*players[selectedTarget]);
                      statusMsg = judge->getName() + " cancelled " +
                                  players[selectedTarget]->getName() +
                                  "'s bribe!";
                      actionTaken = true;
                    } else {
                      statusMsg = "Only Judges can cancel bribe!";
                    }
                  } else {
                    statusMsg =
                        "Select a target player first to cancel their bribe!";
                  }
                } else if (cancelTaxButton.getGlobalBounds().contains(
                               mouse.x, mouse.y)) {
                  if (selectedTarget >= 0) {
                    Governor *governor =
                        dynamic_cast<Governor *>(currentPlayer);
                    if (governor) {
                      governor->cancelTax(*players[selectedTarget]);
                      statusMsg = governor->getName() + " cancelled " +
                                  players[selectedTarget]->getName() +
                                  "'s tax!";
                    } else {
                      statusMsg = "Only Governors can cancel tax!";
                    }
                  } else {
                    statusMsg =
                        "Select a target player first to cancel their tax!";
                  }
                }
                if (actionTaken) {
                  try {
                    if (currentPlayer->getCoins() >= 10) {
                      statusMsg += " WARNING: " + currentPlayer->getName() +
                                   " has 10+ coins and must coup!";
                    }
                    try {
                      std::string winner = game.winner();
                      statusMsg = "GAME OVER! " + winner + " wins!";
                    } catch (...) {}
                  } catch (...) {}
                }
              }
            }
          } catch (const std::exception &e) {
            statusMsg = std::string("Error: ") + e.what();
          }
        }
      }

      // Text input for name
      if (inputActive && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) {
          if (!inputName.empty())
            inputName.pop_back();
        } else if (event.text.unicode >= 32 && event.text.unicode < 127) {
          if (inputName.size() < 16)
            inputName += static_cast<char>(event.text.unicode);
        }
      }
    }

    // Drawing
    window.clear(sf::Color(245, 245, 250));

    // HEADER SECTION
    sf::Text title = makeText("Coup Game - Full Demo", font, 28,
                              sf::Color(50, 50, 100), 30, 20);
    window.draw(title);

    // PLAYER CREATION SECTION
    sf::Text inputLabel =
        makeText("Player Name:", font, 16, sf::Color::Black, 30, 70);
    window.draw(inputLabel);
    window.draw(inputBox);
    inputText.setString(inputName + (inputActive ? "|" : ""));
    window.draw(inputText);
    window.draw(addButton);
    window.draw(addButtonText);

    // Player type dropdown
    sf::Text dropdownLabel =
        makeText("Player Type:", font, 16, sf::Color::Black, 200, 70);
    window.draw(dropdownLabel);
    window.draw(dropdownButton);
    window.draw(dropdownButtonText);
    if (dropdownOpen) {
      for (size_t i = 0; i < dropdownItems.size(); i++) {
        window.draw(dropdownItems[i]);
        window.draw(dropdownTexts[i]);
      }
    }

    // ACTIONS SECTION
    if (!players.empty()) {
      sf::Text actionsLabel =
          makeText("Actions:", font, 20, sf::Color(50, 100, 50), 400, 140);
      window.draw(actionsLabel);

      window.draw(gatherButton);
      window.draw(gatherButtonText);
      window.draw(taxButton);
      window.draw(taxButtonText);
      window.draw(investButton);
      window.draw(investButtonText);
      window.draw(bribeButton);
      window.draw(bribeButtonText);
      window.draw(arrestButton);
      window.draw(arrestButtonText);
      window.draw(sanctionButton);
      window.draw(sanctionButtonText);
      window.draw(coupButton);
      window.draw(coupButtonText);
      window.draw(nextTurnButton);
      window.draw(nextTurnButtonText);

      // SPECIAL ABILITIES SECTION
      sf::Text specialLabel = makeText("Special Abilities:", font, 20,
                                       sf::Color(100, 50, 100), 700, 140);
      window.draw(specialLabel);

      window.draw(seeCoinsButton);
      window.draw(seeCoinsButtonText);
      window.draw(preventArrestButton);
      window.draw(preventArrestButtonText);
      window.draw(preventCoupButton);
      window.draw(preventCoupButtonText);
      window.draw(cancelBribeButton);
      window.draw(cancelBribeButtonText);
      window.draw(cancelTaxButton);
      window.draw(cancelTaxButtonText);

      // TARGET SELECTION
      window.draw(targetText);
    }

    // PLAYER LIST SECTION
    float y = 380;
    sf::Text listTitle = makeText("Players (click to select target):", font, 20,
                                  sf::Color(100, 50, 50), 30, 350);
    window.draw(listTitle);

    if (players.empty()) {
      sf::Text noPlayers = makeText("No players yet. Add players to start!",
                                    font, 16, sf::Color(128, 128, 128), 35, y);
      window.draw(noPlayers);
    } else {
      try {
        std::string currentTurn = game.turn();
        int playerNum = 1;
        for (auto &p : players) {
          std::string playerType = "Unknown";
          if (dynamic_cast<Governor *>(p.get()))
            playerType = "Governor";
          else if (dynamic_cast<Baron *>(p.get()))
            playerType = "Baron";
          else if (dynamic_cast<General *>(p.get()))
            playerType = "General";
          else if (dynamic_cast<Judge *>(p.get()))
            playerType = "Judge";
          else if (dynamic_cast<Merchant *>(p.get()))
            playerType = "Merchant";
          else if (dynamic_cast<Spy *>(p.get()))
            playerType = "Spy";

          std::string row = std::to_string(playerNum) + ". " + p->getName() +
                            " (" + playerType +
                            ") - Coins: " + std::to_string(p->getCoins());
          if (currentTurn == p->getName())
            row += " (CURRENT TURN)";
          if (!p->isAlive())
            row += " (DEAD)";
          if (p->isSanctioned())
            row += " [SANCTIONED]";
          if (p->isArrestPrevented())
            row += " [ARREST PROTECTED]";
          if (p->isCoupPrevented())
            row += " [COUP PROTECTED]";

          sf::Color textColor = sf::Color::Black;
          if (selectedTarget == static_cast<int>(playerNum - 1)) {
            textColor = sf::Color::Red; // Highlight selected target
          }

          sf::Text t = makeText(row, font, 16, textColor, 35, y);
          window.draw(t);
          y += 30;
          ++playerNum;
        }
      } catch (const std::exception &e) {
        sf::Text errorText = makeText("Game not started yet", font, 16,
                                      sf::Color(128, 128, 128), 35, y);
        window.draw(errorText);
      }
    }

    // GAME INFO SECTION
    if (!players.empty()) {
      std::string currentTurn = game.turn();
      sf::Text gameInfo = makeText("Current Turn: " + currentTurn, font, 18,
                                    sf::Color::Blue, 400, 320);
      window.draw(gameInfo);
    }

    // STATUS MESSAGE
    statusText.setString(statusMsg);
    statusText.setPosition(30, 750);
    statusText.setFillColor(sf::Color::Blue);
    window.draw(statusText);

    // HELP SECTION
    sf::Text helpTitle =
        makeText("Role Abilities:", font, 18, sf::Color(80, 80, 80), 30, 550);
    window.draw(helpTitle);

    std::vector<std::string> roleHelp = {
        "Governor: Gets 3 coins from tax, can cancel others' tax",
        "Baron: Can invest 3 coins to get 6 back, gets 1 coin when sanctioned",
        "General: Can pay 5 coins to prevent coup, gets coin back when "
        "arrested",
        "Judge: Can cancel bribe, sanctioner pays extra coin when sanctioning "
        "judge",
        "Merchant: Gets bonus coin if 3+ coins, pays 2 to treasury when "
        "arrested",
        "Spy: Can see coins (free), can prevent arrest (free)"};

    float helpY = 580;
    for (const auto &help : roleHelp) {
      sf::Text helpText =
          makeText(help, font, 12, sf::Color(100, 100, 100), 30, helpY);
      window.draw(helpText);
      helpY += 20;
    }

    window.display();
  }

  // Cleanup before exit
  cleanup();

  return 0;
}
