/* HEADER FILE FOR SERIALIZING BLOCK */

#ifndef SERIALIZE_DEF
#define SERIALIZE_DEF

#include "block.h"
#include "printMerkleTree.h"
#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct block2 {
  unsigned char previousHash[32]; 
  unsigned char rootHashHeader[32];
  int timestamp;
  double target;
  unsigned int nonce;
  unsigned char *rootHash;
  struct block2 *prevBlock;
  struct block2 *nextBlock;
};
typedef struct block2 Block2;

void serialize_blockchain(Block *, FILE *);     
void rebuild_block(FILE *, Block2 *);          
void serialize_first_blockchain(Block *, FILE *);
void rebuild_first_block(FILE *, Block2 *); 
void rebuild_merkleTree(Block *);

#endif