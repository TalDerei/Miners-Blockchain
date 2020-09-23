/* HEADER FILE FOR FUCNTIONS ASSOCIATED WITH MERKLE TREE */
#include "node.h"

FILE *Fopen(const char *, const char *);
size_t Fread(void *, size_t, size_t, FILE *); 
LeafNode *createLeafNodes(LeafNode *, char**, int);
InternalNode *merkleTreeRoot(LeafNode *, int);
void Sort(char**, int);
uint64_t hash(char []);
