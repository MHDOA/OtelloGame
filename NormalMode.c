#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "OtelloFuncs.c"

void ConsoleClean();

typedef int (*PlayGames)(int **, Player *, int, int *);

void NewGame(int table[8][8]);
int PlayGame(int table[8][8], Player Players[3], int playerNum, int *is_endGame);

int main()
{
    int is_endGame = 0;

    Player Players[3];
    Players[1].nut[0] = '#';
    Players[2].nut[0] = 'O';
    scanf("%20s %20s", Players[1].name, Players[2].name);

    int table[8][8] = {0};
    NewGame(table);
    Show(table, Players, 0);

    int playerNum = 1;
    while (PASS)
    {
        playerNum = PlayGameNormal(table, Players, playerNum, &is_endGame);
        if (playerNum > 2)
            playerNum = 1;

        if (is_endGame >= 2)
            break;
    }

    WinnerFinde(table, Players, 0);

    return 0;
}

int PlayGameNormal(int table[8][8], Player Players[3], int playerNum, int *is_endGame)
{
    PlayGames p = &PlayGame;

    printf("Add\n");
    p(table, Players, playerNum, is_endGame);
}

int PlayGame(int table[8][8], Player Players[3], int playerNum, int *is_endGame)
{
    int r, c;

    // Find impossible way to find game over situation.
    enum conditions condition = IsCorrectMove(table, 1, 1, playerNum); // Send test point
    if (condition == IMPOSSIBLE)
    {
        *(is_endGame) += 1;
        playerNum++;
        return playerNum;
    }

    printf("Enter your location %s(%s): ", Players[playerNum].name, Players[playerNum].nut);
    char input[3];
    scanf("%2s", input);

    if (strcmp(input, "z\0") == 0)
    {
        UndoPlay(table, Players, playerNum);

        Players[1].nutsNumber = NutsCounter(table, 1);
        Players[2].nutsNumber = NutsCounter(table, 2);

        Show(table, Players, 0);
    }

    else
    {
        char row[2] = {input[0], '\0'};
        char col[2] = {input[1], '\0'};
        r = atoi(row);
        c = atoi(col);

        condition = IsCorrectMove(table, r, c, playerNum);

        if (condition == PASS)
        {
            TableCoppy(Players[playerNum].table, table);
            table[r - 1][c - 1] = playerNum;
            ReverseNuts(table, r, c, playerNum, Players);

            Players[1].nutsNumber = NutsCounter(table, 1);
            Players[2].nutsNumber = NutsCounter(table, 2);

            Show(table, Players, 0);
            playerNum++;
            *is_endGame = 0;
        }

        else if (condition == AGAIN)
        {
            printf("try again %s\n", Players[playerNum].name);
            *is_endGame = 0;
        }
    }

    return playerNum;
}
