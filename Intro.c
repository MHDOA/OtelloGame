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
        else{
            system("gcc cJSON.c TimingMode.c -o timingMode.exe -lm");
            system("./timingMode.exe");
        }
        
    }
    else if(n == 2){
        system("gcc test.c -o test.exe");
        system("./test.exe");
    }
    else{
        printf("Invalid Input. exit!\n");
    }
    return  0;
}