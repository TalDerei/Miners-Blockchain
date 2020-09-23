#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "sha256.h"
#include "merkleTree.h"
#include "node.h"

uint64_t hash(char arr[]) {

BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int idx;

	sha256_init(&ctx);
	sha256_update(&ctx, arr, strlen(arr));
	sha256_final(&ctx, buf);
    /* Print the digest as one long hex value */ 
    for (int n = 0; n <= SHA256_BLOCK_SIZE; n++) {
        printf("%x", buf[n]);
    }
    putchar('\n');
}