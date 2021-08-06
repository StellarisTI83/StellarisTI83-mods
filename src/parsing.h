#ifndef PARSING_H
#define PARSING_H

#define MAX_KEY_LENGTH 100
#define MAX_LINE_LENGTH 255
#define MAX_VALUE_LENGTH 255

struct _configStruct {
    local_keys key;
    char value[MAX_VALUE_LENGTH];
};

typedef struct _configStruct configStruct;

void config_file_parse(FILE *file, GenericList *liste);

#endif