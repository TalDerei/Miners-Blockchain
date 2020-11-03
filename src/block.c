/* IMPLEMENTATION OF BLOCKS IN BLOCKCHAIN (LINKEDLIST) */

#include "printBlock.h"
#include "stdbool.h"
#define BlockSize (3*SHA256_BLOCK_SIZE + 4*sizeof(int))

//create block consisting of block header and transaction array
void initialize_block(Block *block, InternalNode *Treeroot, unsigned char *pointerToZero, FILE *output_block, FILE *output_blockchain) {
    //printf("******initialize_block********\n");
    //printf("pointerToZero is: %c\n", *pointerToZero); 
    initialize_header(block, Treeroot, pointerToZero);
    block->rootHash = Treeroot; //pointer to full merkletree
    print_first_block(block, 1, output_block); //print block
    fclose(output_block);
    
    //output_blockchain for serializing block 
    serialize_first_blockchain(block, output_blockchain); 
}

void initialize_header(Block *block, InternalNode *Treeroot, unsigned char *pointerToZero) {
    //previousHash initially pointing to 0
    //printf("******initialize_header********\n");
    //printf("pointerToZero is: %c\n", *pointerToZero);
    block->header->previousHash = pointerToZero; //wtf -- why can't equal unsigned char * to unsigned char *???
    // printf("header is fine! previous hash works %s\n", block->header->previousHash);
    // printf("\n-------Treeroot is (within initialize_header): ----\n");

    // printf("\n-------FUCK!!!!!!!!!!!!!!!!!!!!!!!!!: previousHash---------\n");
    //     for (int n = 0; n < 1; n++) {
    //         printf("%x", (unsigned char) block->header->previousHash[n]);
    // }
    // exit(0);

    unsigned char *temp = Treeroot->hash;
    // for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
    //    		printf("%x", temp[n]);
	// }
    strncpy(block->header->rootHash, temp , SHA256_BLOCK_SIZE);//SOMETHING is preventing the acess to temp or header->rootHash. Need debug
    //printf("\nhello NOOBS!!!!!!!!!!!!!!!!");

    //timestamp
    block->header->timestamp = timestamp();
    //nonce with 50% difficulty target
    
    generate_nonce(block->header, Treeroot);
    //printf("nonce for first block is: %s\n", header->nonce);

    //targete value
    block->header->target = 0.5;
}

void create_block(Block *block, InternalNode *Treeroot, Block *prevBlock, FILE *output_block, FILE *output_blockchain, unsigned char *pointerToZero) {
    //printf("******create_block********\n");
    populate_header(block, Treeroot, prevBlock, pointerToZero);
    block->rootHash = Treeroot;    
    print_block(block, 1, output_block); //print block
    fclose(output_block);
    //don't need to malloc header --> malloced block already
    //turned function prototypes in void and passing in block directly
    //output_blockchain for serializing block 
    serialize_blockchain(block, output_blockchain); 
}

//populate the header with 5-elements
void populate_header(Block *block, InternalNode *Treeroot, Block *prevBlock, unsigned char *pointerToZero) {
    //hash of the previous root hash of the previous block
    //printf("******populate_header********\n");

        
    unsigned char *BlockConcat = malloc(BlockSize);
    printf("BlockSize is: %d\n", BlockSize);
    //Don't know if this needed.....In some previous case, we saw some trash when we malloc
    for(int i = 0; i < BlockSize; i++){
        BlockConcat[i] = '\0';
    }

    bool isSecondBlock = prevBlock->header->previousHash == pointerToZero;
    if(isSecondBlock){
        memcpy(BlockConcat, prevBlock->header->previousHash,1);
        memcpy(BlockConcat+1, prevBlock->header->rootHash,SHA256_BLOCK_SIZE);
        memcpy(BlockConcat+1+SHA256_BLOCK_SIZE, &(prevBlock->header->timestamp),sizeof(int));
        memcpy(BlockConcat+1+SHA256_BLOCK_SIZE + sizeof(int), &(prevBlock->header->target),sizeof(double));
        memcpy(BlockConcat+1+SHA256_BLOCK_SIZE + sizeof(int) + sizeof(double), &(prevBlock->header->nonce),sizeof(unsigned int));
        memcpy(BlockConcat+1+SHA256_BLOCK_SIZE + 2*sizeof(int) + sizeof(double), prevBlock->rootHash,SHA256_BLOCK_SIZE);
    }else{
        memcpy(BlockConcat, prevBlock->header->previousHash,SHA256_BLOCK_SIZE);
        memcpy(BlockConcat+SHA256_BLOCK_SIZE, prevBlock->header->rootHash,SHA256_BLOCK_SIZE);
        memcpy(BlockConcat+2*SHA256_BLOCK_SIZE, &(prevBlock->header->timestamp),sizeof(int));
        memcpy(BlockConcat+2*SHA256_BLOCK_SIZE + sizeof(int), &(prevBlock->header->target),sizeof(double));
        memcpy(BlockConcat+2*SHA256_BLOCK_SIZE + sizeof(int) + sizeof(double), &(prevBlock->header->nonce),sizeof(unsigned int));
        memcpy(BlockConcat+2*SHA256_BLOCK_SIZE + 2*sizeof(int) + sizeof(double), prevBlock->rootHash,SHA256_BLOCK_SIZE);

    }
    // printf("Content in BlockConcat is: \n");
    // for(int i = 0; i < BlockSize; i++){
    //     printf("%x", BlockConcat[i]);
    // }
    // printf("\n");
    block->header->previousHash = hash112(BlockConcat);
    // printf("\n-------FUCK!!!!!!!!!!!!!!!!!!!!!!!!!: previousHash---------\n");
    //     for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
    //         printf("%x", (unsigned char) block->header->previousHash[n]);
    // }
    // exit(0);
    //hash of the root of the current block 
    strncpy(block->header->rootHash, hash(Treeroot->hash), SHA256_BLOCK_SIZE);
    //timestamp
    block->header->timestamp = timestamp();
    //nonce with 50% difficulty target
    generate_nonce(block->header, Treeroot);
    //target value
    block->header->target = 0.5;
}

//return unix-time in seconds
int *timestamp() {
    time_t seconds;
    seconds = time(NULL);
    if (seconds == -1) {
        printf("time error");
        exit(0); //neccessary exit(0);
    }
    return seconds;
}

void generate_nonce(Header* header, InternalNode *Treeroot) {
    printf("\n******generate_nonce********\n");
    srand(time(0)); //seed rand() 
    while (1) {
        header->nonce = rand(); //turn into MACRO
        unsigned int temp = header->nonce; //&temp or *temp = &(header->nonce) in order to make it pass by reference because memcpy expects a pointer
        unsigned char tempstr[SHA256_BLOCK_SIZE + sizeof(unsigned int)];//36 char[] (nonce + treeroot->hash)
        memcpy(tempstr, &temp, sizeof(unsigned int));
        memcpy(tempstr + sizeof(unsigned int), Treeroot->hash, SHA256_BLOCK_SIZE);//use memcpy instead to avoid /0 problem
        //strcat -- 0000\0 00000000000000000000000000000000\0 and will cut off after \0, use memcpy with pointer aritmetic to concatnate buffers
        // printf("\n!!!!!!!!!!!!!!!tempstr is:");
        // for( int i =0; i < sizeof(tempstr) / (sizeof(unsigned char)); ++i){ //unsigned char shoul dbe 1 but it can vary
        //     printf("%x", tempstr[i]); //doesn't have null terminators anymore
        // }
        // printf("\n");
        // for (int n = 0; n < SHA256_BLOCK_SIZE; ++n) {
        //     printf("%x", (unsigned char) Treeroot->hash[n]);
        // }
        // printf("\n");
        unsigned char *hashResult = hash(tempstr);
        if (hashResult[0] <= 0x7f) { //turn into MACRO, 0x7f -- 01111111
            printf("nonce is: %u\n", header->nonce);
            return;
        }
    }
} 
