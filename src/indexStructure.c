/* IMPLEMENTATION FOR SEARCHING FOR STRING IN A BLOCKCHAIN */

#include "indexStructure.h"
#define BUFFER 100

/* inchain provides proof of membership */
int inchain(char search_block[], char search_string[]) {
    char buffer[BUFFER];
    FILE *outputBlock; 
    outputBlock = fopen(search_block,"rb");

    while (fgets(buffer, BUFFER, outputBlock) != NULL) {
        if ((strstr(buffer, search_string)) != NULL) {
            return 1;
        }
        clear_buffer(buffer);
    }
    return 0;
}

/* clear buffer for re-use */
void clear_buffer(char buffer[]) {
    for(int i = 0 ; i < BUFFER; i++){
        buffer[i] = NULL;
    }
}

