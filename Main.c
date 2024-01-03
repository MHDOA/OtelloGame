#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

void newGame(int table[8][8]);
void PlayGame(int table[8][8], char Player[3][20], int player);
int Limitter(int table[8][8], int x, int y, int player);
void Show(int table[8][8]);

int main(){
    
    int table[8][8] = {0};
    char Player[3][20];

    scanf("%20s %20s", Player[1], Player[2]);
    //printf("%s\n %s", Player[1], Player[2]);

    newGame(table);
    PlayGame(table, Player, 1);

    return 0;
}

void newGame(int table[8][8]){
    table[3][3] = 2;
    table[4][3] = 1;

    table[3][4] = 1;
    table[4][4] = 2;

    Show(table);
}

void Show(int table[8][8]){
    system("cls");
    
    char *top = "    1   2   3   4   5   6   7   8  ";
    printf("%s\n", top);

    for(int i = 0; i < 8; i++){
            printf("   --- --- --- --- --- --- --- ---\n");
            printf("  |   |   |   |   |   |   |   |   |\n");

            printf("%d ", i+1);
            for(int k = 0; k < 8; k++){
                printf("| ");
                if(table[i][k] == 1) printf("#");
                else if(table[i][k] == 2) printf("o");
                else printf(" ");
                printf(" ");
            }

            printf("|\n  |   |   |   |   |   |   |   |   |\n");
    }
    printf("   --- --- --- --- --- --- --- ---\n");
}

void PlayGame(int table[8][8], char Player[3][20], int player){

    int x, y;

    printf("Enter your location %s: ", Player[player]);
    scanf("%d %d", &x, &y);

    if(Limitter(table, x, y, player) == 1){
        if(x > 8 || y > 8){
            printf("try again %s\n", Player[player]);
            PlayGame(table, Player, player);
            return;
        }
        table[x - 1][y - 1] = player;
        Show(table);
    }
    
    if(player >= 2) player = 0;
    PlayGame(table, Player, player + 1);
}

int Limitter(int table[8][8], int x, int y, int player){
    return 1;
}