/* IMPLEMENTATION FOR PRINTING BLOCK TO FILE*/

#include "printBlock.h"

//NEED TO FIX THIS FILE

void print_block(Block* B_ptr, int ID_K, FILE* out){
    printf("entered print_block!!!!!!!!!!!!!!!!!\n");
    fprintf(out, "header: \n");
    fprintf(out, "PreviousHash: %s, RootHash: %s", B_ptr->header->previousHash, B_ptr->header->rootHash);
    fprintf(out, "");
    fprintf(out, "TimeStamp: %d, Target: %s, nonce: %d", B_ptr->header->timestamp, B_ptr->header->target, B_ptr->header->nonce);
    fprintf(out, "");
    fprintf(out, "content: \n");
    print_merkle_tree(B_ptr->rootHash, ID_K, out);
}