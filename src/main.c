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
    printf("\nDestroy list :\n");
    #endif

    while(config) {
        switch(config->type) {
            case type_string:
                #ifdef DEBUG
                printf("key = %d value = '%s'\n", config->key, config->value_string);
                #endif
                break;
            case type_int:
                #ifdef DEBUG
                printf("key = %d value = %d\n", config->key, config->value_int);
                #endif
                break;
        }
        free(config);
        i++;
        config = GenericCellGet(liste, i);
    }
    GenericListFree(liste);
}

int main() {
    FILE *spaceship_file;

    GenericList *spaceship_liste = GenericListCreate();

    spaceship_file = fopen("config files/spaceship.txt", "r");
    
    
    config_file_parse(spaceship_file, spaceship_liste);

    appvar_ecrire("data.8xv", "data_1.0", "Data for Stellaris", spaceship_liste);

    fclose(spaceship_file);

    delete_list(spaceship_liste);
    
    #ifdef DEBUG
    printf("\nEXIT_SUCCESS");
    #endif
    exit(EXIT_SUCCESS);
}