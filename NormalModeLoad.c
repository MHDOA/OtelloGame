#include "OtelloFuncs.c"

int main() {
  int is_endGame = 0;

  Player Players[3];
  Players[1].nut[0] = '#';
  Players[2].nut[0] = 'O';

  GetPlayerName(Players);

  int table[8][8] = {0};

  int playerNum = 1;
  enum conditions cond = LoadGame(table, Players, &playerNum, 0);

  if(cond == PASS){
    Players[1].nutsNumber = NutsCounter(table, 1);
    Players[2].nutsNumber = NutsCounter(table, 2);

    Show(table, Players, 0);

    while (PASS) {
        playerNum = PlayGame(table, Players, playerNum, &is_endGame, 0);
        if(playerNum > 2)
            playerNum = 1;

        if(is_endGame >= 2)
            break;
    }

    // Show winner
    if(is_endGame != 6){
      WinnerFind(table, Players, 0);
    }
  }
  else{
    printf("\n!Empty!\n");
    GoToMain();
  }

  return 0;
}