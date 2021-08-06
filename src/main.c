#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "local_keys.h"
#include "generic_lists.h"
#include "appvar.h"
#include "parsing.h"

void delete_list(GenericList *liste){
    int i = 0;
    configStruct *config;
    config = GenericCellGet(liste, i);
    while(config) {
        printf("%d, %s\n", config->key, config->value);
        free(config);
        i++;
        config = GenericCellGet(liste, i);
    }
    GenericListFree(liste);
    printf("\n");
}

int main() {
    FILE *spaceship_file;

    GenericList *spaceship_liste = GenericListCreate();

    spaceship_file = fopen("config files/spaceship.txt", "r");
    
    
    config_file_parse(spaceship_file, spaceship_liste);

    appvar_ecrire("data.8xv", "data_1.0", "Data for Stellaris", spaceship_liste);

    fclose(spaceship_file);

    delete_list(spaceship_file);

    return 0;
}