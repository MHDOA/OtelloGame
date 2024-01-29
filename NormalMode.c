#include "OtelloFuncs.c"

int main()
{
    int is_endGame = 0;

    // Initial Players
    Player Players[3];
    Players[1].nut[0] = '#';
    Players[2].nut[0] = 'O';
    Players[1].nutsNumber = 2;
    Players[2].nutsNumber = 2;
    Players[1].score = 0;
    Players[2].score = 0;

    GetPlayerName(Players);

    // Initial New Table
    int table[8][8] = {0};
    NewGame(table);
    Show(table, Players, 0);

    int playerNum = 1; // Player Turn Show
    while (PASS)
    {
        playerNum = PlayGame(table, Players, playerNum, &is_endGame, 0, 0);

        // Player Turn just be 1 or 2.
        if (playerNum > 2)
            playerNum = 1;
        
        // Mean Two Time Impossible or SaveGame
        if (is_endGame >= 2)
            break;
    }

    // Show winner
    if(is_endGame != 6){ // If equal to 6 mean eject by saving.
      WinnerFind(table, Players, 0);
    }

    return 0;
}