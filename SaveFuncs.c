#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "Crypt.c"

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

void ConsoleClean() {
#ifdef _WIN32
  system("cls");
#elif __linux__
  system("clear");
#endif
}

typedef struct Player Player;

enum conditions { IMPOSSIBLE = -1, AGAIN, PASS };

void SaveJson(char *str_json){
    FILE *fp = fopen("data.json", "w");

    char cipher[strlen(str_json)];
    EncryptMessage(str_json, cipher);
    fputs(cipher, fp);
    fclose(fp);
}

void JsonInitial(){
    cJSON *json = cJSON_CreateObject();
    cJSON *tables = cJSON_CreateObject();
    cJSON *scores = cJSON_CreateObject();
    cJSON *item = cJSON_CreateObject();

    cJSON_AddItemToObject(tables, "Time", item);
    item = cJSON_CreateObject();
    cJSON_AddItemToObject(tables, "Normal", item);

    cJSON_AddItemToObject(json, "Table", tables);
    cJSON_AddItemToObject(json, "Score", scores);

    char *str = cJSON_Print(json);
    SaveJson(str);
    cJSON_Delete(json);

    //printf("%s\n", str);
}

cJSON *ReadJson(){
    
    FILE *fp = fopen("data.json", "r");
    if(fp == NULL){
        JsonInitial();
        fp = fopen("data.json", "r");
    }
    
    char buffer[40960];
    size_t len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    char decrypted[40960];
    DecryptCipher(buffer, decrypted);

    cJSON *json = cJSON_Parse(decrypted);

    //printf("%s", decrypted); // Run this code to See Decrypt json file in console

    return json;
}

void SetScore(struct Player Players[3], int playerNum){
    cJSON *json = ReadJson();
     if(json == NULL){
        JsonInitial();
        json = ReadJson();
    }

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

    char *str = cJSON_Print(json);
    SaveJson(str);
    cJSON_Delete(json);
}

void GetScore(struct Player Players[3], int playerNum){
    cJSON *json = ReadJson();
    if(json == NULL){
        JsonInitial();
        json = ReadJson();
    }

    cJSON *ScoreJson = cJSON_GetObjectItem(json, "Score");
    if(ScoreJson == NULL){
        JsonInitial();
        json = ReadJson();
        ScoreJson = cJSON_GetObjectItem(json, "Score");
    }

    cJSON *playerScore = cJSON_GetObjectItem(ScoreJson, Players[playerNum].name);
    int score;
    if(playerScore != NULL)
        score = playerScore -> valueint;

    cJSON_Delete(json);
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

void SaveNameSet(char result[],int size){
    char RevNumber[4];

    int index = 0;

    while(size >= 10){
        int n = size % 10;
        RevNumber[index] = n + 48;
        size /= 10;
        index++;
    }
    RevNumber[index] = size + 48;

    index++;

    for(int i = 0; i < index; i++){
        result[index - i - 1] = RevNumber[i];
    }

    result[index] = '\0';
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

void GoToMain(){
    printf("\nGo to main menu\n");
    printf("<Enter any char>");
    
    char input[2];
    scanf("%1s", input);

    ConsoleClean();
    system("gcc Intro.c -o intro.exe");
    system("./intro.exe");
}

void SaveGame(int table[8][8], Player Players[3], int playerTurn, int TimingMode){
    cJSON *json = ReadJson();
    if(json == NULL){
        JsonInitial();
        json = ReadJson();
    }

    cJSON *Table = cJSON_GetObjectItem(json, "Table");
    if(Table == NULL){
        JsonInitial();
        json = ReadJson();
        Table = cJSON_GetObjectItem(json, "Table");
    }

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

        cJSON_AddNumberToObject(NewGame, "Turn", playerTurn);

        cJSON *TimeTable = cJSON_GetObjectItem(Table, "Time");
        if(TimeTable == NULL){
            JsonInitial();
            json = ReadJson();
            Table = cJSON_GetObjectItem(json, "Table");
            TimeTable = cJSON_GetObjectItem(Table, "Time");
        }

        cJSON *Item = cJSON_GetObjectItem(TimeTable, SaveName);
        if(Item != NULL){
            int size = cJSON_GetArraySize(Item);
            size++;

            char name[5];
            SaveNameSet(name, size);
            cJSON_AddItemToObject(Item, name, NewGame);
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

        cJSON_AddNumberToObject(NewGame, "Turn", playerTurn);

        cJSON *NormalTable = cJSON_GetObjectItem(Table, "Normal");
        if(NormalTable == NULL){
            JsonInitial();
            json = ReadJson();
            Table = cJSON_GetObjectItem(json, "Table");
            NormalTable = cJSON_GetObjectItem(Table, "Normal");
        }

        cJSON *Item = cJSON_GetObjectItem(NormalTable, SaveName);
        if(Item != NULL){
            int size = cJSON_GetArraySize(Item);
            size++;

            char name[5];
            SaveNameSet(name, size);
            cJSON_AddItemToObject(Item, name, NewGame);
        }
        else{
            cJSON *First = cJSON_CreateObject();
            cJSON_AddItemToObject(First, "1", NewGame);
            cJSON_AddItemToObject(NormalTable, SaveName, First);
        }
    }

    char* str = cJSON_Print(json);
    SaveJson(str);
    cJSON_Delete(json);
    //printf("%s", str);
}

enum conditions LoadGame(int table[8][8], Player Players[], int *playerTurn, int TimingMode){

    cJSON *json = ReadJson();
     if(json == NULL){
        JsonInitial();
        json = ReadJson();
    }
    cJSON *Table = cJSON_GetObjectItem(json, "Table");
    if(Table == NULL){
        JsonInitial();
        json = ReadJson();
        Table = cJSON_GetObjectItem(json, "Table");
    }

    char SaveName[42] = "";
    strcat(SaveName, Players[1].name);
    strcat(SaveName, "-");
    strcat(SaveName, Players[2].name);

    if(TimingMode == 1){
        cJSON *Time = cJSON_GetObjectItem(Table, "Time");
        if(Time == NULL){
            JsonInitial();
            json = ReadJson();
            Table = cJSON_GetObjectItem(json, "Table");
            Time = cJSON_GetObjectItem(Table, "Time");
        }

        cJSON *Game = cJSON_GetObjectItem(Time, SaveName);
        if(Game == NULL){
            return IMPOSSIBLE;
        }
        else{
            int size = cJSON_GetArraySize(Game);
            printf("Choose number to load game(%d game): ", size);
            char c[3];
            scanf("%2s", c);

            cJSON *Item = cJSON_GetObjectItem(Game, c);

            GetTable(Item, table, "CurrentTable");

            GetTable(Item, Players[1].table, "P1Table");
            GetTable(Item, Players[2].table, "P2Table");

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

            Value = cJSON_GetObjectItem(Item, "Turn");
            *playerTurn = Value->valueint;

            return PASS;
        }
    }
    else{
        cJSON *Normal = cJSON_GetObjectItem(Table, "Normal");
        if(Normal == NULL){
            JsonInitial();
            json = ReadJson();
            Table = cJSON_GetObjectItem(json, "Table");
            Normal = cJSON_GetObjectItem(Table, "Normal");
        }

        cJSON *Game = cJSON_GetObjectItem(Normal, SaveName);

        if(Game == NULL){
            return IMPOSSIBLE;
        }

        else{
            int size = cJSON_GetArraySize(Game);
            printf("Choose number to load game(%d game): ", size);
            char c[3];
            scanf("%2s", c);

            cJSON *Item = cJSON_GetObjectItem(Game, c);

            GetTable(Item, table, "CurrentTable");

            cJSON *Value = NULL;
            Value = cJSON_GetObjectItem(Item, "P1Score");
            Players[1].score = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2Score");
            Players[2].score = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "Turn");
            *playerTurn = Value->valueint;

            return PASS;
        }
    }
}