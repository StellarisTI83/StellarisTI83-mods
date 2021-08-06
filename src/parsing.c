#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "local_keys.h"
#include "generic_lists.h"
#include "parsing.h"

void remove_spaces(char* string) {
    const char* d = string;
    bool intoString = false;
    do {
        if(!intoString) {
            while (*d == ' ') {
                ++d;
            }
        }
        
        if((*d == '"')){
            intoString = !intoString;
            ++d;
        }
    } while (*string++ = *d++);
}



int config_getKey(const char* string, char* pointer) {
    char *i = 0;
    int position = 0;
    i = strchr(string, '=');
    position = (int)(i - string);
    strncpy(pointer, string, position);
    return 0;
}



int config_getValue(const char* string, char* pointer) {
    char *i = 0;
    i = strchr(string, '=') + sizeof(char);
    strcpy(pointer, i);
    if(pointer[strlen(pointer) - 1] == '\n')
        pointer[strlen(pointer) - 1] = '\0';
    return 0;
}

void config_file_parse(FILE *file, GenericList *liste) {
    int lineLength = MAX_LINE_LENGTH;
    char line[lineLength];
    int lineNumber;
    char keyRaw[MAX_KEY_LENGTH] = {0};
    char value[MAX_VALUE_LENGTH] = {0};
    local_keys key;

    configStruct *config;

    while(fgets(line, lineLength, file)) {
        
        remove_spaces(line);
        if((!strchr(line, '#')) && (line[0] != ' ') && (line[0] != '\n')) {
            key = lc_none;
            memset(keyRaw, 0, sizeof(keyRaw));
            memset(value, 0, sizeof(value));

            config_getKey(line, keyRaw);
            config_getValue(line, value);
            if(!strcmp(keyRaw, "lc_load")) {
                key = lc_load;
            } else if(!strcmp(keyRaw, "lc_new_game")) {
                key = lc_new_game;
            } else if(!strcmp(keyRaw, "lc_settings")) {
                key = lc_settings;
            } else if(!strcmp(keyRaw, "lc_exit")) {
                key = lc_exit;
            } else if(!strcmp(keyRaw, "lc_save")) {
                key = lc_save;
            } else if(!strcmp(keyRaw, "lc_back")) {
                key = lc_back;
            } else if(!strcmp(keyRaw, "lc_create_galaxie")) {
                key = lc_create_galaxie;
            } else {
                printf("Unknown key line %d: %s=%s\n", lineNumber, keyRaw, value);
            }
            if(key) {
                config = malloc(sizeof(configStruct));
                config->key = key;
                strcpy(config->value, value);
                printf("%d, %s\n", config->key, config->value);
                GenericCellAdd(liste, config);
            }
        }
        lineNumber++;
    }    
    printf("\n");

    if(line)
        free(line);
}