/* HEADER FILE FOR PRINTING BLOCK */

#ifndef PRINTBLOCK_DEF
#define PRINTBLOCK_DEF

#include <stdio.h>
#include <string.h>
#include "block.h"
#include "merkleTree.h"
#include "hash.h"
#include "sha256.h"

void print_block(Block*, int, FILE*);

#endif