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

#endif