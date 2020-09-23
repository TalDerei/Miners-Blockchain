/* NODE STRUCTURE */	

#ifndef NODE_DEF
#define NODE_DEF

#include <stdint.h>   /* for uint*_t types */

//leaf node
struct leaf_node {
  char hash[32];
  char value[100];
};
typedef struct leaf_node LeafNode;

//internal node
struct node {
  char hash[32];
  struct node *leftChild;
  struct node *rightChild;
  char leftEdge;
  char rightEdge;
};
typedef struct node InternalNode;

#endif