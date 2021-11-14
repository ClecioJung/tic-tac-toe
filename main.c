/*
 *------------------------------------------------------------------------------
 * LIBRARIES
 *------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*
 *------------------------------------------------------------------------------
 * DEFINITIONS
 *------------------------------------------------------------------------------
 */

// Marks
#define CROSS 'X'
#define NOUGHT 'O'
#define EMPTY ' '

#define BOARD_SIZE 3

typedef enum
{
    ttt_continue = -2,
    ttt_player_win = -1,
    ttt_draw = 0,
    ttt_computer_win = 1,
} ttt_state;

/*
 *------------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *------------------------------------------------------------------------------
 */

char board[BOARD_SIZE][BOARD_SIZE];

/*
 *------------------------------------------------------------------------------
 * FUNCTIONS
 *------------------------------------------------------------------------------
 */

char getReadableChar(void)
{
    char c;
    do
    {
        c = getc(stdin);
    } while (c < 32);
    fflush(stdin);
    return c;
}

void printBoard(void)
{
    printf("\n    A   B   C\n");
    for (unsigned int line = 0; line < BOARD_SIZE; line++)
    {
        printf(" %d:", (line + 1));
        for (unsigned int col = 0; col < BOARD_SIZE; col++)
        {
            printf(" %c ", board[line][col]);
            if (col != (BOARD_SIZE - 1))
            {
                putchar('|');
            }
        }
        if (line != (BOARD_SIZE - 1))
        {
            printf("\n   -----------\n");
        }
    }
    putchar('\n');
}

void getPlayerMove(const char player)
{
    int line, col;
getColumn:
    printf("Which column would you like to mark? (A/B/C) ");
    col = toupper(getReadableChar()) - 'A';
    if ((col < 0) || (col > (BOARD_SIZE - 1)))
    {
        printf("Incorrect column informed!\n");
        goto getColumn;
    }
getLine:
    printf("Which line would you like to mark? (1/2/3) ");
    line = getReadableChar() - '1';
    if ((line < 0) || (line > (BOARD_SIZE - 1)))
    {
        printf("Incorrect line informed!\n");
        goto getLine;
    }
    if (board[line][col] != EMPTY)
    {
        printf("This house is already occupied!\n");
        goto getColumn;
    }
    board[line][col] = player;
}

ttt_state gameState(const char player, const char computer)
{
    ttt_state state = ttt_draw;
    // Index tables for winning combinations
    static const unsigned int lineWinning[8][BOARD_SIZE] =
        {
            {0, 1, 2},
            {0, 1, 2},
            {0, 1, 2},
            {0, 0, 0},
            {1, 1, 1},
            {2, 2, 2},
            {0, 1, 2},
            {0, 1, 2},
        };
    static const unsigned int colWinning[8][BOARD_SIZE] =
        {
            {0, 0, 0},
            {1, 1, 1},
            {2, 2, 2},
            {0, 1, 2},
            {0, 1, 2},
            {0, 1, 2},
            {0, 1, 2},
            {2, 1, 0},
        };
    for (unsigned int line = 0; line < 8; line++)
    {
        unsigned int player_cont = 0;
        unsigned int computer_cont = 0;
        for (unsigned int col = 0; col < BOARD_SIZE; col++)
        {
            if (board[lineWinning[line][col]][colWinning[line][col]] == player)
            {
                player_cont++;
            }
            else if (board[lineWinning[line][col]][colWinning[line][col]] == computer)
            {
                computer_cont++;
            }
        }
        if (player_cont == BOARD_SIZE)
        {
            return ttt_player_win;
        }
        else if (computer_cont == BOARD_SIZE)
        {
            return ttt_computer_win;
        }
        else if ((player_cont == 0) || (computer_cont == 0))
        {
            state = ttt_continue;
        }
    }
    return state;
}

int minimax(const int whoIsPlaying, const char player, const char computer)
{
    ttt_state score = gameState(player, computer);
    // If the game has ended, returns 1 for player win and -1 for computer win
    if (score != ttt_continue)
    {
        return (whoIsPlaying * score);
    }
    else
    {
        for (unsigned int coluna = 0; coluna < BOARD_SIZE; coluna++)
        {
            for (unsigned int linha = 0; linha < BOARD_SIZE; linha++)
            {
                if (board[linha][coluna] == EMPTY)
                {
                    board[linha][coluna] = (whoIsPlaying == 1) ? computer : player;
                    ttt_state thisScore = -minimax(-whoIsPlaying, player, computer);
                    board[linha][coluna] = EMPTY;
                    if (thisScore > score)
                    {
                        score = thisScore;
                    }
                }
            }
        }
    }
    if (score == ttt_continue)
    {
        return ttt_draw;
    }
    return score;
}

void getComputerMove(const int whoIsPlaying, const char player, const char computer)
{
    unsigned int moveLine, moveCol;
    ttt_state score = ttt_continue;
    for (unsigned int col = 0; col < BOARD_SIZE; col++)
    {
        for (unsigned int line = 0; line < BOARD_SIZE; line++)
        {
            if (board[line][col] == EMPTY)
            {
                board[line][col] = (whoIsPlaying == 1) ? computer : player;
                ttt_state thisScore = -minimax(-whoIsPlaying, player, computer);
                board[line][col] = EMPTY;
                if (thisScore > score)
                {
                    moveLine = line;
                    moveCol = col;
                    score = thisScore;
                }
            }
        }
    }
    printf("The computer will play at column %c and line %d\n", 'A' + (char)moveCol, moveLine + 1);
    board[moveLine][moveCol] = computer;
}

/*
 *------------------------------------------------------------------------------
 * MAIN
 *------------------------------------------------------------------------------
 */

int main(void)
{
    ttt_state state = ttt_continue;

    printf("Which character would you like to play? (X/O) ");
    char player = getReadableChar();
    char computer = (toupper(player) != CROSS) ? CROSS : NOUGHT;

    printf("Would you like to be the first to play? (Y/N) ");
    int whoIsPlaying = (toupper(getReadableChar()) == 'Y') ? -1 : 1;
    putchar('\n');

    printf("Player: %c\n", player);
    printf("Computer: %c\n", computer);

    memset(board, EMPTY, BOARD_SIZE * BOARD_SIZE);
    printBoard();

    while (state == ttt_continue)
    {
        if (whoIsPlaying == -1)
        {
            getPlayerMove(player);
        }
        else
        {
            getComputerMove(whoIsPlaying, player, computer);
        }
        printBoard();
        whoIsPlaying *= -1;
        state = gameState(player, computer);
    }

    switch (state)
    {
    case ttt_player_win:
        printf("Congratulations! You Won!\n");
        break;
    case ttt_computer_win:
        printf("You Lost! Good luck next time!\n");
        break;
    case ttt_draw:
        printf("The game ended in a draw!\n");
        break;
    case ttt_continue:
    default:
        printf("This is probably a bug in this software!\n");
        break;
    }
    return EXIT_SUCCESS;
}

/*
 *------------------------------------------------------------------------------
 * END
 *------------------------------------------------------------------------------
 */