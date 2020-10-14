#include <stdio.h>
#include "block.h"
#include "hash.h"
#include "merkleTree.h"
#include <string.h>
#include "sha256.h"

void printblock(block* B_ptr){
 printf("header: \n");
 printf("PreviousHash: %s, RootHash: %s", B_ptr->header->previousHash, B_ptr->header->rootHash);
 printf("");
 printf("TimeStamp: %d, Target: %s, nonce: %d", B_ptr->header->timestamp, B_ptr->header->target, B_ptr->header->nonce);
 printf("");
 prinff("content: \n");
 print_merkle_tree(B_ptr->rootHash);
}