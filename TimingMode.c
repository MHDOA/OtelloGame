#include "OtelloFuncs.c"
#include <stdio.h>

int main() {
  int is_endGame = 0;

  // Define Players list
  Player Players[3];
  Players[1].nut[0] = '#';
  Players[2].nut[0] = 'O';
  Players[1].nutsNumber = 2;
  Players[2].nutsNumber = 2;
  Players[1].score = 0;
  Players[2].score = 0;
  Players[1].undoUseCounter = 1;
  Players[2].undoUseCounter = 1;
  Players[1].isUndoMode = 0;
  Players[2].isUndoMode = 0;

  scanf("%20s %20s", Players[1].name, Players[2].name);

  // Get Time
  printf("Enter your time(90s to 60,000s): ");
  int timeTmp;
  scanf("%d", &timeTmp);
  if (timeTmp > 60000)
    timeTmp = 60000;
  if (timeTmp < 90)
    timeTmp = 90;

  Players[1].time = timeTmp;
  Players[2].time = timeTmp;

  // Initial new game table
  int table[8][8] = {0};
  NewGame(table);

  // Initial undo play
  TableCopy(Players[1].table, table);
  TableCopy(Players[2].table, table);

  Show(table, Players, 1);

  // Start game
  int playerNum = 1;
  while (PASS) {
    playerNum = PlayGame(table, Players, playerNum, &is_endGame, 1);

    // Player number just 1 or 2.
    if (playerNum > 2)
      playerNum = 1;

    // When game over
    if (is_endGame >= 2 || Players[1].time <= 0 || Players[2].time <= 0)
      break;
  }

  // Show winner
  WinnerFind(table, Players, 1);

  return 0;
}
