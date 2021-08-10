#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "appvar.h"
#include "generic_lists.h"
#include "parsing.h"

int sizeof_data(GenericList* liste) {
    configStruct *config;
    int fileSize = 0;
    int i = 0;
    config = GenericCellGet(liste, i);
    while(config) {
        switch(config->type) {
            case type_string:
                fileSize += sizeof(uint16_t) + sizeof(config->key) + sizeof(char) * strlen(config->value_string);
                break;
            case type_int:
                fileSize += sizeof(uint16_t) + sizeof(config->key) + sizeof(uint16_t);
                break;
        }
        i++;
        config = GenericCellGet(liste, i);
    }
    fileSize += sizeof(uint16_t);
    return fileSize;
}

void write_data(unsigned char *data, GenericList* liste, size_t fileSize) {
    configStruct *config;
    uint16_t i = 0;
    uint16_t data_index = 0;
    uint16_t dataBlockSize = 0;
    uint16_t key = 0;
    bool error = false;
    int list_size = 0;
    

    #ifdef DEBUG
    printf("\nWrite data :\nsize = %d\n", GenericListArraySize(liste));
    #endif

    data_index += 2;
    config = GenericCellGet(liste, i);
    while(config) {
        //taille de la variable
        switch (config->type) {
            case type_string:
                dataBlockSize = sizeof(char) * strlen(config->value_string);
                break;
            case type_int:
                dataBlockSize = sizeof(uint16_t);
                break;
            default:
                #ifdef DEBUG
                printf("Error unknow type '%d'\n", config->type);
                #endif
                error = true;
                i++;
                config = GenericCellGet(liste, i);
                continue;
                break;
        }
        memcpy(&data[data_index], &dataBlockSize, sizeof(uint16_t));
        data_index += sizeof(uint16_t);
        #ifdef DEBUG
        printf("dataBlockSize = %d   ", dataBlockSize);
        #endif

        //clef de la variable
        key = config->key;
        memcpy(&data[data_index], &(key), sizeof(uint16_t));
        data_index += sizeof(uint16_t);
        #ifdef DEBUG
        printf("key = %d   ", key);
        #endif

        //copier variable
        switch (config->type) {
            case type_string:
                memcpy(&data[data_index], &(config->value_string), dataBlockSize);
                #ifdef DEBUG
                printf("value(%d / string) = '%s'\n", dataBlockSize, config->value_string);
                #endif
                break;
            case type_int:
                memcpy(&data[data_index], &(config->value_int), dataBlockSize);
                #ifdef DEBUG
                printf("value(%d / int) = %d\n", dataBlockSize, config->value_int);
                #endif
                break;
        }
        if(!error)
            data_index += dataBlockSize;

        error = false;

        list_size++;
        i++;
        config = GenericCellGet(liste, i);
    }

    data[0] = ((list_size) & 0xFF);
    data[1] = (((list_size)>>8) & 0xFF);
}

void appvar_ecrire(char* nomDuFichier, char* nomDeLappvar, char* comment, GenericList* liste) {
    FILE *fptr;
    int fileSize = 0;
    unsigned char header[] = {
        0x2A, 0x2A, 0x54, 0x49, 0x38, 0x33, 0x46, 0x2A,		// **TI83F*
        0x1A, 0x0A, 0x00,									// signature

                          0x00, 0x00, 0x00, 0x00, 0x00,     // comment area 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00,

                                      0x00, 0x00            //data size
    };
    unsigned char varheader[] = {
        0x0D, 0x00,											// start of variable header
        0x00, 0x00,											// length of variable in bytes
        0x15,												// variable type ID. 0x15 is for appvar
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// variable name (max 8 characters)
        0x00,												// version
        0x80,												// flag. 80h for archived variable. 00h otherwise
        0x00, 0x00,											// length of variable in bytes (copy)
        0x00, 0x00	
    };

    unsigned char *data;

    unsigned char checksum[] = {
        0x00, 0x00                                          // checksum of the file
    };

    int i = 0;
    int sum = 0;

    // ecrire le commentaire
    for (i = 0; i < 42; i++) {
        if (i >= strlen(comment))
            header[11 + i] = 0x00;
        else
            header[11 + i] = comment[i];
    }

    fileSize = sizeof_data(liste);
    data = malloc(fileSize);
    
    write_data(data, liste, fileSize);
    
    header[53] = ((fileSize + sizeof(varheader)/sizeof(varheader[0])) & 0xFF);
    header[54] = ((fileSize + sizeof(varheader)/sizeof(varheader[0])>>8) & 0xFF);

    varheader[2] = ((fileSize + 2) & 0xFF);
    varheader[3] = (((fileSize + 2)>>8) & 0xFF);
    varheader[15] = varheader[2];
    varheader[16] = varheader[3];
    varheader[17] = ((fileSize) & 0xFF);
    varheader[18] = (((fileSize)>>8) & 0xFF);

    //ecrire le nom du fichier
    for (i = 0; i < 8; i++) {
        if (i >= strlen(nomDeLappvar))
            varheader[5 + i] = 0x00;
        else
            varheader[5 + i] = nomDeLappvar[i];
    }

    for (int i = 0; i < fileSize; i++)
        sum += data[i];
    for (int i = 0; i < sizeof(varheader); i++)
        sum += varheader[i];
    checksum[0] = ((sum) & 0xff);
    checksum[1] = ((sum>>8) & 0xff);

    fptr = fopen(nomDuFichier, "wb");
    if(fptr == NULL) {
        return;
    }

    fwrite(header, sizeof(header[0]), sizeof(header)/sizeof(header[0]), fptr);
    fwrite(varheader, sizeof(varheader[0]), sizeof(varheader)/sizeof(varheader[0]), fptr);
    fwrite(data, fileSize, 1, fptr);
    fwrite(checksum, sizeof(checksum[0]), sizeof(checksum)/sizeof(checksum[0]), fptr);
    fclose(fptr);
}