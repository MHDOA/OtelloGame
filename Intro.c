#include <stdio.h>
#include <stdlib.h>
#include "SaveFuncs.c"

int main(){

    // Game Start
    printf("1. New game\n2. Load game\n3. Show Rank\n");
    printf("Enter number: ");
    
    int n;
    scanf("%d", &n);

    // New Game
    if(n == 1){
        printf("1. Regular game\n2. Time game\n");
        printf("Enter number: ");
        
        scanf("%d", &n);

        // New Normal
        if(n == 1){
            system("gcc cJSON.c NormalMode.c -o normalMode.exe -lm");
            system("./normalMode.exe");
        }

        // New Time
        else if(n == 2){
            system("gcc cJSON.c TimingMode.c -o timingMode.exe -lm");
            system("./timingMode.exe");
        }

        //Invalid Input
        else{
            printf("Invalid Input. exit!\n");
        }
        
    }

    // Load Game
    else if(n == 2){
        printf("1. Regular game\n2. Time game\n");
        printf("Enter number: ");
        
        scanf("%d", &n);

        // Load Normal
        if(n == 1){
            system("gcc cJSON.c NormalModeLoad.c -o normalModeLoad.exe -lm");
            system("./normalModeLoad.exe");
        }

        //Load Time
        else if(n == 2){
            system("gcc cJSON.c TimingModeLoad.c -o timingModeLoad.exe -lm");
            system("./timingModeLoad.exe");
        }

        // Invalid Input
        else{
            printf("Invalid Input. exit!\n");
        }
    }

    // Show Rank
    else if(n == 3){
        ShowRank();
    }

    // Invalid Input
    else{
        printf("Invalid Input. exit!\n");
    }
    return  0;
}