/* NODE STRUCTURE */	

#ifndef NODE_DEF
#define NODE_DEF

//leaf node struct
struct leaf_node {
  char hash[64];
  char value[100];
};
typedef struct leaf_node LeafNode;

//internal node struct
struct node {
  char hash[32];
  struct node *leftChild;
  struct node *rightChild;
  char leftEdge[100];
  char rightEdge[100];
};
typedef struct node InternalNode;

#endif