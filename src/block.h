#ifndef BLOCK_DEF
#define BLOCK_DEF

#include "sha256.h"
#include "merkleTree.h"

//header struct
struct header {
  unsigned char* previousHash; 
  char rootHash[32];
  int timestamp;
  double target;
  unsigned char* nonce;
};
typedef struct header Header;

//block struct
struct block {
  Header *header;
  char *rootHash[32];
};
typedef struct block Block;

Block *create_block(InternalNode *, Block *);
Block *initialize_block(InternalNode *, int *);
void populate_header(Header *, InternalNode *, Block *);
void initialize_header(Header *, InternalNode *, int *);
int *timestamp();
void generate_nonce(unsigned char *, InternalNode *);

#endif