struct Player
{
    char name[21];
    char nut[1];
    int score;

    int table[8][8];
};

typedef struct Player Player;

enum conditions
{
    IMPOSSIBLE = -1,
    AGAIN,
    PASS
};

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
            int flag = 0; // At least should be bihind an opposite nut.

            // To skip currnt cell
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

 int IsCorrectMove(int table[8][8], int r, int c, int playerNum)
{
    // Save all ways for player in all_way and send to AllWays to find them.
    int **all_ways = (int **)malloc(8 * sizeof(int *));
    for (int i = 0; i < 8; i++)
        *(all_ways + i) = (int *)malloc(8 * sizeof(int));

    enum conditions conditionPlay = AllWays(table, playerNum, all_ways);

    // Send message to know this player dosn't have any way to play now.
    if (conditionPlay == IMPOSSIBLE)
        return IMPOSSIBLE;

    // Check input posation to be valid and then set it.
    r--;
    c--;
    if (*(*(all_ways + r) + c) == 1)
        return PASS;

    // If we have any way to play but player didn't input correct posation, say try again.
    else
        return AGAIN;
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

 void ReverseNuts(int table[8][8], int r, int c, int playerNum, Player Players[3])
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

 void WinnerFinde(int table[8][8], Player Players[3])
{
    int playerNuts[3] = {0};

    playerNuts[1] = NutsCounter(table, 1);
    playerNuts[2] = NutsCounter(table, 2);

    if (playerNuts[1] > playerNuts[2])
        printf("%s Win, number of nuts: %d", Players[1].name, playerNuts[1]);
    else if (playerNuts[2] > playerNuts[1])
        printf("%s Win, number of nuts: %d", Players[2].name, playerNuts[2]);
    else
        printf("Equal");
}



 void TableCoppy(int target[8][8], int source[8][8])
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
    TableCoppy(table, Players[playerNum].table);
}

