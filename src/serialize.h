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


void serialize_blockchain(Block *, FILE *);     //Fwrite binary-data blockchain to output file
void rebuild_block(FILE *, Block2 *);          //1. need to read back in unsigned chars from file produced by serialize blockchain. 
                                                 //2. link block (doubly linked list in read_block)
                                                 //3. rebuild blockchain by initialization + populate blocks 
                                                 //arugments: filename, pointer to block_counter, and array of blocks
void serialize_first_blockchain(Block *, FILE *);
void rebuild_first_block(FILE *, Block2 *); 

void rebuild_merkleTree(Block *);
//1. read in merkle Tree array representation
//2. sort by ID
//3. search for key-word leaf node
//4. rebuild bottom-up
//5. reassign pointers to children nodes 

//writing unsigned chars and reading-in unsigned chars


#endif