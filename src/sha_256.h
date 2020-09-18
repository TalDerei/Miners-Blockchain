/* HEADER FILE FOR SHA-256 ALGORITHM FOR ENCRYPTION */
//source: https://github.com/B-Con/crypto-algorithms; with minor modifications

/*********************************************************************
* Filename:   sha_256.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding SHA1 implementation.
*********************************************************************/

#ifndef sha_256_H
#define sha_256_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define sha_256_BLOCK_SIZE 32            // sha_256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef size_t BYTE;             // size_t byte
typedef unsigned int  WORD;             // 32-bit word, change to "long" for 16-bit machines

typedef struct {
	BYTE data[64];
	WORD datalen;
	unsigned long long bitlen;
	WORD state[8];
} sha_256_CTX;

/*********************** FUNCTION DECLARATIONS **********************/
void sha_256_init(sha_256_CTX *ctx);
void sha_256_update(sha_256_CTX *ctx, const BYTE data[], size_t len);
void sha_256_final(sha_256_CTX *ctx, BYTE hash[]);

#endif   // sha_256_H