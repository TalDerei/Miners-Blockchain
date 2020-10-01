/* HEADER FILE FOR FUCNTIONS ASSOCIATED WITH MERKLE TREE */
#include <stdio.h>
#include "node.h"
#include "block.h"
#define BUFFER 100

LeafNode *createLeafNodes(LeafNode *, char**, int);
InternalNode *convertLeaftoInternal(LeafNode *LeafNode, int count);
InternalNode *merkleTreeRoot(InternalNode *, int);
void print_merkle_tree(InternalNode *, int);
FILE *Fopen(const char *, const char *);
void Fclose(FILE *);
size_t Fread(void *, size_t, size_t, FILE *); 
//size_t Fwrite(void *, size_t, size_t, FILE *);
void Sort(char**, int);
unsigned char* hash(char arr[]);
Block *create_block(InternalNode *, Block *);
Block *initialize_block(InternalNode *);
void populate_header(Header *, InternalNode *, Block *);
void initialize_header(Header *, InternalNode *);
int *timestamp();
void generate_nonce(unsigned char *, InternalNode *);