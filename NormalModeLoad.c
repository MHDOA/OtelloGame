#include "OtelloFuncs.c"
#include <stdio.h>

int main() {
  int is_endGame = 0;

  Player Players[3];
  Players[1].nut[0] = '#';
  Players[2].nut[0] = 'O';

  scanf("%20s %20s", Players[1].name, Players[2].name);

  int table[8][8] = {0};

  int playerNum = 1;
  LoadGame(table, Players, &playerNum, 0);

  Players[1].nutsNumber = NutsCounter(table, 1);
  Players[2].nutsNumber = NutsCounter(table, 2);

  Show(table, Players, 0);

  while (PASS) {
    playerNum = PlayGame(table, Players, playerNum, &is_endGame, 0);
    if (playerNum > 2)
      playerNum = 1;

    if (is_endGame >= 2)
      break;
  }

  WinnerFind(table, Players, 0);

  return 0;
}