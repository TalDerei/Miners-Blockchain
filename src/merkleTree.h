/* HEADER FILE FOR FUNCTIONS ASSOCIATED WITH MERKLE TREE */

#ifndef MERKLETREE_DEF
#define MERKLETREE_DEF

#include <stdio.h>
#include "sha256.h"
#define BUFFER 100

/* leaf node struct */
struct leaf_node {
  char hash[SHA256_BLOCK_SIZE + 1];
  char value[100];
};
typedef struct leaf_node LeafNode;

/* internal node struct */
struct node {
  char hash[2 * SHA256_BLOCK_SIZE + 1];
  struct node *leftChild;
  struct node *rightChild;
  char leftEdge[100];
  char rightEdge[100];
};
typedef struct node InternalNode;

/* function prototypes for merkle-tree */
void createLeafNodes(LeafNode *, char **, int);
void convertLeaftoInternal(InternalNode *, LeafNode *, int);
InternalNode *merkleTreeRoot(InternalNode *, int);

#endif