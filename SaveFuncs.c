#include <stdio.h>
#include "cJSON.h"

struct Player{
    char *name;
    int score;
};

void JsonInitial();
cJSON *ReadJson();
void SetScore(struct Player Players[3], int playerNum);
void GetScore(struct Player Players[3], int playerNum);

int main(){

    struct Player Players[] = {
        {"ali", 21},
        {"ahmad", 23},
        {"karim", 24}
    };
    
    //JsonInitial();
    GetScore(Players, 1);

    return 0;
}

void JsonInitial(){
    cJSON *json = cJSON_CreateObject();
    cJSON *tables = cJSON_CreateObject();
    cJSON *scores = cJSON_CreateObject();

    cJSON_AddStringToObject(tables, "initial", "initial");
    cJSON_AddNumberToObject(scores, "name", 0);
    

    cJSON_AddItemToObject(json, "Table", tables);
    cJSON_AddItemToObject(json, "Score", scores);

    FILE *fp = fopen("data.json", "w");
    char *str = cJSON_Print(json);
    fputs(str, fp);
    cJSON_Delete(json);
    fclose(fp);

    printf("%s\n", str);
}

cJSON *ReadJson(){

    FILE *fp = fopen("data.json", "r");
    if(fp == NULL){
        JsonInitial();
        fp = fopen("data.json", "r");
    }

    char buffer[1024];
    size_t len = fread(buffer, 1, 1024, fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);

    return json;
}

void SetScore(struct Player Players[3], int playerNum){
    cJSON *json = ReadJson();

    cJSON *ScoreJson = cJSON_GetObjectItem(json, "Score");
    if(ScoreJson == NULL){
        JsonInitial();
        json = ReadJson();
        ScoreJson = cJSON_GetObjectItem(json, "Score");
    }

    cJSON *playerScore = cJSON_GetObjectItem(ScoreJson, Players[playerNum].name);

    if(playerScore != NULL){
        int score = playerScore->valueint + Players[playerNum].score;
        cJSON_SetNumberValue(playerScore, score);
    }
    else{
        cJSON_AddNumberToObject(ScoreJson, Players[playerNum].name, Players[playerNum].score);
    }

    FILE *fp = fopen("data.json", "w");
    char *str = cJSON_Print(json);
    fputs(str, fp);
    fclose(fp);
    cJSON_Delete(json);

    //printf("%s", str);

}

void GetScore(struct Player Players[3], int playerNum){
    cJSON *json = ReadJson();
    cJSON *ScoreJson = cJSON_GetObjectItem(json, "Score");

    if(ScoreJson == NULL){
        JsonInitial();
        json = ReadJson();
        ScoreJson = cJSON_GetObjectItem(json, "Score");
    }

    //int size = cJSON_GetArraySize(ScoreJson);

    cJSON *playerScore = cJSON_GetObjectItem(ScoreJson, Players[playerNum].name);
    int score;
    if(playerScore != NULL)
        score = playerScore -> valueint;

    cJSON_Delete(json);

    //printf("%d", size);
}