#include <stdio.h>
#include "cJSON.h"

int main () {
    int table[8][8] = {1, 1 };
    cJSON *root = cJSON_CreateObject ();
    cJSON *array = cJSON_CreateArray ();

    for (int i = 0; i < 8; i++) {
        cJSON *row = cJSON_CreateArray ();
        for (int j = 0; j < 8; j++) {
            cJSON_AddItemToArray (row, cJSON_CreateNumber (table[i][j]));
        }
        cJSON_AddItemToArray (array, row);
    }

    cJSON_AddItemToObject (root, "table", array);

    char *json_str = cJSON_Print (root);
    //FILE *fp = fopen("data.json", "a");

    printf("%s\n", json_str);
    //fputs(json_str, fp);
    //close(fp);
    return 0;
}
