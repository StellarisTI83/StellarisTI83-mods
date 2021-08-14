#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "keys.h"
#include "generic_lists.h"
#include "parsing.h"

int parsed_keys = 0;
int invalid_keys = 0;

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
    if(i) {
        position = (int)(i - string);
        strncpy(pointer, string, position);
    } else {
        strcpy(pointer, string);
        if(pointer[strlen(pointer) - 1] == '\n')
            pointer[strlen(pointer) - 1] = '\0';
    }
    return 0;
}



int config_getValue(const char* string, char* pointer) {
    char *i = 0;
    i = strchr(string, '=');
    if(i){
        i += sizeof(char);
        strcpy(pointer, i);
        if(pointer[strlen(pointer) - 1] == '\n')
            pointer[strlen(pointer) - 1] = '\0';
    }
    return 0;
}

configStruct *config_line_parse(char *line, int lineNumber, GenericList *list) {
    char keyRaw[MAX_KEY_LENGTH] = {0};
    char value[MAX_VALUE_LENGTH] = {0};
    keys key;
    value_type type = type_error;
    configStruct *config = NULL;

    if((!strchr(line, '#')) && (line[0] != ' ') && (line[0] != '\n')) {
        key = ky_none;
        memset(keyRaw, 0, sizeof(keyRaw));
        memset(value, 0, sizeof(value));

        config_getKey(line, keyRaw);
        config_getValue(line, value);
        if(!strcmp(value, "{")) {
            key = ky_struct;
            type = type_struct;
        }else if(!strcmp(keyRaw, "}")) {
            key = ky_struct;
            type = type_struct_end;
        }else if(!strcmp(keyRaw, "name")) {
            key = ky_name;
            type = type_string;
        } else if(!strcmp(keyRaw, "ship_type")) {
            key = ky_ship_type;
            type = type_int;
        } else if(!strcmp(keyRaw, "power")) {
            key = ky_power;
            type = type_int;
        } else if(!strcmp(keyRaw, "hull_life")) {
            key = ky_hull_life;
            type = type_int;
        } else if(!strcmp(keyRaw, "armor_life")) {
            key = ky_armor_life;
            type = type_int;
        } else if(!strcmp(keyRaw, "shield_life")) {
            key = ky_shield_life;
            type = type_int;
        } else {
            printf("Unknown key line %d: %s\n", lineNumber, keyRaw);
        }
        if(type == type_struct_end) {
            #ifdef DEBUG
            printf("close struct\n");
            #endif
            config = malloc(sizeof(configStruct));
            config->key = key;
            config->type = type;
        } else if(key) {
            config = malloc(sizeof(configStruct));
            config->key = key;
            config->type = type;

            switch (type) {
                case type_string:
                    strcpy(config->value_string, value);
                    #ifdef DEBUG
                    printf("key = %d value = '%s'\n", config->key, config->value_string);
                    #endif
                    parsed_keys++;
                    break;
                case type_int:
                    config->value_int = atoi(value);
                    #ifdef DEBUG
                    printf("key = %d value = %d\n", config->key, config->value_int);
                    #endif
                    parsed_keys++;
                    break;
                case type_struct:
                    config->value_struct = GenericListCreate();
                    config->mother_list = list;
                    strcpy(config->struct_name, keyRaw);
                    #ifdef DEBUG
                    printf("open struct '%s' at '0x%p' with mother '0x%p'\n", config->struct_name, config->value_struct, config->mother_list);
                    #endif
                    break;
                default:
                    printf("Error unknow type line %d\n", lineNumber);
                    invalid_keys++;
                    break;
            }

            GenericCellAdd(list, config);
        }

    } else {
        #ifdef DEBUG
        printf("Nothing line %d\n", lineNumber);
        #endif
    }
    return config;
}

void config_file_parse(FILE *file, GenericList *list) {
    int lineLength = MAX_LINE_LENGTH;
    char line[lineLength];
    int lineNumber = 1;
    value_type type = type_error;

    configStruct *config, *base_config;

    while(fgets(line, lineLength, file)) {
        remove_spaces(line);
        #ifdef DEBUG
        printf("\n%d| %s> ", lineNumber, line);
        #endif
        config = config_line_parse(line, lineNumber, list);
        if(config){
            type = config->type;
        }
        if(type == type_struct) {
            base_config = config;
            list = config->value_struct;
        }
        if(type == type_struct_end) {
            free(config);
            config = base_config;
            list = config->mother_list;
        }
        lineNumber++;
        type = type_error;
    }  
    printf("\nParsed keys : %d\nInvalid keys : %d\n", parsed_keys, invalid_keys);

    if(line)
        free(line);
}