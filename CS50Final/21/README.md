# Blackjack Game (21) in C

Final Challenge (C Programming)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

## Table of Contents
- [Github and Edex](#github-and-edex)
- [City and Country](#city-and-country)
- [Date](#date)
- [Description](#description)
- [Installation](#installation)
- [Technologies Used](#technologies-used)
- [Features](#features)
- [Usage Information](#usage-information)
- [Eample-Gameplay](#Example-Gameplay)
- [Suggested Future Development](#suggested-future-development)
- [License](#license)
- [Questions](#questions)

## Name

- **Alfredo Salayandia**

## Github and Edex

- **Donsidious**

## City and Country

- **Brighton, USA**

## Date

- **2024**

## Description

This is a simple text based Blackjack (21) game developed in C. The game simulates the popular card game where players attempt to achieve a hand value closest to 21 without exceeding it, competing against a dealer. The game includes the following features:

- **Card Drawing**: Both player and dealer draw cards, with face cards (Jack, Queen, King) worth 10 points and Ace worth either 1 or 11.
- **Betting System**: Players place a bet using their available chips before each round.
- **Statistics**: Player stats, including wins, losses, ties, and total chips, are saved in an SQLite database.
- **Win/Loss Analysis**: View your win/loss/tie ratios after each game session.
- **Persistent Player State**: Saves and loads player data (chips, games played, wins, losses, ties) between sessions.

This game was created to practice basic C programming skills, including handling user input, implementing game logic, and working with databases.

## Installation

To run this game locally, follow the steps below:

1. **Clone the repository**:

    ```bash
    git clone https://github.com/Donsidious/21
    cd CS50Final
    ```

2. **Install SQLite**:
   Ensure that SQLite is installed on your machine. You can download it from the official website: [SQLite Download](https://www.sqlite.org/download.html).

3. **Compile the code**:

    Use the following command to compile the program:

    ```bash
    gcc -o 21 21.c -lsqlite3
    ```

4. **Run the game**:

    Launch the game by running the compiled program:

    ```bash
    ./21
    ```

    This will start the game in your terminal. You will be prompted to place bets and play against the dealer.

## Technologies Used

This project utilizes the following technologies:

- **C Programming Language** (v11.2.0)
- **SQLite** for database management (v3.36)
- **GCC** (GNU Compiler Collection) for compiling C code

## Features

Key features of the Blackjack game include:

- **Single-player** gameplay against a computer-controlled dealer.
- **Database Integration** to store player data (chips, wins, losses, ties).
- **Betting System** allowing players to place bets each round.
- **Real-time Feedback** after each round showing the outcome and updated stats.
- **Win/Loss/Tie Analysis** showing the player’s win/loss ratio.
- **Persistent Player State** saved and loaded between game sessions.

## Usage Information

This game is intended to be played via the terminal:

1. **Start the Game**: After launching, the game will display your current chip count and ask you to place a bet.
2. **Player's Turn**: You can choose to "hit" (draw a card) or "stand" (end your turn). The dealer automatically plays according to Blackjack rules (draws until reaching at least 17).
3. **Game Outcome**: The result of each round (win, loss, or tie) will be shown, and your chip count will be updated.
4. **View Statistics**: You can view your win/loss/tie ratios and total games played.
5. **Save & Resume**: Player data is saved after each session, and the next time you launch the game, your stats will be automatically loaded.


### Example Gameplay:

### Suggested Future Development: 
looking into creating alogrithms one is able to you and play around with.

1. **Fibonacci Sequence**: Implement a program that generates the Fibonacci sequence (1, 1, 2, 3, 5, 8, 13, 21, etc.). Other alogrithms might to be implemented in the future.

2. **Terminal Interface**: Create a better terminal interface allowing the process of payments to be made.

3. **Ading a better terminal experience**.


### License

This project is licensed under the MIT License. For more information, see the [LICENSE](LICENSE) file.


### Questions

If you have any questions or suggestions, please contact me at [genzdegen@tuta.io](mailto:genzdegen@tuta.io).




    
