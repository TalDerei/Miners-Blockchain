/* HASH IMPLEMENTATION */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "sha256.h"
#include "merkleTree.h"
#include "hash.h"
#define BlockSize (3*SHA256_BLOCK_SIZE + 4*sizeof(int))

/* different hash function depending on the size of the data passed-in */

unsigned char *hash(unsigned char *arr) {
    BYTE* buf = (BYTE *)malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    /* initialize, update, and final functions to produce 32-byte hash digest */
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, strlen(arr));
	sha256_final(&ctx, buf);
    return (unsigned char *)buf;
}

unsigned char *hash64(unsigned char *arr) {
    BYTE* buf = (BYTE *)malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    /* initialize, update, and final functions to produce 32-byte hash digest */
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, 2*SHA256_BLOCK_SIZE);
	sha256_final(&ctx, buf);
    return (unsigned char *)buf;
}

unsigned char *hash112(unsigned char *arr) {
    BYTE* buf = (BYTE *)malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    /* initialize, update, and final functions to produce 32-byte hash digest */
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, BlockSize);
	sha256_final(&ctx, buf);
    return (unsigned char *)buf;
}