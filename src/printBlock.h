/* HEADER FILE FOR PRINTBLOCK.C */

#ifndef PRINTBLOCK_DEF
#define PRINTBLOCK_DEF

#include <stdio.h>
#include "block.h"
#include "hash.h"
#include "merkleTree.h"
#include <string.h>
#include "sha256.h"

void serialize_block(Block*, int, FILE*);

#endif