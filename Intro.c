#include <stdio.h>

int main(){
    
    printf("1- New game\n2- Load game\n");
    
    int n;
    scanf("%d", &n);

    if(n == 1){
        printf("1- Regular game\n2- Time game\n");
        scanf("%d", &n);

        if(n == 1){
            system("gcc Main.c -o main.exe");
            system("./main.exe");
        }
        else{
            system("gcc Main.c -o main.exe");
            system("./main.exe");
        }
        
    }
    else if(n == 2){
        system("gcc test.c -o test.exe");
        system("./test.exe");
    }
    return  0;
}