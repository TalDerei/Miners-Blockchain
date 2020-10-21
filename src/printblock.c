/* IMPLEMENTATION FOR PRINTING BLOCK TO FILE*/

#include "printBlock.h"

void print_first_block(Block* B_ptr, int ID_K, FILE* out){
    fprintf(out, "BEGIN BLOCK\n");
    fprintf(out, "BEGIN HEADER\n");
    fprintf(out, "%s\n", B_ptr->header->previousHash);
    for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		fprintf(out,"%x", (unsigned char) B_ptr->header->rootHash[n]);
	}
    fprintf(out, "%d\n", B_ptr->header->timestamp);
    fprintf(out, "%f\n", B_ptr->header->target);
    fprintf(out, "%d\n", B_ptr->header->nonce);
    fprintf(out, "END HEADER\n");
    fprintf(out, "\n");
    print_merkle_tree(B_ptr->rootHash, ID_K, out);
    fprintf(out, "END BLOCK\n");
}

void print_block(Block* B_ptr, int ID_K, FILE* out){
    fprintf(out, "BEGIN BLOCK\n");
    fprintf(out, "BEGIN HEADER\n");
    for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		fprintf(out,"%x", (unsigned char) B_ptr->header->previousHash[n]);
	}
    for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		fprintf(out,"%x", (unsigned char) B_ptr->header->rootHash[n]);
	}
    fprintf(out, "%d\n", B_ptr->header->timestamp);
    fprintf(out, "%f\n", B_ptr->header->target);
    fprintf(out, "%d\n", B_ptr->header->nonce);
    fprintf(out, "END HEADER\n");
    fprintf(out, "\n");
    print_merkle_tree(B_ptr->rootHash, ID_K, out);
    fprintf(out, "END BLOCK\n");
}