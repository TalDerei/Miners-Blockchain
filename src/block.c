#include "block.h"
#include "merkleTree.h"
#include "sha256.h"
#include "time.h"
#include <stdint.h>

//create block consisting of block header and transaction array
Block *initialize_block(InternalNode *Treeroot, int *pointerToZero) {
    Header *header = (Header *)malloc(sizeof (header));
    initialize_header(header, Treeroot, pointerToZero);

    print_merkle_tree(Treeroot, 1);
}

void initialize_header(Header *header, InternalNode *Treeroot, int *pointerToZero) {
    //hash of the previous root hash of the previous block
    header->previousHash = pointerToZero;
    printf("header is fine! previouhash works %s\n", header->previousHash);
    //hash of the root of the current block 
    printf("\n-------root is: ----\n");
        for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
            printf("%x", (unsigned char) Treeroot->hash[n]);
        }
    strncpy(header->rootHash, hash(Treeroot->hash), SHA256_BLOCK_SIZE);
    printf("hello motherfuckers!!!!!!!!!!!!!!!!");
    exit(0);
    //timestamp
    header->timestamp = timestamp();
    //nonce with 50% difficulty target
    unsigned char *nonce = (unsigned char *)malloc(8);
    generate_nonce(nonce, Treeroot);
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
    unsigned char *nonce = (unsigned char *)malloc(8);
    generate_nonce(nonce, Treeroot);
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

void generate_nonce(unsigned char * nonce, InternalNode *Treeroot) {
    while (1) {
        unsigned char *nonce = rand(0); 
        unsigned char *hashResult = hash(strcat(nonce, Treeroot));
        if (hashResult[0] <= 0x7f) { 
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