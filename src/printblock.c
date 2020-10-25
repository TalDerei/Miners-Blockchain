/* IMPLEMENTATION FOR PRINTING BLOCK TO FILE */

#include "printBlock.h"

/* printing out the first block */
void print_first_block(Block *B_ptr, int ID_K, FILE *out){
    fprintf(out, "BEGIN BLOCK\n");
    fprintf(out, "BEGIN HEADER\n");
    fprintf(out, "previous hash is: %s\n", B_ptr->header->previousHash);
    fprintf(out, "\n");
    fprintf(out, "\nroot hash is: ");
    for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		fprintf(out,"%x", (unsigned char) B_ptr->header->rootHash[n]);
	}
    fprintf(out, "\ntimestamp is: %d\n", B_ptr->header->timestamp);
    fprintf(out, "target value is: %f\n", B_ptr->header->target);
    fprintf(out, "nonce is: %d\n", B_ptr->header->nonce);
    fprintf(out, "END HEADER\n");
    print_merkle_tree(B_ptr->rootHash, ID_K, out);
    fprintf(out, "\nEND BLOCK");
}

/* printing out additional blocks */
void print_block(Block* B_ptr, int ID_K, FILE* out){
    fprintf(out, "BEGIN BLOCK\n");
    fprintf(out, "BEGIN HEADER\n");
    fprintf(out, "previous hash is: ");
    for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		fprintf(out,"%x", (unsigned char) B_ptr->header->previousHash[n]);
	}
    printf(out, "\n");
    fprintf(out, "root hash is: ");
    for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		fprintf(out,"%x", (unsigned char) B_ptr->header->rootHash[n]);
	}
    fprintf(out, "\n%d\n", B_ptr->header->timestamp);
    fprintf(out, "%f\n", B_ptr->header->target);
    fprintf(out, "%d\n", B_ptr->header->nonce);
    fprintf(out, "\nEND HEADER\n");
    print_merkle_tree(B_ptr->rootHash, ID_K, out);
    fprintf(out, "\nEND BLOCK");
}