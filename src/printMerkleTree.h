/* HEADER FILE FOR PRINTING MERKLE TREE */  

#ifndef PRINT_MERKLETREE_DEF
#define PRINT_MERKLETREE_DEF

#include "merkleTree.h"

/* function prototypes for printing merkle tree */
void print_merkle_tree(InternalNode *, int, FILE *);
void print_merkle_tree_value(char **, int, FILE *);

#endif