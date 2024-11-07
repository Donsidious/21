Blackjack Game (21) in C

FInal Challenge (C Programming)



Table of Contents

Description
Installation
Technologies Used
Features
Usage Information
Suggested Future Development
Contribution Guidelines
Test Instructions
License
Questions
Description

This application is a simple text based Blackjack game built using the C programming language. The game allows the player to play against a dealer in a standard 21 game, where the goal is to have a higher hand value than the dealer without exceeding 21.

The game tracks player statistics such as total chips, wins, losses, ties, and total games played, which are saved using an SQLite database. Players can also view their win/loss ratio after each session.

The game is designed to be played in the terminal, and the player can choose to "hit" (draw a card) or "stand" (end their turn). A series of rules for busting (going over 21) and winning conditions are implemented for both the player and the dealer.

Installation

To run this game locally, follow the steps below:

Clone the repository from GitHub.
Install SQLite on your machine if it’s not already installed. For installation instructions, visit the SQLite download page.
Use a C compiler (such as GCC) to compile the program:
gcc -o blackjack blackjack.c -lsqlite3
Run the compiled game:
./blackjack
This will launch the game and display your current chip count.

Technologies Used

This application is built using the following technologies:

C Programming Language (v11.2.0)
SQLite for database management
GCC (GNU Compiler Collection) for compiling the C code
Standard C libraries (stdio.h, stdlib.h, time.h, sqlite3.h)
Features

Key features of the Blackjack game:

Game Mechanics: Standard Blackjack game rules (player vs. dealer).
Database Integration: Tracks game results, player statistics, and progress in an SQLite database.
Player Actions: The player can "hit" (draw a card) or "stand" (end their turn).
Win/Loss/Tie Statistics: View your win/loss/tie ratio after each game.
Persistent State: Player's chip count and game statistics are saved and loaded between sessions.
Usage Information

This game is intended to be played through the terminal. Once launched, you will be prompted to place a bet. After each round, you can choose to play again if you have enough chips. If you run out of chips, the game ends.

Start the Game: On launch, the game will load your saved state (if available) from the database.
Place Your Bet: Enter the number of chips you want to bet (must be between 1 and your current chip count).
Player Turn: You will be given two cards, and you can choose to "hit" or "stand".
Dealer Turn: After your turn, the dealer will automatically play according to Blackjack rules (hits until they have at least 17).
Result: After each round, the game will display whether you won, lost, or tied, and your chip count will be updated.
Statistics: At any time, you can see your win/loss/tie ratio.
Suggested Future Development

Possible improvements and additions for future versions:

Graphical User Interface (GUI): Transition from the terminal interface to a GUI-based system using a library like GTK or SDL.
Multiplayer Support: Allow multiple players to play against each other or against a computer-controlled dealer.
Bet Limits: Introduce betting limits or special chips for advanced gameplay.
More Detailed Stats: Track additional statistics like average hand value, biggest win/loss, etc.
Sound Effects/Animations: Add sound effects or ASCII art animations to enhance the gameplay experience.
Contribution Guidelines

Contributions are welcome! To contribute:

Fork the repository.
Create a new branch for your feature or bug fix.
Make your changes and test thoroughly.
Submit a pull request explaining the changes you made.
Please ensure that your code adheres to the existing coding style and passes any tests before submitting your pull request.

Test Instructions

Currently, there are no unit tests written for this application. However, manual testing is essential to ensure the correct functionality of the game.

Test the flow of the game by placing different types of bets and playing multiple rounds.
Ensure that your chip count updates correctly and that game results are saved to the database.
License

This project is licensed under the MIT License. See the LICENSE file for more information.

Questions

Have additional questions? Feel free to reach out to me via:

GitHub donsidious
Email: genzdegen@tuta.io

