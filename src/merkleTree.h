/* HEADER FILE FOR FUCNTIONS ASSOCIATED WITH MERKLE TREE */
/* char *sorting(ContentArray *); */
#include "node.h"

FILE *Fopen(const char *, const char *);
size_t Fread(void *, size_t, size_t, FILE *); 
LeafNode **merkleTree(char *, int);
/* InternalNode *merkleTreeRoot(merkleTree **); */
