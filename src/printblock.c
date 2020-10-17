/* HEADER FILE FOR FUCNTIONS ASSOCIATED WITH MERKLE TREE */

#include "printBlock.h"

void serialize_block(Block* B_ptr, int ID_K, FILE* out){
 fprintf(out, "header: \n");
 fprintf(out, "PreviousHash: %s, RootHash: %s", B_ptr->header->previousHash, B_ptr->header->rootHash);
 fprintf(out, "");
 fprintf(out, "TimeStamp: %d, Target: %s, nonce: %d", B_ptr->header->timestamp, B_ptr->header->target, B_ptr->header->nonce);
 fprintf(out, "");
 fprinff(out, "content: \n");
 print_merkle_tree(B_ptr->rootHash, ID_K, out);
}