#include <stdio.h>
#include <stdlib.h>

enum conditions
{
    IMPOSSIBLE = -1,
    AGAIN,
    PASS
};

void newGame(int table[8][8]);
int PlayGame(int table[8][8], char Player[3][20], int player);
int MovementChecker(int table[8][8], int r, int c, int player);
int AllWays(int table[8][8], int player, int **arr);
int IsCorrectMove(int table[8][8], int r, int c, int player);
void Show(int table[8][8]);
void ReverseNuts(int table[8][8], int r, int c, int player);

int main()
{

    int table[8][8] = {0};
    char Player[3][20];

    scanf("%20s %20s", Player[1], Player[2]);

    newGame(table);

    int player = 1;
    while (PASS)
    {   
        player = PlayGame(table, Player, player);
        if(player > 2) player = 1;
    }

    return 0;
}

void newGame(int table[8][8])
{
    table[3][3] = 2;
    table[4][3] = 1;

    table[3][4] = 1;
    table[4][4] = 2;

    Show(table);
}

void Show(int table[8][8])
{
    // To char* to create table structure
    char *top_buttom_Square = "-------";
    char *sideSquare = "|";

    // Clear console.
    system("clear"); // For Linux running.
    // system("cls") // For windows running.

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

int PlayGame(int table[8][8], char Player[3][20], int player)
{
    int r, c;

    printf("Enter your location %s: ", Player[player]);
    scanf("%d %d", &r, &c);

    enum conditions condition = IsCorrectMove(table, r, c, player);

    if (condition == PASS)
    {
        table[r - 1][c - 1] = player;
        ReverseNuts(table, r, c, player);
        Show(table);
        player++;
        return player;
    }

    else if(condition == AGAIN)
    {
        printf("try again %s\n", Player[player]);
        return player;
    }

    else{
        
    }
}

int MovementChecker(int table[8][8], int r, int c, int player)
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
            int flag = 0; // At least should be bihind an opposite nut.

            // To skip currnt cell
            if (dc == 0 && dr == 0)
                continue;

            int rtmp = r + dr;
            int ctmp = c + dc;

            
            while (rtmp >= 0 && rtmp < 8 && ctmp >= 0 && ctmp < 8 && table[rtmp][ctmp] == 3 - player)
            {
                flag = 1;
                rtmp += dr;
                ctmp += dc;
            }


            if (rtmp >= 0 && rtmp < 8 && ctmp >= 0 && ctmp < 8 && table[rtmp][ctmp] == player && flag == 1)
                return 1;
        }
    }

    return 0;
}

int IsCorrectMove(int table[8][8], int r, int c, int player)
{
    int **all_ways = (int **)malloc(8 * sizeof(int *));
    for (int i = 0; i < 8; i++)
        *(all_ways + i) = (int *)malloc(8 * sizeof(int));

    enum conditions conditionPlay = AllWays(table, player, all_ways);
    if (conditionPlay == IMPOSSIBLE)
        return IMPOSSIBLE;

    r--;
    c--;

    if (*(*(all_ways + r) + c) == 1)
        return PASS;

    else
        return AGAIN;
}

int AllWays(int table[8][8], int player, int **arr)
{
    int Impossible_Play = 1;
    for (int r = 1; r <= 8; r++)
    {
        for (int c = 1; c <= 8; c++)
        {
            if (MovementChecker(table, r, c, player) == 1)
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

void ReverseNuts(int table[8][8], int r, int c, int player)
{
    r--;
    c--;

    for (int dr = -1; dr <= 1; dr++)
    {
        for (int dc = -1; dc <= 1; dc++)
        {
            int flag = 0; // At least should be bihind an opposite nut.

            // To skip currnt cell
            if (dc == 0 && dr == 0)
                continue;

            int rtmp = r + dr;
            int ctmp = c + dc;

            
            while (rtmp >= 0 && rtmp < 8 && ctmp >= 0 && ctmp < 8 && table[rtmp][ctmp] == 3 - player)
            {
                flag = 1;
                rtmp += dr;
                ctmp += dc;
            }


            if (rtmp >= 0 && rtmp < 8 && ctmp >= 0 && ctmp < 8 && table[rtmp][ctmp] == player && flag == 1)
            {
                rtmp = r + dr;
                ctmp = c + dc;

            
                while (rtmp >= 0 && rtmp < 8 && ctmp >= 0 && ctmp < 8 && table[rtmp][ctmp] == 3 - player)
                {
                    table[rtmp][ctmp] = player;
                    rtmp += dr;
                    ctmp += dc;
                }
            }
        }
    }
}