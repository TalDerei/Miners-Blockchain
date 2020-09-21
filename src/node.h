/* NODE STRUCTURE */	

#ifndef NODE_DEF
#define NODE_DEF

#include <stdint.h>   /* for uint*_t types */

//leaf node
struct leaf_node {
  uint64_t *hash;
  char* value;
};
typedef struct leaf_node LeafNode;

//internal node
struct node {
  uint64_t hash;
  struct node *leftChild;
  struct node *rightChild;
  //string leftEdge;
  //string rightEdge;
};
typedef struct node InternalNode;

#endif