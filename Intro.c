#include <stdio.h>
#include <stdlib.h>

int main(){
    
    printf("1. New game\n2. Load game\n");
    printf("Enter number: ");
    
    int n;
    scanf("%d", &n);

    if(n == 1){
        printf("1. Regular game\n2. Time game\n");
        printf("Enter number: ");
        
        scanf("%d", &n);

        if(n == 1){
            system("gcc cJSON.c NormalMode.c -o normalMode.exe -lm");
            system("./normalMode.exe");
        }
        else if(n == 2){
            system("gcc cJSON.c TimingMode.c -o timingMode.exe -lm");
            system("./timingMode.exe");
        }
        else{
            printf("Invalid Input. exit!\n");
        }
        
    }
    else if(n == 2){
        printf("1. Regular game\n2. Time game\n");
        printf("Enter number: ");
        
        scanf("%d", &n);

        if(n == 1){
            system("gcc cJSON.c NormalModeLoad.c -o normalModeLoad.exe -lm");
            system("./normalModeLoad.exe");
        }
        else if(n == 2){
            system("gcc cJSON.c TimingModeLoad.c -o timingModeLoad.exe -lm");
            system("./timingModeLoad.exe");
        }
        else{
            printf("Invalid Input. exit!\n");
        }
    }
    else{
        printf("Invalid Input. exit!\n");
    }
    return  0;
}