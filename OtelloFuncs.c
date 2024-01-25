#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FineTime 30

struct Player
{
    char name[21];

    char nut[1];
    int nutsNumber;

    int score;
    int lastScore;

    int table[8][8];
    
    int time;
    int playTimeLeft;

    int undoUseCounter;
    
};

typedef struct Player Player;

enum conditions
{
    IMPOSSIBLE = -1,
    AGAIN,
    PASS
};

void ConsoleClean()
{
#ifdef _WIN32
    system("cls");
#elif __linux__
    system("clear");
#endif
}

void Show(int table[8][8], Player Players[3], int is_TimingMode)
{
    // To char* to create table structure
    char *top_bottom_Square = "-------";
    char *sideSquare = "|";

    ConsoleClean(); // To Clean console and overwrite next information.

    // Print number of column.
    printf("%7d %7d %7d %7d %7d %7d %7d %7d\n", 1, 2, 3, 4, 5, 6, 7, 8);

    for (int i = 0; i < 8; i++)
    {
        // Print '-----' in top of column.
        printf("%10s %s %s %s %s %s %s %s\n", top_bottom_Square, top_bottom_Square,
               top_bottom_Square, top_bottom_Square, top_bottom_Square, top_bottom_Square, top_bottom_Square, top_bottom_Square);

        // Print '|' in start of row.
        printf("%3s %7s %7s %7s %7s %7s %7s %7s %7s", sideSquare, sideSquare,
               sideSquare, sideSquare, sideSquare, sideSquare, sideSquare, sideSquare, sideSquare);

        if (i == 3)
            printf("%-5sPlayer 1 Nuts: %d", "\0", Players[1].nutsNumber);
        else if (i == 4)
            printf("%-5sPlayer 2 Nuts: %d", "\0", Players[2].nutsNumber);
        printf("\n");

        // Print alphabet of row
        printf("%c ", i + 97);

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

        printf("%s", sideSquare);

        if (is_TimingMode)
        {
            if (i == 3)
                printf("%-5sPlayer 1 Time: %ds", "\0", Players[1].time);
            else if (i == 4)
                printf("%-5sPlayer 2 Time: %ds", "\0", Players[2].time);
        }

        // Print '|' for in end of row.
        printf("\n%3s %7s %7s %7s %7s %7s %7s %7s %7s", sideSquare, sideSquare, sideSquare,
               sideSquare, sideSquare, sideSquare, sideSquare, sideSquare, sideSquare);

        if (i == 3)
            printf("%-5sPlayer 1 Score: %d", "\0", Players[1].score);
        else if (i == 4)
            printf("%-5sPlayer 2 Score: %d", "\0", Players[2].score);

        printf("\n");
    }

    // Print '---' for in end of column.
    printf("%10s %s %s %s %s %s %s %s\n", top_bottom_Square, top_bottom_Square,
           top_bottom_Square, top_bottom_Square, top_bottom_Square, top_bottom_Square, top_bottom_Square, top_bottom_Square);
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

    // Show(table);
}

int MovementChecker(int table[8][8], int r, int c, int playerNum)
{
    r--;
    c--;

    // cell should be empty
    if (table[r][c] != 0)
        return 0;

    for (int dr = -1; dr <= 1; dr++)
    {
        for (int dc = -1; dc <= 1; dc++)
        {
            int flag = 0; // At least should be behind an opposite nut.

            // To skip curent cell
            if (dc == 0 && dr == 0)
                continue;

            int rtmp = r + dr;
            int ctmp = c + dc;

            while (rtmp >= 0 && rtmp < 8 && ctmp >= 0 && ctmp < 8 && table[rtmp][ctmp] == 3 - playerNum)
            {
                flag = 1;
                rtmp += dr;
                ctmp += dc;
            }

            if (rtmp >= 0 && rtmp < 8 && ctmp >= 0 && ctmp < 8 && table[rtmp][ctmp] == playerNum && flag == 1)
                return 1;
        }
    }

    return 0;
}

int AllWays(int table[8][8], int playerNum, int **arr)
{
    int Impossible_Play = 1; // To show do we have any way or not.
    for (int r = 1; r <= 8; r++)
    {
        for (int c = 1; c <= 8; c++)
        {
            if (MovementChecker(table, r, c, playerNum) == 1)
            {
                Impossible_Play = 0;
                *(*(arr + r - 1) + c - 1) = 1;
            }
        }
    }

    if (Impossible_Play == 1)
        return IMPOSSIBLE;
    else
        return PASS;
}

int IsCorrectMove(int table[8][8], int r, int c, int playerNum)
{
    // Save all ways for player in all_way and send to AllWays to find them.
    int **all_ways = (int **)malloc(8 * sizeof(int *));
    for (int i = 0; i < 8; i++)
        *(all_ways + i) = (int *)malloc(8 * sizeof(int));

    enum conditions conditionPlay = AllWays(table, playerNum, all_ways);

    // Send message to know this player doesn't have any way to play now.
    if (conditionPlay == IMPOSSIBLE)
        return IMPOSSIBLE;

    // Check input postion to be valid and then set it.
    r--;
    c--;
    if (*(*(all_ways + r) + c) == 1)
        return PASS;

    // If we have any way to play but player didn't input correct postion, say try again.
    else
        return AGAIN;
}

void ReverseNuts(int table[8][8], int r, int c, int playerNum, Player Players[3])
{
    r--;
    c--;

    for (int dr = -1; dr <= 1; dr++)
    {
        for (int dc = -1; dc <= 1; dc++)
        {
            int flag = 0; // At least should be behind an opposite nut.

            // To skip curent cell
            if (dc == 0 && dr == 0)
                continue;

            int rtmp = r + dr;
            int ctmp = c + dc;

            while (rtmp >= 0 && rtmp < 8 && ctmp >= 0 && ctmp < 8 && table[rtmp][ctmp] == 3 - playerNum)
            {
                flag = 1;
                rtmp += dr;
                ctmp += dc;
            }

            if (rtmp >= 0 && rtmp < 8 && ctmp >= 0 && ctmp < 8 && table[rtmp][ctmp] == playerNum && flag == 1)
            {
                rtmp = r + dr;
                ctmp = c + dc;

                while (rtmp >= 0 && rtmp < 8 && ctmp >= 0 && ctmp < 8 && table[rtmp][ctmp] == 3 - playerNum)
                {
                    table[rtmp][ctmp] = playerNum;
                    rtmp += dr;
                    ctmp += dc;
                    Players[playerNum].score++;
                }
            }
        }
    }
}

int NutsCounter(int table[8][8], int playerNum)
{

    int nutsNumber = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (table[i][j] == playerNum)
                nutsNumber++;
        }
    }

    return nutsNumber;
}

void WinnerFind(int table[8][8], Player Players[3], int is_TimingMode)
{
    int playerNuts[3] = {0};

    playerNuts[1] = NutsCounter(table, 1);
    playerNuts[2] = NutsCounter(table, 2);

    if (is_TimingMode == 1 && (Players[1].time <= 0 || Players[2].time <= 0))
    {
        if (Players[1].time <= 0)
            printf("%s Win, number of nuts: %d", Players[2].name, playerNuts[2]);
        else if (Players[2].time <= 0)
            printf("%s Win, number of nuts: %d", Players[1].name, playerNuts[1]);
    }
    else
    {
        if (playerNuts[1] > playerNuts[2])
            printf("%s Win, number of nuts: %d", Players[1].name, playerNuts[1]);
        else if (playerNuts[2] > playerNuts[1])
            printf("%s Win, number of nuts: %d", Players[2].name, playerNuts[2]);
        else
            printf("Equal");
    }
}

void TableCopy(int target[8][8], int source[8][8])
{

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            target[i][j] = source[i][j];
        }
    }
}

void UndoPlay(int table[8][8], Player Players[3], int playerNum)
{
    TableCopy(table, Players[playerNum].table);
    if(Players[playerNum].undoUseCounter > 0){
        Players[playerNum].undoUseCounter --;
        Players[playerNum].time -= FineTime;
    }
    else{
        Players[playerNum].time -= 2 * FineTime;
        int addTime = Players[1].playTimeLeft + Players[2].playTimeLeft;

        (playerNum == 1)? (Players[2].time += addTime) : (Players[1].time += addTime);

    }
}

void TryAgain(Player Players[3], int *is_endGame, int *is_playAgain, int playerNum)
{
    printf("try again %s\n", Players[playerNum].name);
    *is_playAgain = 1;
    *is_endGame = 0;
}

int PlayGame(int table[8][8], Player Players[3], int playerNum, int *is_endGame, int TimingMode)
{
    int r, c;
    enum conditions cd = PASS;

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

    if (TimingMode == 1)
    {
        // Start Timer
        if (is_playAgain == 0)
            time(&LastTime);
    }

    char input[3];
    scanf("%2s", input);

    // Undo Play
    if (input[0] == 'z' && TimingMode == 1)
    {
        switch (input[1]) {
            case '#':
                UndoPlay(table, Players, 1);
                playerNum = 1;
                break;
            
            case 'o':
                UndoPlay(table, Players, 2);
                playerNum = 2;
                break;

            default:
                TryAgain(Players, is_endGame, &is_playAgain, playerNum);
                cd = AGAIN;
        }
        
        if(cd == PASS)
        {
            Players[1].score = Players[1].lastScore;
            Players[2].score = Players[2].lastScore;

            Players[1].nutsNumber = NutsCounter(table, 1);
            Players[2].nutsNumber = NutsCounter(table, 2);
            Show(table, Players, TimingMode);
        }
    }

    else if(input[0] >= 97 && input[0] <= 104 && input[1] >= 49 && input[1] <= 56)
    {
        // Set row and col
        int row = (int) input[0] - 96;
        char col[2] = {input[1], '\0'};
        r = row;
        c = atoi(col);

        condition = IsCorrectMove(table, r, c, playerNum);

        if (condition == PASS)
        {
            // Process Nuts state
            TableCopy(Players[playerNum].table, table);
            table[r - 1][c - 1] = playerNum;

            Players[1].lastScore = Players[1].score;
            Players[2].lastScore = Players[2].score;

            ReverseNuts(table, r, c, playerNum, Players);

            Players[1].nutsNumber = NutsCounter(table, 1);
            Players[2].nutsNumber = NutsCounter(table, 2);
            
            // Calculate spent time
            if (TimingMode == 1)
            {
                
                time(&CurrentTime);
                Players[playerNum].time += LastTime - CurrentTime;
                Players[playerNum].playTimeLeft = CurrentTime - LastTime;
                is_playAgain = 0;
            }

            // Show final table
            Show(table, Players, TimingMode);
            playerNum++;
            *is_endGame = 0;
        }

        else if (condition == AGAIN)
        {
            TryAgain(Players, is_endGame, &is_playAgain, playerNum);
        }
    }
    else{
        TryAgain(Players, is_endGame, &is_playAgain, playerNum);
    }

    return playerNum;
}
