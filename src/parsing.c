#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "keys.h"
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

int config_line_parse(char *line, configStruct *config, int lineNumber, GenericList *list) {
    char keyRaw[MAX_KEY_LENGTH] = {0};
    char value[MAX_VALUE_LENGTH] = {0};
    keys key;
    value_type type = type_error;

    if((!strchr(line, '#')) && (line[0] != ' ') && (line[0] != '\n') && (line[0] != '}')) {
        key = ky_none;
        memset(keyRaw, 0, sizeof(keyRaw));
        memset(value, 0, sizeof(value));

        config_getKey(line, keyRaw);
        config_getValue(line, value);

        if(!strcmp(value, "{")) {
            key = ky_struct;
            type = type_struct;
        }else if(!strcmp(keyRaw, "name")) {
            key = ky_name;
            type = type_string;
        } else if(!strcmp(keyRaw, "ship_size")) {
            key = ky_ship_size;
            type = type_int;
        } else {
            printf("Unknown key line %d: %s=%s\n", lineNumber, keyRaw, value);
        }

        if(key) {
            config = malloc(sizeof(configStruct));
            config->key = key;
            config->type = type;

            switch (type) {
                case type_string:
                    strcpy(config->value_string, value);
                    #ifdef DEBUG
                    printf("key = %d value = '%s'\n", config->key, config->value_string);
                    #endif
                    break;
                case type_int:
                    config->value_int = atoi(value);
                    #ifdef DEBUG
                    printf("key = %d value = %d\n", config->key, config->value_int);
                    #endif
                    break;
                case type_struct:
                    config->value_struct = GenericListCreate();
                    config->mother_list = list;
                    printf("open struct '%p' with mother '%p'\n", config->value_struct, config->mother_list);
                default:
                    printf("Error unknow type line %d\n", lineNumber);
                    break;
            }

            GenericCellAdd(list, config);
        }

    } else {
        #ifdef DEBUG
        printf("Nothing line %d\n", lineNumber);
        #endif
    }
    return type;
}

void config_file_parse(FILE *file, GenericList *list) {
    int lineLength = MAX_LINE_LENGTH;
    char line[lineLength];
    int lineNumber = 1;
    value_type type = type_error;

    configStruct *config;

    while(fgets(line, lineLength, file)) {
        remove_spaces(line);
        #ifdef DEBUG
        printf("\n%d | %s> ", lineNumber, line);
        #endif
        type = config_line_parse(line, config, lineNumber, list);
        lineNumber++;
    }    

    if(line)
        free(line);
}