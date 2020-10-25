/* HEADER FILE FOR VALIDATING BLOCK */

#ifndef VALIDATE_DEF
#define VALIDATE_DEF

#include "serialize.h"
#include "block.h"
#include "merkleTree.h"

int validation(Block2 *, char [], char []);
int validate_header(Block2 *, char []);
int validate_merkle_tree(Block2 *, char []);
int compareMerkleTree(InternalNode *, FILE *, char *, int);
void clearBuffer(char *);
void substring(char *, int);

#endif
