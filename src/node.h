/* NODE STRUCTURE */	

#ifndef NODE_DEF
#define NODE_DEF

#include <stdint.h>    /* for uint*_t types */

struct leaf_node {
  uint64_t hash;
  int value;
};
typedef struct leaf_node LeafNode;

struct internal_node {
  uint64_t hash;
  string leftEdge;
  string rightEdge;
  struct node *leftChild;
  struct node *rightChild;
};
typedef struct internal_node InternalNode;

#endif

