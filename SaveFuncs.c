#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "Crypt.c"


// Define Player structure
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


// Define console clean function
void ConsoleClean() {
#ifdef _WIN32 // For windows
  system("cls");
#elif __linux__ // For linux
  system("clear");
#endif
}

typedef struct Player Player;

enum conditions { IMPOSSIBLE = -1, AGAIN, PASS };  // Conditions enum

void SaveJson(char *str_json){
    FILE *fp = fopen("data.json", "w"); //Open file from write mode

    char cipher[strlen(str_json)];
    EncryptMessage(str_json, cipher); //Encrypt json

    fputs(cipher, fp); // Save encrypted json file
    fclose(fp);
}

void JsonInitial(){
    cJSON *json = cJSON_CreateObject(); // Main property
    cJSON *tables = cJSON_CreateObject(); // To Save Games
    cJSON *scores = cJSON_CreateObject(); // To Save Scores
    cJSON *item = cJSON_CreateObject(); // To Add Time and Normal in tables

    cJSON_AddItemToObject(tables, "Time", item); // add Time
    item = cJSON_CreateObject();
    cJSON_AddItemToObject(tables, "Normal", item); // add Normal

    cJSON_AddItemToObject(json, "Table", tables);
    cJSON_AddItemToObject(json, "Score", scores);

    char *str = cJSON_Print(json); // Get json structure in string
    SaveJson(str); // Save Encrypted json file
    cJSON_Delete(json);

    //printf("%s\n", str);
}

cJSON *ReadJson(){
    
    FILE *fp = fopen("data.json", "r"); // Read Encrypted json file
    if(fp == NULL){ //
        JsonInitial();
        fp = fopen("data.json", "r");
    }
    
    // Read from encrypted json file
    char buffer[40960];
    size_t len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    // Decrypt json strings
    char decrypted[40960];
    DecryptCipher(buffer, decrypted);

    cJSON *json = cJSON_Parse(decrypted); // Parse json file

    //printf("%s", decrypted); // Run this code to See Decrypt json file in console

    return json;
}

void SetScore(struct Player Players[3], int playerNum){

    // Read json file
    cJSON *json = ReadJson();
     if(json == NULL){
        JsonInitial();
        json = ReadJson();
    }

    // Get Score property
    cJSON *ScoreJson = cJSON_GetObjectItem(json, "Score");
    if(ScoreJson == NULL){
        JsonInitial();
        json = ReadJson();
        ScoreJson = cJSON_GetObjectItem(json, "Score");
    }

    // Get a specific player score
    cJSON *playerScore = cJSON_GetObjectItem(ScoreJson, Players[playerNum].name);

    // Update if exist
    if(playerScore != NULL){
        int score = playerScore->valueint + Players[playerNum].score;
        cJSON_SetNumberValue(playerScore, score);
    }

    // Add player
    else{
        cJSON_AddNumberToObject(ScoreJson, Players[playerNum].name, Players[playerNum].score);
    }

    // Save json file
    char *str = cJSON_Print(json);
    SaveJson(str);
    cJSON_Delete(json);
}

// Didn't use!
int GetScore(struct Player Players[3], int playerNum){

    // Read json file
    cJSON *json = ReadJson();
    if(json == NULL){
        JsonInitial();
        json = ReadJson();
    }

    // Get Score property
    cJSON *ScoreJson = cJSON_GetObjectItem(json, "Score");
    if(ScoreJson == NULL){
        JsonInitial();
        json = ReadJson();
        ScoreJson = cJSON_GetObjectItem(json, "Score");
    }

    // Get specific player score
    cJSON *playerScore = cJSON_GetObjectItem(ScoreJson, Players[playerNum].name);

    // If exist get it
    int score;
    if(playerScore != NULL)
        score = playerScore -> valueint;

    cJSON_Delete(json);

    return score;
}

void Ranking(Player Players[], int size){

    // Sorting players by them scores
    for(int i = 0; i < size - 1; i++){
        for(int j = 0; j < size - i - 1; j++){

            if(Players[j].score > Players[j + 1].score){

                // Swap score
                int scoreHolder = Players[j].score;
                Players[j].score = Players[j + 1]. score;
                Players[j + 1].score = scoreHolder;

                // Swap name
                char nameHolder[21] = "";
                strcpy(nameHolder, Players[j].name);
                strcpy(Players[j].name, Players[j + 1].name);
                strcpy(Players[j + 1].name, nameHolder);
                
            }
        }
    }
}

void ShowRank(){

    // Read json file
    cJSON *json = ReadJson();
    if(json == NULL){
        JsonInitial();
        json = ReadJson();
    }

    // Get Score property
    cJSON *ScoreJson = cJSON_GetObjectItem(json, "Score");
    if(ScoreJson == NULL){
        JsonInitial();
        json = ReadJson();
        ScoreJson = cJSON_GetObjectItem(json, "Score");
    }

    // Get Score size and create array of players
    int size = cJSON_GetArraySize(ScoreJson);
    Player Players[size];
    
    // Cast all scores to struct Players
    cJSON *item = NULL;
    int i = 0;
    for (item = ScoreJson->child; item != NULL; item = item->next){
        //printf("%s: %d\n", item->string, item->valueint);

        strcpy(Players[i].name, item->string);
        Players[i].score = item->valueint;
        i++;
    }

    Ranking(Players, size); // Sort Players

    // Show Players rank
    printf("%14s%26s\n", "**Names**", "**Scores**");
    for(int i = size - 1; i >= 0; i--){
        printf("%-20s%-25d\n", Players[i].name, Players[i].score);
    }

}

// Create appropriate name for save game
void SaveNameSet(char result[],int size){
    char RevNumber[4];

    int index = 0;

    // Get digit of size
    while(size >= 10){
        int n = size % 10;
        RevNumber[index] = n + 48; // Add char of digit
        size /= 10;
        index++;
    }

    RevNumber[index] = size + 48; // Cast the last digit

    index++;

    // Cast RevNumber reverse to result
    for(int i = 0; i < index; i++){
        result[index - i - 1] = RevNumber[i];
    }

    result[index] = '\0'; // Convert to correct formate
}

void SetTable(cJSON *item, int table[8][8], char *name){

    cJSON *array = cJSON_CreateArray (); // 2DArray for save table 

    for (int i = 0; i < 8; i++) {
        cJSON *row = cJSON_CreateArray (); // Create row
        for (int j = 0; j < 8; j++) {
            cJSON_AddItemToArray (row, cJSON_CreateNumber (table[i][j])); // Add index
        }
        cJSON_AddItemToArray (array, row); // Create column
    }

    cJSON_AddItemToObject (item, name, array); // Add table array to json
}

void GetTable(cJSON *item, int table[8][8], char *name){

    cJSON *array = cJSON_GetObjectItem(item, name); // 2DArray to save table

    for (int i = 0; i < 8; i++) {
        cJSON *row = cJSON_GetArrayItem(array, i); // Get Row
        for (int j = 0; j < 8; j++) {
            cJSON *col = cJSON_GetArrayItem(row, j); //Get Column
            table[i][j] = col->valueint; // Get index and save to table
        }
    }
}

// After save or show rank
void GoToMain(){
    printf("\nGo to main menu\n");
    printf("<Enter any char>");
    
    char input[2];
    scanf("%1s", input);

    ConsoleClean();
    system("gcc cJSON.c Intro.c -o intro.exe -lm");
    system("./intro.exe");
}

void SaveGame(int table[8][8], Player Players[3], int playerTurn, int TimingMode){

    // Read json file
    cJSON *json = ReadJson();
    if(json == NULL){
        JsonInitial();
        json = ReadJson();
    }

    // Get Table property
    cJSON *Table = cJSON_GetObjectItem(json, "Table");
    if(Table == NULL){
        JsonInitial();
        json = ReadJson();
        Table = cJSON_GetObjectItem(json, "Table");
    }

    // Make appropriate name for save property
    char SaveName[42] = "";
    strcat(SaveName, Players[1].name);
    strcat(SaveName, "-");
    strcat(SaveName, Players[2].name);

    // For timing mod save
    if(TimingMode == 1){

        // Create new object
        cJSON *NewGame = cJSON_CreateObject();

        SetTable(NewGame, table, "CurrentTable");

        SetTable(NewGame, Players[1].table, "P1Table"); // Save undo table
        SetTable(NewGame, Players[2].table, "P2Table");

        cJSON_AddNumberToObject(NewGame, "P1Score", Players[1].score); // Save score
        cJSON_AddNumberToObject(NewGame, "P2Score", Players[2].score);

        cJSON_AddNumberToObject(NewGame, "P1LastScore", Players[1].lastScore); // Save undo score
        cJSON_AddNumberToObject(NewGame, "P2LastScore", Players[2].lastScore);

        cJSON_AddNumberToObject(NewGame, "P1Time", Players[1].time); // Save time
        cJSON_AddNumberToObject(NewGame, "P2Time", Players[2].time);

        cJSON_AddNumberToObject(NewGame, "P1LeftTime", Players[1].playTimeLeft); // Save play time left
        cJSON_AddNumberToObject(NewGame, "P2LeftTime", Players[2].playTimeLeft);

        cJSON_AddNumberToObject(NewGame, "P1undoUse", Players[1].undoUseCounter); // Save number of undo use
        cJSON_AddNumberToObject(NewGame, "P2undoUse", Players[2].undoUseCounter);

        cJSON_AddNumberToObject(NewGame, "P1isUndoMode", Players[1].isUndoMode); // Save undo mode situation
        cJSON_AddNumberToObject(NewGame, "P2isUndoMode", Players[2].isUndoMode);

        cJSON_AddNumberToObject(NewGame, "Turn", playerTurn); // Save play Turn
 
        // Get Time property
        cJSON *TimeTable = cJSON_GetObjectItem(Table, "Time");
        if(TimeTable == NULL){
            JsonInitial();
            json = ReadJson();
            Table = cJSON_GetObjectItem(json, "Table");
            TimeTable = cJSON_GetObjectItem(Table, "Time");
        }

        // Get item to update
        cJSON *Item = cJSON_GetObjectItem(TimeTable, SaveName);
        if(Item != NULL){
            int size = cJSON_GetArraySize(Item);
            size++;

            // Create appropriate name to save
            char name[5];
            SaveNameSet(name, size);

            cJSON_AddItemToObject(Item, name, NewGame); // Save properties
        }

        // Create if didn't exist
        else{
            cJSON *First = cJSON_CreateObject();
            cJSON_AddItemToObject(First, "1", NewGame);
            cJSON_AddItemToObject(TimeTable, SaveName, First);
        }
    }

    // Normal mod save
    else{
        cJSON *NewGame = cJSON_CreateObject();

        SetTable(NewGame, table, "CurrentTable"); // table save

        cJSON_AddNumberToObject(NewGame, "P1Score", Players[1].score); // Player score save
        cJSON_AddNumberToObject(NewGame, "P2Score", Players[2].score);

        cJSON_AddNumberToObject(NewGame, "Turn", playerTurn); // Save play turn

        // Get Normal property
        cJSON *NormalTable = cJSON_GetObjectItem(Table, "Normal");
        if(NormalTable == NULL){
            JsonInitial();
            json = ReadJson();
            Table = cJSON_GetObjectItem(json, "Table");
            NormalTable = cJSON_GetObjectItem(Table, "Normal");
        }

        // Update item
        cJSON *Item = cJSON_GetObjectItem(NormalTable, SaveName);
        if(Item != NULL){
            int size = cJSON_GetArraySize(Item);
            size++;

            // Create correct name
            char name[5];
            SaveNameSet(name, size);

            cJSON_AddItemToObject(Item, name, NewGame); // Save properties
        }

        // Crate if didn't exist
        else{
            cJSON *First = cJSON_CreateObject();
            cJSON_AddItemToObject(First, "1", NewGame);
            cJSON_AddItemToObject(NormalTable, SaveName, First);
        }
    }

    // Save json file
    char* str = cJSON_Print(json);
    SaveJson(str);
    cJSON_Delete(json);
    //printf("%s", str);
}

enum conditions LoadGame(int table[8][8], Player Players[], int *playerTurn, int TimingMode){ // Return enum to detect number of load game


    // Read json file
    cJSON *json = ReadJson();
     if(json == NULL){
        JsonInitial();
        json = ReadJson();
    }

    // Get Table Property
    cJSON *Table = cJSON_GetObjectItem(json, "Table");
    if(Table == NULL){
        JsonInitial();
        json = ReadJson();
        Table = cJSON_GetObjectItem(json, "Table");
    }

    // Create appropriate load name 
    char SaveName[42] = "";
    strcat(SaveName, Players[1].name);
    strcat(SaveName, "-");
    strcat(SaveName, Players[2].name);

    // Time mod load
    if(TimingMode == 1){

        // Get Time property
        cJSON *Time = cJSON_GetObjectItem(Table, "Time");
        if(Time == NULL){
            JsonInitial();
            json = ReadJson();
            Table = cJSON_GetObjectItem(json, "Table");
            Time = cJSON_GetObjectItem(Table, "Time");
        }

        // Get saved games
        cJSON *Game = cJSON_GetObjectItem(Time, SaveName);

        // Not found any saved game
        if(Game == NULL){
            return IMPOSSIBLE;
        }

        // Load game
        else{

            // Set number of saved games
            int size = cJSON_GetArraySize(Game);
            printf("Choose number to load game(%d game): ", size);
            char c[3];
            scanf("%2s", c);

            // Get an item of them
            cJSON *Item = cJSON_GetObjectItem(Game, c);

            GetTable(Item, table, "CurrentTable"); // Load table

            GetTable(Item, Players[1].table, "P1Table"); // Load undo table
            GetTable(Item, Players[2].table, "P2Table");

            cJSON *Value = NULL; // To get values

            Value = cJSON_GetObjectItem(Item, "P1Score"); // Load scores
            Players[1].score = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2Score");
            Players[2].score = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "P1LastScore"); // Load undo scores
            Players[1].lastScore = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2LastScore");
            Players[2].lastScore = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "P1Time"); // Load time
            Players[1].time = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2Time");
            Players[2].time = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "P1LeftTime"); // Load play time left
            Players[1].playTimeLeft = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2LeftTime");
            Players[2].playTimeLeft = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "P1undoUse"); // Load number of undo use
            Players[1].undoUseCounter = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2undoUse");
            Players[2].undoUseCounter = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "P1isUndoMode"); // Load undo mode situation
            Players[1].isUndoMode = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2isUndoMode");
            Players[2].isUndoMode = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "Turn"); // Load play turn
            *playerTurn = Value->valueint;

            return PASS; // Load successful
        }
    }

    // Normal Load
    else{

        // Get Normal property
        cJSON *Normal = cJSON_GetObjectItem(Table, "Normal");
        if(Normal == NULL){
            JsonInitial();
            json = ReadJson();
            Table = cJSON_GetObjectItem(json, "Table");
            Normal = cJSON_GetObjectItem(Table, "Normal");
        }

        cJSON *Game = cJSON_GetObjectItem(Normal, SaveName); // Select a game to load

        // Not found any saved game
        if(Game == NULL){
            return IMPOSSIBLE;
        }

        // Load game
        else{

            // Set game to load
            int size = cJSON_GetArraySize(Game);
            printf("Choose number to load game(%d game): ", size);
            char c[3];
            scanf("%2s", c);

            cJSON *Item = cJSON_GetObjectItem(Game, c); // Select Item to load

            GetTable(Item, table, "CurrentTable"); // Load table

            cJSON *Value = NULL;
            Value = cJSON_GetObjectItem(Item, "P1Score"); // Load player scores
            Players[1].score = Value->valueint;
            Value = cJSON_GetObjectItem(Item, "P2Score");
            Players[2].score = Value->valueint;

            Value = cJSON_GetObjectItem(Item, "Turn"); // Load play turn
            *playerTurn = Value->valueint;

            return PASS; // Load successful
        }
    }
}