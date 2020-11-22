/* IMPLEMENTATION OF BLOCKS IN BLOCKCHAIN (LINKEDLIST) */

#include "printBlock.h"
#include "stdbool.h"
#define BlockSize (3 * SHA256_BLOCK_SIZE + 4 * sizeof(int))

/* initialize_block creates the first block in the blockchain consisting of block header and transaction array */
void initialize_block(Block *block, InternalNode *Treeroot, unsigned char *pointerToZero, FILE *output_block, FILE *output_blockchain) {
    initialize_header(block, Treeroot, pointerToZero);
    block->rootHash = Treeroot; /* pointer to tree of merkle tree */

    /* print and serialize the first block to disk in the blockchain */
    print_first_block(block, 1, output_block); 
    fclose(output_block);
    serialize_first_blockchain(block, output_blockchain); 
}

/* initialize_header initializes and populating the header contents associated with the first block */
void initialize_header(Block *block, InternalNode *Treeroot, unsigned char *pointerToZero) {
    block->header->previousHash = pointerToZero; 
    unsigned char *temp = Treeroot->hash;
    strncpy(block->header->rootHash, temp , SHA256_BLOCK_SIZE);
    block->header->timestamp = timestamp();
    generate_nonce(block->header, Treeroot);
    block->header->target = 0.5;
    /* populate header with previous hash of previous block, root hash of current block, nonce, 50% difficulty target, and timestamp */
}

/* create_block creates and appends additional blocks to the first block in the blockchain */
void create_block(Block *block, InternalNode *Treeroot, Block *prevBlock, FILE *output_block, FILE *output_blockchain, unsigned char *pointerToZero) {
    populate_header(block, Treeroot, prevBlock, pointerToZero);
    block->rootHash = Treeroot;    

    /* print and serialize subsequent blocks to disk in the blockchain */
    print_block(block, 1, output_block); 
    fclose(output_block);
    serialize_blockchain(block, output_blockchain); 
}

/* initialize_header initializes and populating the header contents for additional blocks */
void populate_header(Block *block, InternalNode *Treeroot, Block *prevBlock, unsigned char *pointerToZero) {
    unsigned char *BlockConcat = malloc(BlockSize);
    for(int i = 0; i < BlockSize; i++){
        BlockConcat[i] = '\0';
    }

    /* populating the header of the first block and subsequent blocks */
    bool isSecondBlock = prevBlock->header->previousHash == pointerToZero;
    if(isSecondBlock){
        memcpy(BlockConcat, prevBlock->header->previousHash,1);
        memcpy(BlockConcat + 1, prevBlock->header->rootHash,SHA256_BLOCK_SIZE);
        memcpy(BlockConcat + 1 + SHA256_BLOCK_SIZE, &(prevBlock->header->timestamp),sizeof(int));
        memcpy(BlockConcat + 1 + SHA256_BLOCK_SIZE + sizeof(int), &(prevBlock->header->target),sizeof(double));
        memcpy(BlockConcat + 1 + SHA256_BLOCK_SIZE + sizeof(int) + sizeof(double), &(prevBlock->header->nonce),sizeof(unsigned int));
        memcpy(BlockConcat + 1 + SHA256_BLOCK_SIZE + 2 * sizeof(int) + sizeof(double), prevBlock->rootHash,SHA256_BLOCK_SIZE);
    }else{
        memcpy(BlockConcat, prevBlock->header->previousHash,SHA256_BLOCK_SIZE);
        memcpy(BlockConcat + SHA256_BLOCK_SIZE, prevBlock->header->rootHash,SHA256_BLOCK_SIZE);
        memcpy(BlockConcat + 2 * SHA256_BLOCK_SIZE, &(prevBlock->header->timestamp),sizeof(int));
        memcpy(BlockConcat + 2 * SHA256_BLOCK_SIZE + sizeof(int), &(prevBlock->header->target),sizeof(double));
        memcpy(BlockConcat + 2 * SHA256_BLOCK_SIZE + sizeof(int) + sizeof(double), &(prevBlock->header->nonce),sizeof(unsigned int));
        memcpy(BlockConcat + 2 * SHA256_BLOCK_SIZE + 2 * sizeof(int) + sizeof(double), prevBlock->rootHash,SHA256_BLOCK_SIZE);

    }

    /* hash of the concatenation of the header contents */
    block->header->previousHash = hash112(BlockConcat);

    /* other header contents */
    strncpy(block->header->rootHash, hash(Treeroot->hash), SHA256_BLOCK_SIZE);
    block->header->timestamp = timestamp();
    generate_nonce(block->header, Treeroot);
    block->header->target = 0.5;
}

/* unix-time in seconds */
int *timestamp() {
    time_t seconds;
    seconds = time(NULL);
    if (seconds == -1) {
        printf("time error");
        exit(0); 
    }
    return seconds;
}

/* generate_nonce generates a random nonce such that the concatenation of (nonce + root hash of merkle tree < target) */
void generate_nonce(Header* header, InternalNode *Treeroot) {
    srand(time(0)); /* seed */
    while (1) {
        header->nonce = rand(); 
        unsigned int temp = header->nonce; 
        unsigned char tempstr[SHA256_BLOCK_SIZE + sizeof(unsigned int)];
        memcpy(tempstr, &temp, sizeof(unsigned int));
        memcpy(tempstr + sizeof(unsigned int), Treeroot->hash, SHA256_BLOCK_SIZE);
        unsigned char *hashResult = hash(tempstr);
        if (hashResult[0] <= 0x7f) { 
            return;
        }
    }
} 
