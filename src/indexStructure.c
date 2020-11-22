/* IMPLEMENTATION FOR SEARCHING FOR STRING IN A BLOCKCHAIN */

#include "indexStructure.h"
#define BUFFER 100

int inchain(char search_block[], char search_string[]) {
    char buffer[BUFFER];

    //open file associated with outputBlock file handler
    FILE *outputBlock; 
    outputBlock = fopen(search_block,"rb");

    while (fgets(buffer, BUFFER, outputBlock) != NULL) {
        if ((strstr(buffer, search_string)) != NULL) {
            printf("search_string found in block!");
            return 1;
        }
        clear_buffer(buffer);
    }
    printf("search_string NOT FOUND");
    return 0;
}

void clear_buffer(char buffer[]) {
    for(int i = 0 ; i < BUFFER; i++){
        buffer[i] = NULL;
    }
}

