/* HEADER FILE FOR FUCNTIONS ASSOCIATED WITH MERKLE TREE */
#include "node.h"

FILE *Fopen(const char *, const char *);
size_t Fread(void *, size_t, size_t, FILE *); 
LeafNode *createLeafNodes(LeafNode *, char**, int);
InternalNode *convertLeaftoInternal(LeafNode *LeafNode, int count);
InternalNode *merkleTreeRoot(InternalNode *, int);
void print_merkle_tree(InternalNode *, int);
void Sort(char**, int);
char* hash(char arr[]);
