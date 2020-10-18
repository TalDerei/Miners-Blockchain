/* HEADER FILE FOR PRINTING BLOCK */

#ifndef PRINTBLOCK_DEF
#define PRINTBLOCK_DEF

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "block.h"
#include "merkleTree.h"
#include "printMerkleTree.h"
#include "printBlock.h"
#include "sha256.h"
#include "time.h"
#include "hash.h"

void print_block(Block*, int, FILE*);

#endif