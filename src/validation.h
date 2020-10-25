/* HEADER FILE FOR VALIDATING BLOCK */

#ifndef VALIDATE_DEF
#define VALIDATE_DEF

#include "serialize.h"
#include "block.h"
#include "merkleTree.h"

int validation(block2 **, char [], char []);
int validate_header(Block2 *, char []);
int validate_merkle_tree(Block2 *, char []);
int compareMerkleTree(InternalNode *, FILE *, char *);
void clearBuffer(char *);
void substring(char *, int);

#endif
