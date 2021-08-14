#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "keys.h"
#include "generic_lists.h"
#include "appvar.h"
#include "parsing.h"

void delete_list(GenericList *liste){
    int i = 0;
    configStruct *config;
    config = GenericCellGet(liste, i);

    #ifdef DEBUG
    printf("Destroy list :\n");
    #endif

    while(config) {
        switch(config->type) {
            case type_string:
                #ifdef DEBUG
                printf("String key = %d value = '%s'\n", config->key, config->value_string);
                #endif
                break;
            case type_int:
                #ifdef DEBUG
                printf("Int key = %d value = %d\n", config->key, config->value_int);
                #endif
                break;
            case type_struct:
                #ifdef DEBUG
                printf("Struct key = %d value = %p\n", config->key, config->value_struct);
                #endif
                delete_list(config->value_struct);
                break;
        }
        free(config);
        i++;
        config = GenericCellGet(liste, i);
    }
    GenericListFree(liste);
    #ifdef DEBUG
    printf("End of list\n");
    #endif
}

int main() {
    FILE *spaceship_file;

    GenericList *spaceship_liste = GenericListCreate();

    spaceship_file = fopen("config files/spaceship.txt", "r");
    
    
    config_file_parse(spaceship_file, spaceship_liste);

    appvar_ecrire("data.8xv", "data", "Data for Stellaris", spaceship_liste);

    fclose(spaceship_file);

    #ifdef DEBUG
    printf("\n");
    #endif
    delete_list(spaceship_liste);
    
    #ifdef DEBUG
    printf("\nEXIT_SUCCESS");
    #endif
    exit(EXIT_SUCCESS);
}