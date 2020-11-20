/* IMPLEMENTATION FOR PRINTING BLOCK TO FILE */

#include "printBlock.h"

/* printing out the first block */
void print_first_block(Block *B_ptr, int ID_K, FILE *out){
    fprintf(out, "BEGIN BLOCK\n");
    fprintf(out, "BEGIN HEADER\n");
    fprintf(out, "previous hash is: %s", B_ptr->header->previousHash);
    fprintf(out, "\nroot hash is: ");
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        if ((unsigned char)B_ptr->header->rootHash[i] <= 0x0f) {
            fprintf(out, "0%x", (unsigned char)B_ptr->header->rootHash[i]);
        } else{
            fprintf(out, "%x", (unsigned char)B_ptr->header->rootHash[i]);
        }
    }
    fprintf(out, "\ntimestamp is: %d\n", B_ptr->header->timestamp);
    fprintf(out, "target value is: %f\n", B_ptr->header->target);
    fprintf(out, "nonce is: %d\n", B_ptr->header->nonce);
    fprintf(out, "END HEADER\n");
    print_merkle_tree(B_ptr->rootHash, ID_K, out);
    fprintf(out, "END BLOCK");
}

/* printing out additional blocks */
void print_block(Block* B_ptr, int ID_K, FILE* out){
    fprintf(out, "BEGIN BLOCK\n");
    fprintf(out, "BEGIN HEADER\n");
    fprintf(out, "previous hash is: ");
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        if ((unsigned char)B_ptr->header->previousHash[i] <= 0x0f) {
            fprintf(out, "0%x", (unsigned char)B_ptr->header->previousHash[i]);
        } else{
            fprintf(out, "%x", (unsigned char)B_ptr->header->previousHash[i]);
        }
    }
    fprintf(out, "\n");
    fprintf(out, "root hash is: ");
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        if ((unsigned char)B_ptr->header->rootHash[i] <= 0x0f) {
            fprintf(out, "0%x", (unsigned char)B_ptr->header->rootHash[i]);
        } else{
            fprintf(out, "%x", (unsigned char)B_ptr->header->rootHash[i]);
        }
    }
    fprintf(out, "\ntimestamp is: %d\n", B_ptr->header->timestamp);
    fprintf(out, "target value is: %f\n", B_ptr->header->target);
    fprintf(out, "nonce is: %d\n", B_ptr->header->nonce);
    fprintf(out, "END HEADER\n");
    print_merkle_tree(B_ptr->rootHash, ID_K, out);
    fprintf(out, "END BLOCK");
}

//make them the same