#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "sha256.h"
#include "merkleTree.h"
#include "node.h"

char *hash(char arr[]) {
    printf ("value passed in: %s\n", arr);

    BYTE *buf = malloc(sizeof(BYTE));
	SHA256_CTX ctx;
	int idx;

	sha256_init(&ctx);
	sha256_update(&ctx, arr, strlen(arr));
	sha256_final(&ctx, buf);
    /* Print the digest as one long hex value */ 
    /*for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
        printf("%x", buf[n]);
    }*/
    //printf("\nfinal hash value is: %x\n", buf);
    return buf;
}