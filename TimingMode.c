#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "OtelloFuncs.c"

void ConsoleClean();

typedef int (*PlayGames)(int **, Player *, int, int *);
typedef void (*ShowTable)(int **);

void NewGame(int table[8][8]);
int PlayGame(int table[8][8], Player Players[3], int playerNum, int *is_endGame);

int main()
{
    int is_endGame = 0;

    // Define Players list
    Player Players[3];
    Players[1].nut[0] = '#';
    Players[2].nut[0] = 'O';
    Players[1].nutsNumber = 2;
    Players[2].nutsNumber = 2;
    Players[1].score = 0;
    Players[2].score = 0;

    scanf("%20s %20s", Players[1].name, Players[2].name);

    // Get Time
    printf("Enter your time(90s to 60,000s): ");
    int timeTmp;
    scanf("%d", &timeTmp);
    if (timeTmp > 60000)
        timeTmp = 60000;
    // if(timeTmp < 90) timeTmp = 90;

    Players[1].time = timeTmp;
    Players[2].time = timeTmp;

    // Initial new game table
    int table[8][8] = {0};
    NewGame(table);
    Show(table, Players, 1);

    // Start game
    int playerNum = 1;
    while (PASS)
    {
        playerNum = PlayGame(table, Players, playerNum, &is_endGame);

        // Player number just 1 or 2.
        if (playerNum > 2)
            playerNum = 1;

        // When game over
        if (is_endGame >= 2 || Players[1].time <= 0 || Players[2].time <= 0)
            break;
    }

    // Show winner
    WinnerFinde(table, Players, 1);

    return 0;
}

int PlayGame(int table[8][8], Player Players[3], int playerNum, int *is_endGame)
{
    int r, c;

    // Time didn't pause when play again.
    static int is_playAgain = 0;
    time_t LastTime, CurrentTime;

    // Find impossible way to find game over situation.
    enum conditions condition = IsCorrectMove(table, 1, 1, playerNum); // Send test point
    if (condition == IMPOSSIBLE)
    {
        *(is_endGame) += 1;
        playerNum++;
        return playerNum;
    }

    printf("Enter your location %s(%s): ", Players[playerNum].name, Players[playerNum].nut);

    // Start Timer
    if (is_playAgain == 0)
        time(&LastTime);

    char input[3];
    scanf("%2s", input);

    // Undo Play
    if (strcmp(input, "z\0") == 0)
    {
        UndoPlay(table, Players, playerNum);

        Players[1].score = Players[1].lastScore;
        Players[2].score = Players[2].lastScore;

        Players[1].nutsNumber = NutsCounter(table, 1);
        Players[2].nutsNumber = NutsCounter(table, 2);
        Show(table, Players, 1);
    }

    else
    {
        // Set row and col
        char row[2] = {input[0], '\0'};
        char col[2] = {input[1], '\0'};
        r = atoi(row);
        c = atoi(col);

        condition = IsCorrectMove(table, r, c, playerNum);

        if (condition == PASS)
        {
            // Process Nuts state
            TableCoppy(Players[playerNum].table, table);
            table[r - 1][c - 1] = playerNum;

            Players[playerNum].lastScore = Players[playerNum].score;

            ReverseNuts(table, r, c, playerNum, Players);

            Players[1].nutsNumber = NutsCounter(table, 1);
            Players[2].nutsNumber = NutsCounter(table, 2);

            // Calculate spent time
            time(&CurrentTime);
            Players[playerNum].time += LastTime - CurrentTime;
            is_playAgain = 0;

            // Show final table
            Show(table, Players, 1);
            playerNum++;
            *is_endGame = 0;
        }

        else if (condition == AGAIN)
        {
            printf("try again %s\n", Players[playerNum].name);
            is_playAgain = 1;
            *is_endGame = 0;
        }
    }

    return playerNum;
}
