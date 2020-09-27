/* HEADER FILE FOR FUCNTIONS ASSOCIATED WITH MERKLE TREE */
#include "node.h"

LeafNode *createLeafNodes(LeafNode *, char**, int);
InternalNode *convertLeaftoInternal(LeafNode *LeafNode, int count);
InternalNode *merkleTreeRoot(InternalNode *, int);
void print_merkle_tree(InternalNode *, int);
FILE *Fopen(const char *, const char *);
void Fclose(FILE *);
size_t Fread(void *, size_t, size_t, FILE *); 
size_t Fwrite(void *, size_t, size_t, FILE *);
void Sort(char**, int);
unsigned char* hash(char arr[]);
