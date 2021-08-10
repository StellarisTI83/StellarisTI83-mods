#ifndef PARSING_H
#define PARSING_H

#define MAX_KEY_LENGTH 100
#define MAX_LINE_LENGTH 255
#define MAX_VALUE_LENGTH 255

#include "keys.h"
#include "generic_lists.h"

struct _configStruct {
    keys key;
    value_type type;
    union {
        int value_int;
        char value_string[MAX_VALUE_LENGTH];
        struct {
            GenericList *value_struct;
            GenericList *mother_list;
        };
    };
};

typedef struct _configStruct configStruct;

void config_file_parse(FILE *file, GenericList *liste);

#endif