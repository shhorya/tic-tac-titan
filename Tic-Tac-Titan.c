#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

typedef enum {
    EASY = 1,
    MEDIUM,
    HARD
} Difficulty;

char board[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

void printBoard();
bool checkWin(char player);
bool isBoardFull();
int evaluate();
int minimax(bool isAI);
int findBestMove();
void playerMove();
void aiMove(Difficulty difficulty);
void resetBoard();
int getDifficultyLevel();

void resetBoard() {
    for (int i = 0; i < 9; i++) {
        board[i] = '1' + i;
    }
}

void printBoard() {
    printf("\n%sTic%s %sTac%s %sTitan%s\n\n", GREEN, RESET, RED, RESET, YELLOW, RESET);
    for (int i = 0; i < 9; i++) {
        if (board[i] == 'X') {
            printf(RED " %c " RESET, board[i]);
        } else if (board[i] == 'O') {
            printf(GREEN " %c " RESET, board[i]);
        } else {
            printf(" %c ", board[i]);
        }

        if ((i + 1) % 3 == 0) {
            printf("\n");
            if (i < 8) printf("---+---+---\n");
        } else {
            printf("|");
        }
    }
    printf("\n");
}

bool checkWin(char player) {
    int wins[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (int i = 0; i < 8; i++) {
        if (board[wins[i][0]] == player &&
            board[wins[i][1]] == player &&
            board[wins[i][2]] == player) {
            return true;
        }
    }
    return false;
}

bool isBoardFull() {
    for (int i = 0; i < 9; i++) {
        if (board[i] != 'X' && board[i] != 'O') {
            return false;
        }
    }
    return true;
}

int evaluate() {
    if (checkWin('O')) return 10;
    if (checkWin('X')) return -10;
    return 0;
}

int minimax(bool isAI) {
    int score = evaluate();

    if (score == 10 || score == -10)
        return score;

    if (isBoardFull())
        return 0;

    if (isAI) {
        int best = -1000;

        for (int i = 0; i < 9; i++) {
            if (board[i] != 'X' && board[i] != 'O') {

                board[i] = 'O';

                int moveVal = minimax(false);

                if (moveVal > best)
                    best = moveVal;

                board[i] = '1' + i;
            }
        }
        return best;
    } else {
        int best = 1000;

        for (int i = 0; i < 9; i++) {
            if (board[i] != 'X' && board[i] != 'O') {

                board[i] = 'X';

                int moveVal = minimax(true);

                if (moveVal < best)
                    best = moveVal;

                board[i] = '1' + i;
            }
        }
        return best;
    }
}

int findBestMove() {
    int bestVal = -1000;
    int bestMove = -1;

    for (int i = 0; i < 9; i++) {
        if (board[i] != 'X' && board[i] != 'O') {

            board[i] = 'O';

            int moveVal = minimax(false);
            
            board[i] = '1' + i;

            if (moveVal > bestVal) {
                bestMove = i;
                bestVal = moveVal;
            }
        }
    }
    return bestMove;
}

void playerMove() {
    int move;
    printf("Your move (choose 1-9): ");
    while (true) {
        if (scanf("%d", &move) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number between 1 and 9: ");
            continue;
        }

        if (move >= 1 && move <= 9 && board[move - 1] != 'X' && board[move - 1] != 'O') {
            board[move - 1] = 'X';
            break;
        } else {
            printf("Invalid move, try again: ");
        }
    }
}

void aiMove(Difficulty difficulty) {
    int bestMove;

    srand(time(0));

    if (difficulty == EASY) {
        
        int availableMoves[9];
        int count = 0;
        for (int i = 0; i < 9; i++) {
            if (board[i] != 'X' && board[i] != 'O') {
                availableMoves[count++] = i;
            }
        }
        if (count > 0) {
            bestMove = availableMoves[rand() % count];
            
            board[bestMove] = 'O';
        }
    }
    else if (difficulty == MEDIUM) {

        int chance = rand() % 100;
        if (chance < 55) {
            bestMove = findBestMove();
            
            board[bestMove] = 'O';
        }
        else {
            int availableMoves[9];
            int count = 0;
            for (int i = 0; i < 9; i++) {
                if (board[i] != 'X' && board[i] != 'O') {
                    availableMoves[count++] = i;
                }
            }
            if (count > 0) {
                bestMove = availableMoves[rand() % count];
                
                board[bestMove] = 'O';
            }
        }
    }
    else if (difficulty == HARD) {
        
        bestMove = findBestMove();
       
        board[bestMove] = 'O';
    }
}


int getDifficultyLevel() {
    int choice;
    printf("Select Difficulty Level:\n");
    printf("%s1. Easy%s\n", GREEN, RESET);
    printf("%s2. Medium%s\n", YELLOW, RESET);
    printf("%s3. Hard%s\n",RED, RESET);

    printf("Enter your choice (1-3): ");
    while (true) {
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number between 1 and 3: ");
            continue;
        }

        if (choice >= 1 && choice <= 3) {
            while (getchar() != '\n');
            return choice;
        }
        else {
            printf("Invalid choice. Please enter a number between 1 and 3: ");
        }
    }
}

int main() {
    int turn;
    char playAgain;
    Difficulty difficulty;

    srand(time(0));

    printf("Welcome to %sTic%s %sTac%s %sTitan%s\n\n", GREEN, RESET, RED, RESET, YELLOW, RESET);

    difficulty = (Difficulty)getDifficultyLevel();
    printf("\n=== Starting a New Game ===\n");

    do {
        turn = 1;
        resetBoard();
        printBoard();

        while (true) {
            if (turn % 2 == 1) {
                
                playerMove();
            } else {
                
                aiMove(difficulty);
            }

            printBoard();
            
            if (checkWin('X')) {
                printf(RED "Congratulations! You win!\n" RESET);
                break;
            }
            else if (checkWin('O')) {
                printf(GREEN "AI wins! Better luck next time!\n" RESET);
                break;
            }
            else if (isBoardFull()) {
                printf(YELLOW "It's a tie!\n" RESET);
                break;
            }
            turn++;
        }
        
        printf("Do you want to play again? (y/n): ");
        
        while ((getchar()) != '\n');
        scanf(" %c", &playAgain);

        if (playAgain == 'y' || playAgain == 'Y') {
            printf("\n=== Starting a New Game ===\n\n");
            
            printf("Do you want to change the difficulty level? (y/n): ");
            char changeDifficulty;
            while ((getchar()) != '\n');
            scanf(" %c", &changeDifficulty);
            if (changeDifficulty == 'y' || changeDifficulty == 'Y') {
                difficulty = (Difficulty)getDifficultyLevel();
                printf("\n=== Difficulty Level Changed ===\n\n");
            }
        }

    } while (playAgain == 'y' || playAgain == 'Y');

    printf("Thank you for playing!\n");
    return 0;
}
