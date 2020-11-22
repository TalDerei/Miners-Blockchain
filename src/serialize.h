/* HEADER FILE FOR SERIALIZING BLOCK */

#ifndef SERIALIZE_DEF
#define SERIALIZE_DEF

#include "block.h"
#include "printMerkleTree.h"
#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Block2 struct */
struct block2 {
  unsigned char previousHash[SHA256_BLOCK_SIZE]; 
  unsigned char rootHashHeader[SHA256_BLOCK_SIZE];
  int timestamp;
  double target;
  unsigned int nonce;
  unsigned char rootHash[SHA256_BLOCK_SIZE];
  struct block2 *prevBlock;
  struct block2 *nextBlock;
};
typedef struct block2 Block2;

/* function prototype for serializing blockchain */
void serialize_blockchain(Block *, FILE *);     
void rebuild_block(FILE *, Block2 *);          
void serialize_first_blockchain(Block *, FILE *);
void rebuild_first_block(FILE *, Block2 *); 
void rebuild_merkleTree(Block *);

#endif