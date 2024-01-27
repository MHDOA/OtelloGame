#include <stdio.h>
#include "cJSON.h"

struct Player{
    char *name;
    int score;
};

cJSON *ReadJson();
void SetScore(struct Player Players[3], int playerNum);
void GetScore();
void UpdateScore();

int main(){

    struct Player Players[] = {
        {"ali", 21},
        {"ahmad", 23},
        {"karim", 24}
    };
    
    SetScore(Players, 0);

    return 0;
}

cJSON *ReadJson(){

    FILE *fp = fopen("data.json", "w");
    /*if(fp == NULL){
        FILE *fp = fopen("data.json", "w");
    }*/

    char buffer[1024];
    size_t len = fread(buffer, 1, 1024, fp);

    cJSON *json = cJSON_Parse(buffer);

    return json;
}

void SetScore(struct Player Players[3], int playerNum){
    cJSON *json = ReadJson();
    printf("r\n");

    cJSON *ScoresJson = cJSON_GetObjectItem(json, "Score");

    cJSON *playerScore = cJSON_GetObjectItem(ScoresJson, Players[playerNum].name);

    /*if(playerScore != NULL){
        int score = playerScore->valueint + Players[playerNum].score;
        cJSON_ReplaceItemInObject(ScoresJson, Players[playerNum].name, cJSON_CreateNumber(score));
    }
    else {
        cJSON_AddNumberToObject(ScoresJson, Players[playerNum].name, Players[playerNum].score);
    }

    cJSON_ReplaceItemInObject(json, "Score", ScoresJson);

    FILE *fp = fopen("data.json", "w");*/
    char *str = cJSON_Print(json);
    //fputs(str, fp);

    printf("%s", str);

}