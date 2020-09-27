#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "sha256.h"
#include "merkleTree.h"
#include "node.h"

unsigned char *hash(char arr[]) {
    //printf ("content passed in: %s\n", arr);

    BYTE* buf = malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    //initialize, update, and final functions to produce hash digest
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, strlen(arr));
	sha256_final(&ctx, buf);
    printf ("content passed out:\n");
    for(int i = 0 ; i < SHA256_BLOCK_SIZE; i++){
        printf("%x\n", buf[i]);
    }
    printf("\n");
    return buf;
}