#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Op.c"

void ConsoleClean();
//void TableCoppy(int target[8][8], int source[8][8]);

//void UndoPlay(int table[8][8], Player Players[3], int playerNum);
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

    int playerNum = 1;
    while (PASS)
    {
        playerNum = PlayGame(table, Players, playerNum, &is_endGame);
        if (playerNum > 2)
            playerNum = 1;

        if (is_endGame >= 2)
            break;
    }

    WinnerFinde(table, Players);

    return 0;
}

void ConsoleClean()
{
#ifdef _WIN32
    system("cls");
#elif __linux__
    system("clear");
#endif
}

void Show(int table[8][8])
{
    // To char* to create table structure
    char *top_buttom_Square = "-------";
    char *sideSquare = "|";

    ConsoleClean(); // To Clean console and overwrite next information.

    // Print number of column.
    printf("%7d %7d %7d %7d %7d %7d %7d %7d\n", 1, 2, 3, 4, 5, 6, 7, 8);

    for (int i = 0; i < 8; i++)
    {
        // Print '-----' in top of column.
        printf("%10s %s %s %s %s %s %s %s\n", top_buttom_Square, top_buttom_Square,
               top_buttom_Square, top_buttom_Square, top_buttom_Square, top_buttom_Square, top_buttom_Square, top_buttom_Square);

        // Print '|' in Top of row.
        printf("%3s %7s %7s %7s %7s %7s %7s %7s %7s\n", sideSquare, sideSquare,
               sideSquare, sideSquare, sideSquare, sideSquare, sideSquare, sideSquare, sideSquare);

        // Print number of row.
        printf("%d ", i + 1);

        for (int k = 0; k < 8; k++)
        {
            printf("|   ");

            // print o or # if we have nut and print " " if nothing.
            if (table[i][k] == 1)
                printf("#  ");
            else if (table[i][k] == 2)
                printf("o  ");
            else
                printf("   ");
            // end if

            printf(" ");
        }

        // Print '|' for in middle row.
        printf("%s\n%3s %7s %7s %7s %7s %7s %7s %7s %7s\n", sideSquare, sideSquare, sideSquare,
               sideSquare, sideSquare, sideSquare, sideSquare, sideSquare, sideSquare, sideSquare);
    }

    // Print '|' for in end of row.
    printf("%10s %s %s %s %s %s %s %s\n", top_buttom_Square, top_buttom_Square,
           top_buttom_Square, top_buttom_Square, top_buttom_Square, top_buttom_Square, top_buttom_Square, top_buttom_Square);

    
}

void NewGame(int table[8][8])
{
    /*for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            table[r][c] = 1;
        }
    }*/

    /*table[0][6] = 2;
    table[6][6] = 0;
    table[7][7] = 0;*/

    table[3][3] = 2;
    table[4][3] = 1;

    table[3][4] = 1;
    table[4][4] = 2;

    Show(table);
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
        Show(table);
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
            Show(table);
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

