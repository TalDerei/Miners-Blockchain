#include "block.h"
#include "merkleTree.h"
#include "sha256.h"
#include "time.h"
#include "hash.h"
#include <stdint.h>

//create block consisting of block header and transaction array
Block *initialize_block(InternalNode *Treeroot, int *pointerToZero) {
    Header *header = (Header *)malloc(sizeof (header));
    initialize_header(header, Treeroot, pointerToZero);
    print_merkle_tree(Treeroot, 1);
    return header; 
}

void initialize_header(Header *header, InternalNode *Treeroot, int *pointerToZero) {
    //previousHash initially pointing to 0
    header->previousHash = pointerToZero;
    printf("header is fine! previous hash works %s\n", header->previousHash);
    printf("\n-------Treeroot is (within initialize_header): ----\n");
        for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
            printf("%x", (unsigned char) Treeroot->hash[n]);
        }
    printf("\n");
    unsigned char *temp = hash(Treeroot->hash);
    for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
       		printf("%x", temp[n]);
	} 
    strncpy(header->rootHash, temp , SHA256_BLOCK_SIZE);//SOMETHING is preventing the acess to temp or header->rootHash. Need debug
    printf("\nhello NOOBS!!!!!!!!!!!!!!!!");

    //timestamp
    header->timestamp = timestamp();
    //nonce with 50% difficulty target
    
    generate_nonce(header, Treeroot);
    //printf("nonce for first block is: %s\n", header->nonce);

    //targete value
    header->target = 0.5;
}

Block *create_block(InternalNode *Treeroot, Block *prevBlock) {
    Header *header = (Header *)malloc(sizeof (header));
    populate_header(header, Treeroot, prevBlock);
    print_merkle_tree(Treeroot, 1);
}

//populate the header with 5-elements
void populate_header(Header *header, InternalNode *Treeroot, Block *prevBlock) {
    //hash of the previous root hash of the previous block
    strncpy(header->previousHash, hash(prevBlock->rootHash), SHA256_BLOCK_SIZE);
    //hash of the root of the current block 
    strncpy(header->rootHash, hash(Treeroot->hash), SHA256_BLOCK_SIZE);
    //timestamp
    header->timestamp = timestamp();
    //nonce with 50% difficulty target
    generate_nonce(header, Treeroot);
    //targete value
    header->target = 0.5;
}

//return unix-time in seconds
int *timestamp() {
    time_t seconds;
    seconds = time(NULL);
    if (seconds == -1) {
        printf("time error");
        exit(0);
    }
    return seconds;
}

void generate_nonce(Header* header, InternalNode *Treeroot) {
    while (1) {
        srand(time(NULL)); //seed rand() 
        header->nonce = rand(); //turn into MACRO
        printf("\n nonce is: %d\n", header->nonce);
        unsigned int temp = header->nonce; //&temp or *temp = &(header->nonce) in order to make it pass by reference because memcpy expects a pointer
        unsigned char tempstr[SHA256_BLOCK_SIZE + sizeof(unsigned int)];//36 char[] (nonce + treeroot->hash)
        memcpy(tempstr, &temp, sizeof(unsigned int));
        memcpy(tempstr + sizeof(unsigned int), Treeroot->hash, SHA256_BLOCK_SIZE);//use memcpy instead to avoid /0 problem
        //strcat -- 0000\0 00000000000000000000000000000000\0 and will cut off after \0, use memcpy with pointer aritmetic to concatnate buffers
        printf("\n!!!!!!!!!!!!!!!tempstr is:");
        for( int i =0; i < sizeof(tempstr) / (sizeof(unsigned char)); ++i){ //unsigned char shoul dbe 1 but it can vary
            printf("%x\n", tempstr[i]); //doesn't have null terminators anymore
        }
        for (int n = 0; n < SHA256_BLOCK_SIZE; ++n) {
            printf("%x", (unsigned char) Treeroot->hash[n]);
        }
        printf("\n");
        unsigned char *hashResult = hash(tempstr);
        if (hashResult[0] <= 0x7f) { //turn into MACRO, 0x7f -- 01111111
            return;
        }
    }
} 

//PLEASE IMPLEMENT GROUP MEMBERS 
// print_block(block, count, fileNames) {
//     printf("----PRINT BLOCK----");
//     for(int i = 0; i < count; i++){
//         printf("%x",(unsigned char *) block[i];
//     }
// }   