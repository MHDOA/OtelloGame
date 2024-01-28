#include "OtelloFuncs.c"

int main()
{
    int is_endGame = 0;

    Player Players[3];
    Players[1].nut[0] = '#';
    Players[2].nut[0] = 'O';
    Players[1].nutsNumber = 2;
    Players[2].nutsNumber = 2;
    Players[1].score = 0;
    Players[2].score = 0;

    GetPlayerName(Players);

    int table[8][8] = {0};
    NewGame(table);
    Show(table, Players, 0);

    int playerNum = 1;
    while (PASS)
    {
        playerNum = PlayGame(table, Players, playerNum, &is_endGame, 0);
        if (playerNum > 2)
            playerNum = 1;

        if (is_endGame >= 2)
            break;
    }

    // Show winner
    if(is_endGame != 6){
      WinnerFind(table, Players, 0);
    }

    return 0;
}