#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sqlite3.h>
#include <string.h>

// Function Prototypes
void playGame(int *chips);
int drawCard();
int getHandValue(int hand[], int numCards);
void displayHand(int hand[], int numCards);
bool playerTurn(int *playerHand, int *numPlayerCards);
bool dealerTurn(int *dealerHand, int *numDealerCards);
void updateChips(int result, int *chips, int bet);
void initializeDatabase();
void storeGameResult(const char *result, int bet, int chipsWonLost, int startingChips, int endingChips);
void analyzeWinLossRatio();
void saveUserState(int chips, int gamesPlayed, int wins, int losses, int ties);
void loadUserState(int *chips, int *gamesPlayed, int *wins, int *losses, int *ties);

// Simplified ASCII Art for cards
const char *cardArt[] = {
    "┌─────┐\n│A    │\n│     │\n│    A│\n└─────┘", // Ace
    "┌─────┐\n│2    │\n│     │\n│    2│\n└─────┘", // 2
    "┌─────┐\n│3    │\n│     │\n│    3│\n└─────┘", // 3
    "┌─────┐\n│4    │\n│     │\n│    4│\n└─────┘", // 4
    "┌─────┐\n│5    │\n│     │\n│    5│\n└─────┘", // 5
    "┌─────┐\n│6    │\n│     │\n│    6│\n└─────┘", // 6
    "┌─────┐\n│7    │\n│     │\n│    7│\n└─────┘", // 7
    "┌─────┐\n│8    │\n│     │\n│    8│\n└─────┘", // 8
    "┌─────┐\n│9    │\n│     │\n│    9│\n└─────┘", // 9
    "┌─────┐\n│10   │\n│     │\n│   10│\n└─────┘", // 10
    "┌─────┐\n│J    │\n│     │\n│    J│\n└─────┘", // Jack
    "┌─────┐\n│Q    │\n│     │\n│    Q│\n└─────┘", // Queen
    "┌─────┐\n│K    │\n│     │\n│    K│\n└─────┘"  // King
};

// This is where we start the game 
int main() {
    initializeDatabase(); 

    int chips, gamesPlayed, wins = 0, losses = 0, ties = 0;
    loadUserState(&chips, &gamesPlayed, &wins, &losses, &ties);

    char choice;
    srand(time(NULL));

    printf("Welcome back to 21!\nYou have %d chips.\n", chips);
    analyzeWinLossRatio();

    do {
        gamesPlayed++;
        playGame(&chips);

        printf("You now have %d chips.\n", chips);
        printf("Games played: %d\n", gamesPlayed);

        if (chips <= 0) {
            printf("You are out of chips! Game over.\n");
            break;
        }

        printf("Do you want to play another game? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    saveUserState(chips, gamesPlayed, wins, losses, ties);
    printf("Thanks for playing! You played %d games and ended with %d chips.\n", gamesPlayed, chips);
    return 0;
}

void analyzeWinLossRatio() {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int wins = 0, losses = 0, ties = 0;

    int rc = sqlite3_open("21.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char *sql = "SELECT result, COUNT(*) FROM games GROUP BY result;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *result = sqlite3_column_text(stmt, 0);
        int count = sqlite3_column_int(stmt, 1);

        if (strcmp((const char*)result, "win") == 0) wins = count;
        else if (strcmp((const char*)result, "loss") == 0) losses = count;
        else if (strcmp((const char*)result, "tie") == 0) ties = count;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    int totalGames = wins + losses + ties;
    printf("Win/Loss/Tie Ratios:\n");
    printf("Wins: %d (%.2f%%)\n", wins, totalGames > 0 ? (wins / (float)totalGames) * 100 : 0);
    printf("Losses: %d (%.2f%%)\n", losses, totalGames > 0 ? (losses / (float)totalGames) * 100 : 0);
    printf("Ties: %d (%.2f%%)\n", ties, totalGames > 0 ? (ties / (float)totalGames) * 100 : 0);
    printf("Total Games Played: %d\n", totalGames);
}

void initializeDatabase() {
    sqlite3 *db;
    char *errMsg = 0;

    int rc = sqlite3_open("21.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS games ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "result TEXT, "
                      "bet INTEGER, "
                      "chips_won_lost INTEGER, "
                      "starting_chips INTEGER, "
                      "ending_chips INTEGER);";
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error (games table): %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    const char *userStateSql = "CREATE TABLE IF NOT EXISTS user_state ("
                               "user_id INTEGER PRIMARY KEY, "
                               "chips INTEGER, "
                               "games_played INTEGER, "
                               "wins INTEGER, "
                               "losses INTEGER, "
                               "ties INTEGER);";
    rc = sqlite3_exec(db, userStateSql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error (user_state table): %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}

void storeGameResult(const char *result, int bet, int chipsWonLost, int startingChips, int endingChips) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    int rc = sqlite3_open("21.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char *sql = "INSERT INTO games (result, bet, chips_won_lost, starting_chips, ending_chips) "
                      "VALUES (?, ?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, result, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, bet);
    sqlite3_bind_int(stmt, 3, chipsWonLost);
    sqlite3_bind_int(stmt, 4, startingChips);
    sqlite3_bind_int(stmt, 5, endingChips);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void playGame(int *chips) {
    int playerHand[10], dealerHand[10];
    int numPlayerCards = 0, numDealerCards = 0;
    int bet;

    // Get player's bet
    printf("You have %d chips. Enter your bet: ", *chips);
    scanf("%d", &bet);
    while (bet > *chips || bet <= 0) {
        printf("Invalid bet. Enter an amount between 1 and %d: ", *chips);
        scanf("%d", &bet);
    }

    // Initial two cards for player and dealer
    playerHand[numPlayerCards++] = drawCard();
    playerHand[numPlayerCards++] = drawCard();
    dealerHand[numDealerCards++] = drawCard();
    dealerHand[numDealerCards++] = drawCard();

    // Player's turn
    printf("\nYour turn:\n");   
    bool playerBust = playerTurn(playerHand, &numPlayerCards);
    if (playerBust) {
        printf("You busted! Dealer wins this round.\n");
        updateChips(-bet, chips, bet);  // Pass result (-bet), chips, and bet
        storeGameResult("loss", bet, -bet, *chips + bet, *chips); // Store loss
        return;
    }

    // Dealer's turn
    printf("\nDealer's turn:\n");
    bool dealerBust = dealerTurn(dealerHand, &numDealerCards);
    if (dealerBust) {
        printf("Dealer busted! You win this round.\n");
        updateChips(bet, chips, bet);  // Player wins, so add bet to chips
        storeGameResult("win", bet, bet, *chips - bet, *chips); // Store win
        return;
    }

    // Determine final scores
    int playerScore = getHandValue(playerHand, numPlayerCards);
    int dealerScore = getHandValue(dealerHand, numDealerCards);

    printf("Your final score: %d\n", playerScore);
    printf("Dealer's final score: %d\n", dealerScore);

    // Decide the winner
    if (playerScore > dealerScore) {
        printf("You win this round!\n");
        updateChips(bet, chips, bet);  // Player wins, so add bet to chips
        storeGameResult("win", bet, bet, *chips - bet, *chips); // Store win
    } else if (playerScore < dealerScore) {
        printf("Dealer wins this round.\n");
        updateChips(-bet, chips, bet);  // Dealer wins, so subtract bet from chips
        storeGameResult("loss", bet, -bet, *chips + bet, *chips); // Store loss
    } else {
        printf("It's a tie!\n");
        storeGameResult("tie", bet, 0, *chips + bet, *chips); // Store tie
    }
}

int drawCard() {
    int card = (rand() % 13) + 1; // 1 to 13, where 1 is Ace, 11 is Jack, 12 is Queen, 13 is King
    return card > 10 ? 10 : card; // Face cards are worth 10
}

// Retturns player's hand 
int getHandValue(int hand[], int numCards) {
    int value = 0, aces = 0;
    for (int i = 0; i < numCards; i++) {
        value += hand[i];
        if (hand[i] == 1) aces++;
    }
    while (aces > 0 && value + 10 <= 21) {
        value += 10;
        aces--;
    }
    return value;
}

void displayHand(int hand[], int numCards) {
    for (int i = 0; i < numCards; i++) {
        int cardIndex = hand[i] - 1; // Subtract 1 to match the array index
        printf("%s\n", cardArt[cardIndex]);
    }
    printf("(Total: %d)\n", getHandValue(hand, numCards));
}

// player's turn
bool playerTurn(int *playerHand, int *numPlayerCards) {
    char choice;
    do {
        displayHand(playerHand, *numPlayerCards);
        printf("Do you want to hit or stand? (h/s): ");
        scanf(" %c", &choice);
        if (choice == 'h' || choice == 'H') {
            playerHand[(*numPlayerCards)++] = drawCard();
            if (getHandValue(playerHand, *numPlayerCards) > 21) {
                return true; // Player busts
            }
        }
    } while (choice == 'h' || choice == 'H');
    return false;
}

bool dealerTurn(int *dealerHand, int *numDealerCards) {
    while (getHandValue(dealerHand, *numDealerCards) < 17) {
        dealerHand[(*numDealerCards)++] = drawCard();
    }
    displayHand(dealerHand, *numDealerCards);
    return getHandValue(dealerHand, *numDealerCards) > 21; // Dealer busts
}

// Update the player's chips based on the result of the game
void updateChips(int result, int *chips, int bet) {
    if (result > 0) {
        *chips += bet;
    } else if (result < 0) {
        *chips -= bet;
    }
}

// Function to store game state
void saveUserState(int chips, int gamesPlayed, int wins, int losses, int ties) {
    sqlite3 *db;
    sqlite3_open("21.db", &db);


    const char *sql = "REPLACE INTO user_state (user_id, chips, games_played, wins, losses, ties) "
                      "VALUES (1, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, chips);
    sqlite3_bind_int(stmt, 2, gamesPlayed);
    sqlite3_bind_int(stmt, 3, wins);
    sqlite3_bind_int(stmt, 4, losses);
    sqlite3_bind_int(stmt, 5, ties);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// load previous user state
void loadUserState(int *chips, int *gamesPlayed, int *wins, int *losses, int *ties) {
    sqlite3 *db;
    sqlite3_open("21.db", &db);

    const char *sql = "SELECT chips, games_played, wins, losses, ties FROM user_state WHERE user_id = 1;";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *chips = sqlite3_column_int(stmt, 0);
        *gamesPlayed = sqlite3_column_int(stmt, 1);
        *wins = sqlite3_column_int(stmt, 2);
        *losses = sqlite3_column_int(stmt, 3);
        *ties = sqlite3_column_int(stmt, 4);

        // If the loaded chips value is 0 or less, set it to 100
        if (*chips <= 0) {
            *chips = 100;
        }
    } else {
        // No saved state, initialize with default values
        *chips = 100;
        *gamesPlayed = 0;
        *wins = 0;
        *losses = 0;
        *ties = 0;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
