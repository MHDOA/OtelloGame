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

void SetTable(cJSON *item, int table[8][8], char *name);
void SaveGame(int table[8][8], Player Players[3], int TimingMode);


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

    char buffer[20480];
    size_t len = fread(buffer, 1, sizeof(buffer), fp);
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

void SetTable(cJSON *item, int table[8][8], char *name){
    cJSON *array = cJSON_CreateArray ();

    for (int i = 0; i < 8; i++) {
        cJSON *row = cJSON_CreateArray ();
        for (int j = 0; j < 8; j++) {
            cJSON_AddItemToArray (row, cJSON_CreateNumber (table[i][j]));
        }
        cJSON_AddItemToArray (array, row);
    }

    cJSON_AddItemToObject (item, name, array);
}

void GetTable(cJSON *item, int table[8][8], char *name){
    cJSON *array = cJSON_GetObjectItem(item, name);
    
    for (int i = 0; i < 8; i++) {
        cJSON *row = cJSON_GetArrayItem(array, i);
        for (int j = 0; j < 8; j++) {
            cJSON *col = cJSON_GetArrayItem(row, j);
            table[i][j] = col->valueint;
        }
    }
}



void SaveGame(int table[8][8], Player Players[3], int TimingMode){
    cJSON *json = ReadJson();
    cJSON *Table = cJSON_GetObjectItem(json, "Table");
    
    char SaveName[42] = "";
    strcat(SaveName, Players[1].name);
    strcat(SaveName, "-");
    strcat(SaveName, Players[2].name);

    if(TimingMode == 1){
        cJSON *NewGame = cJSON_CreateObject();

        SetTable(NewGame, table, "CurrentTable");

        SetTable(NewGame, Players[1].table, "P1Table");
        SetTable(NewGame, Players[2].table, "P2Table");

        cJSON_AddNumberToObject(NewGame, "P1Score", Players[1].score);
        cJSON_AddNumberToObject(NewGame, "P2Score", Players[2].score);

        cJSON_AddNumberToObject(NewGame, "P1LastScore", Players[1].lastScore);
        cJSON_AddNumberToObject(NewGame, "P2LastScore", Players[2].lastScore);

        cJSON_AddNumberToObject(NewGame, "P1Time", Players[1].time);
        cJSON_AddNumberToObject(NewGame, "P2Time", Players[2].time);

        cJSON_AddNumberToObject(NewGame, "P1LeftTime", Players[1].playTimeLeft);
        cJSON_AddNumberToObject(NewGame, "P2LeftTime", Players[2].playTimeLeft);

        cJSON_AddNumberToObject(NewGame, "P1undoUse", Players[1].undoUseCounter);
        cJSON_AddNumberToObject(NewGame, "P2undoUse", Players[2].undoUseCounter);

        cJSON_AddNumberToObject(NewGame, "P1isUndoMode", Players[1].isUndoMode);
        cJSON_AddNumberToObject(NewGame, "P2isUndoMode", Players[2].isUndoMode);

        cJSON *TimeTable = cJSON_GetObjectItem(Table, "Time");
        cJSON *Item = cJSON_GetObjectItem(TimeTable, SaveName);
        if(Item != NULL){
            int size = cJSON_GetArraySize(Item);
            size++;

            char c[2] = {48 + size, '\0'};
            cJSON_AddItemToObject(Item, c, NewGame);
        }
        else{
            cJSON *First = cJSON_CreateObject();
            cJSON_AddItemToObject(First, "1", NewGame);
            cJSON_AddItemToObject(TimeTable, SaveName, First);
        }
    }

    else{
        cJSON *NewGame = cJSON_CreateObject();

        SetTable(NewGame, table, "CurrentTable");

        cJSON_AddNumberToObject(NewGame, "P1Score", Players[1].score);
        cJSON_AddNumberToObject(NewGame, "P2Score", Players[2].score);

        cJSON *NormalTable = cJSON_GetObjectItem(Table, "Normal");
        cJSON *Item = cJSON_GetObjectItem(NormalTable, SaveName);
        if(Item != NULL){
            int size = cJSON_GetArraySize(Item);
            size++;

            char c[2] = {48 + size, '\0'};
            cJSON_AddItemToObject(Item, c, NewGame);
        }
        else{
            cJSON *First = cJSON_CreateObject();
            cJSON_AddItemToObject(First, "1", NewGame);
            cJSON_AddItemToObject(NormalTable, SaveName, First);
        }
    }

    FILE *fp = fopen("data.json", "w");
    char* str = cJSON_Print(json);
    fputs(str, fp);
    fclose(fp);
    printf("%s", str);
}

void LoadGame(int table[8][8], Player Players[], int TimingMode){

    cJSON *json = ReadJson();
    cJSON *Table = cJSON_GetObjectItem(json, "Table");

    char SaveName[42] = "";
    strcat(SaveName, Players[1].name);
    strcat(SaveName, "-");
    strcat(SaveName, Players[2].name);

    if(TimingMode == 1){
        cJSON *Time = cJSON_GetObjectItem(Table, "Time");
        cJSON *Game = cJSON_GetObjectItem(Time, SaveName);
        if(Game == NULL){
            printf("Empty");
        }
        else{
            int size = cJSON_GetArraySize(Game);
            printf("Choose Load game from(%d game): ", size);
            char c[3];
            scanf("%2s", c);

            cJSON *Item = cJSON_GetObjectItem(Game, c);

            GetTable(Item, table, "CurrentTable");

            SetTable(Item, Players[1].table, "P1Table");
            SetTable(Item, Players[2].table, "P2Table");

            cJSON *Value = NULL;
            Value = cJSON_GetObjectItem(Item, "P1Score");
            Players[1].score = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2Score");
            Players[2].score = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "P1LastScore");
            Players[1].lastScore = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2LastScore");
            Players[2].lastScore = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "P1Time");
            Players[1].time = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2Time");
            Players[2].time = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "P1LeftTime");
            Players[1].playTimeLeft = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2LeftTime");
            Players[2].playTimeLeft = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "P1undoUse");
            Players[1].undoUseCounter = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2undoUse");
            Players[2].undoUseCounter = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "P1isUndoMode");
            Players[1].isUndoMode = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2isUndoMode");
            Players[2].isUndoMode = Value->valueint;
        }
    }
    else{
        cJSON *Normal = cJSON_GetObjectItem(Table, "Normal");
        cJSON *Game = cJSON_GetObjectItem(Normal, SaveName);

        if(Game == NULL){
            printf("Empty");
        }

        else{
            int size = cJSON_GetArraySize(Game);
            printf("Choose Load game from(%d game): ", size);
            char c[3];
            scanf("%2s", c);

            cJSON *Item = cJSON_GetObjectItem(Game, c);

            cJSON *Value = NULL;
            Value = cJSON_GetObjectItem(Item, "P1Score");
            Players[1].score = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2Score");
            Players[2].score = Value->valueint;
        }
    }
}