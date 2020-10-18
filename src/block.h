/* HEADER FILE FOR BLOCK.C */

#ifndef BLOCK_DEF
#define BLOCK_DEF

#include "sha256.h"
#include "merkleTree.h"

//header struct
struct header {
  unsigned char* previousHash; 
  unsigned char rootHash[32];
  int timestamp;
  double target;
  unsigned int nonce;
};
typedef struct header Header;

//block struct
struct block {
  Header *header;
  InternalNode *rootHash;
};
typedef struct block Block;

void create_block(Block *, InternalNode *, Block *, FILE *);
void initialize_block(Block *, InternalNode *, unsigned char *, FILE *);
void populate_header(Block *, InternalNode *, Block *);
void initialize_header(Block *, InternalNode *, unsigned char *);
void generate_nonce(Header *, InternalNode *);
int *timestamp();

#endif

