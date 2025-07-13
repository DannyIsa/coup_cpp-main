# Coup Game - C++ Implementation

**Author:** daniisakov@gmail.com

## 🎯 Game Rules

**Objective:** Be the last player standing by eliminating others through strategic actions.

### Basic Rules
- Each player has a role with unique abilities
- Players take turns performing one action
- Players with 10+ coins **must** perform a coup
- Last player alive wins

### Actions (All Players Can Do)
1. **Gather** - Get 1 coin (free action)
2. **Tax** - Get 2 coins 
3. **Arrest** - Take 1 coin from another player (can't target same player twice)
4. **Sanction** - Pay 3 coins to block target's economic actions until their next turn
5. **Bribe** - Pay 4 coins to get an extra action this turn
6. **Coup** - Pay 7 coins to eliminate a player from the game

## 🎭 Player Roles

### Governor
- **Tax**: Gets **3 coins** instead of 2
- **Special**: Can cancel another player's tax action

### Spy  
- **Peek**: See another player's coins and prevent their arrest next turn (free action)

### Baron
- **Invest**: Pay 3 coins to get 6 coins back (net +3 coins)
- **Passive**: When sanctioned, gets 1 coin compensation

### General
- **Block Coup**: Pay 5 coins to prevent any coup (attacker loses their coins)
- **Passive**: When arrested, gets the stolen coin back

### Judge
- **Block Bribe**: Cancel another player's bribe (they lose 4 coins)
- **Passive**: When sanctioned, sanctioner pays extra coin to treasury

### Merchant
- **Passive**: Gets 1 free coin at turn start if having 3+ coins
- **Passive**: When arrested, pays 2 coins to treasury instead of losing 1

## 🚀 How to Run

### Build and Run Game
```bash
make Main      # Compiles and runs the game
```

### Run Tests
```bash
make test      # Compiles and runs all unit tests
./test         # Run tests directly
```

### Other Commands
```bash
make build     # Build executable only
make clean     # Remove build files
make valgrind  # Check for memory leaks (with NVIDIA driver suppression)
make valgrind-detailed # Detailed leak analysis
make valgrind-no-suppress # See all leaks including driver ones
```

## 📁 Project Files

- `main.cpp` - Main game with GUI
- `game.hpp/cpp` - Game logic and state management
- `player.hpp/cpp` - Base player class
- `governor.hpp/cpp`, `spy.hpp/cpp`, etc. - Role implementations
- `test_coup.cpp` - Unit tests
- `Makefile` - Build configuration
- `Demo.cpp` - Demo program

## 🎮 Quick Start

1. **Compile and run:**
   ```bash
   make Main
   ```

2. **Run tests to verify everything works:**
   ```bash
   make test
   ```

3. **Clean up when done:**
   ```bash
   make clean
   ```
