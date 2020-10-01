/* NODE STRUCTURE */	

#ifndef NODE_DEF
#define NODE_DEF

#include "sha256.h"

//leaf node struct
struct leaf_node {
  char hash[SHA256_BLOCK_SIZE + 1];
  char value[100];
};
typedef struct leaf_node LeafNode;

//internal node struct
struct node {
  char hash[2*SHA256_BLOCK_SIZE + 1];
  struct node *leftChild;
  struct node *rightChild;
  char leftEdge[100];
  char rightEdge[100];
};
typedef struct node InternalNode;

//header struct
struct header {
  unsigned char* previousHash[2*SHA256_BLOCK_SIZE + 1]; 
  unsigned char rootHash[32];
  int timestamp;
  size_t nonce;
  size_t target;
};
typedef struct header Header;

//block struct
struct block {
  Header *header;
  char *rootHash[32];
};
typedef struct block Block;

#endif