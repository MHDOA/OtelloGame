#include <stdio.h>
#include <string.h>
#include "cJSON.h"

struct Player {
  char name[21];

  char nut[1];
  int nutsNumber;

  int score;
  int lastScore;

  int table[8][8];

  int time;
  int playTimeLeft;

  int undoUseCounter;
  int isUndoMode;
};

typedef struct Player Player;

void JsonInitial();
cJSON *ReadJson();
void SetScore(Player Players[3], int playerNum);
void GetScore(Player Players[3], int playerNum);

void Ranking(Player Players[], int size);
void ShowRank();


void JsonInitial(){
    cJSON *json = cJSON_CreateObject();
    cJSON *tables = cJSON_CreateObject();
    cJSON *scores = cJSON_CreateObject();

    cJSON_AddItemToObject(json, "Table", tables);
    cJSON_AddItemToObject(json, "Score", scores);

    FILE *fp = fopen("data.json", "w");
    char *str = cJSON_Print(json);
    fputs(str, fp);
    cJSON_Delete(json);
    fclose(fp);

    //printf("%s\n", str);
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

void Ranking(Player Players[], int size){
    for(int i = 0; i < size - 1; i++){
        for(int j = 0; j < size - i - 1; j++){
            if(Players[j].score > Players[j + 1].score){
                int scoreHolder = Players[j].score;
                Players[j].score = Players[j + 1]. score;
                Players[j + 1].score = scoreHolder;

                char nameHolder[21] = "";
                strcpy(nameHolder, Players[j].name);
                strcpy(Players[j].name, Players[j + 1].name);
                strcpy(Players[j + 1].name, nameHolder);
                
            }
        }
    }
}

void ShowRank(){
    cJSON *json = ReadJson();
    cJSON *ScoreJson = cJSON_GetObjectItem(json, "Score");

    if(ScoreJson == NULL){
        JsonInitial();
        json = ReadJson();
        ScoreJson = cJSON_GetObjectItem(json, "Score");
    }

    int size = cJSON_GetArraySize(ScoreJson);
    Player Players[size];
    
    cJSON *item = NULL;
    int i = 0;
    for (item = ScoreJson->child; item != NULL; item = item->next){
        //printf("%s: %d\n", item->string, item->valueint);

        strcpy(Players[i].name, item->string);
        Players[i].score = item->valueint;
        i++;
    }

    Ranking(Players, size);

    printf("%14s%26s\n", "**Names**", "**Scores**");
    for(int i = size - 1; i >= 0; i--){
        printf("%-20s%-25d\n", Players[i].name, Players[i].score);
    }

}