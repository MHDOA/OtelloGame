#include "OtelloFuncs.c"

int main() {
  int is_endGame = 0;

  // Initial Players
  Player Players[3];
  Players[1].nut[0] = '#';
  Players[2].nut[0] = 'O';

  GetPlayerName(Players);

  // Initial Table
  int table[8][8] = {0};

  // Get Table and Players property from json file
  int playerNum = 1;
  enum conditions cond = LoadGame(table, Players, &playerNum, 0);

  if(cond == PASS){

    // Get player nuts to show
    Players[1].nutsNumber = NutsCounter(table, 1);
    Players[2].nutsNumber = NutsCounter(table, 2);

    Show(table, Players, 0);

    while (PASS) {
        playerNum = PlayGame(table, Players, playerNum, &is_endGame, 0);

        // Player Turn just be 1 or 2.
        if(playerNum > 2)
            playerNum = 1;
        
        // Mean Two Time Impossible or SaveGame
        if(is_endGame >= 2)
            break;
    }

    // Show winner
    if(is_endGame != 6){ // If equal to 6 mean eject by saving.
      WinnerFind(table, Players, 0);
    }
  }

  // Game to load not found!
  else{
    printf("\n!Empty!\n");
    GoToMain();
  }

  return 0;
}