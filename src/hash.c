/* HASH IMPLEMENTATION */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "sha256.h"
#include "merkleTree.h"
#include "hash.h"
#define BlockSize (3*SHA256_BLOCK_SIZE + 4*sizeof(int))

unsigned char *hash(unsigned char *arr) {
    //printf("content passed in: %s\n", arr);
    BYTE* buf = malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    //initialize, update, and final functions to produce 32-byte hash digest
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, strlen(arr));
	sha256_final(&ctx, buf);
    // printf ("\ncontent passed out:\n");
    // for(int i = 0 ; i < SHA256_BLOCK_SIZE; i++){
    //     printf("%x", buf[i]);
    // }
    // printf("\n");
    return (unsigned char *)buf;
}

//for some reason, when passing in a strcat of two hashes, it reads strlen(arr) 30 instead of 64
//So we hard coded 64 into the code with 2*SHA256_BLOCK_SIZE
//This has to do with strlen()!
unsigned char *hash64(unsigned char *arr) {
    //printf("content passed in: %s\n", arr);
    BYTE* buf = malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    //initialize, update, and final functions to produce 32-byte hash digest
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, 2*SHA256_BLOCK_SIZE);
	sha256_final(&ctx, buf);
    // printf ("\ncontent passed out:\n");
    // for(int i = 0 ; i < SHA256_BLOCK_SIZE; i++){
    //     printf("%x", buf[i]);
    // }
    // printf("\n");
    return (unsigned char *)buf;
}


unsigned char *hash112(unsigned char *arr) {
    //printf("content passed in: %s\n", arr);
    BYTE* buf = malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    //initialize, update, and final functions to produce 32-byte hash digest
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, BlockSize);
	sha256_final(&ctx, buf);
    // printf ("\ncontent passed out:\n");
    // for(int i = 0 ; i < SHA256_BLOCK_SIZE; i++){
    //     printf("%x", buf[i]);
    // }
    // printf("\n");
    return (unsigned char *)buf;
}