/* HEADER FILE FOR VALIDATING BLOCK */

#ifndef VALIDATE_DEF
#define VALIDATE_DEF

#include "serialize.h"
#include "block.h"
#include "merkleTree.h"

/* function prototypes for validating blockchain */
int validation(Block2 *, char [], char [], int);
int validate_header(Block2 *, char [], int);
int validate_merkle_tree(Block2 *, char []);
int compareMerkleTree(InternalNode *, FILE *, char *, int);
void clearBuffer(char *);

#endif
