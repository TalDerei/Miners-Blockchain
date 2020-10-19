/* IMPLEMENTATION FOR SERIALIZING BLOCKCHAIN TO FILE */

#include "serialize.h"

void serialize_blockchain(Block *block, FILE *write_blockchain) {
    //serialize block's merkleTree to file
    printf("\n-------HEADER: previousHash---------\n");
    Fwrite(block->header->previousHash, sizeof(unsigned char), sizeof(block->header->previousHash), write_blockchain);
    printf("\n-------HEADER: rootHash---------\n");
    Fwrite(block->header->rootHash, sizeof(unsigned char), sizeof(block->header->rootHash), write_blockchain);
    printf("\n-------HEADER: timestamp---------\n");
    Fwrite(&(block->header->timestamp), sizeof(unsigned int), sizeof(block->header->timestamp), write_blockchain);
    printf("\n-------HEADER: target---------\n");
    Fwrite(&(block->header->target), sizeof(double), sizeof(block->header->target), write_blockchain);
    printf("\n-------HEADER: nonce---------\n");
    Fwrite(&(block->header->nonce), sizeof(unsigned int), sizeof(block->header->nonce), write_blockchain);

    //serialize block's header contents to file
    printf("\n-------MERKLETREE: hash of Merkle Tree---------\n");  
    print_merkle_tree(block->rootHash->hash, 1, write_blockchain);
    fclose(write_blockchain);
    fopen(write_blockchain, "rb");
    rebuild_block(write_blockchain, block);
} 

void rebuild_block(FILE *output_blockchain, Block *block) {
    unsigned char buffer_previousHash[sizeof(unsigned char)];
    unsigned char buffer_rootHash[32];
    long long buffer_timestamp[1];
    long long buffer_target[1];
    long long buffer_nonce[1];

    fread(buffer_previousHash, sizeof(unsigned char), 1, output_blockchain);
    printf("previousHash is: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    for(int i = 0; i < sizeof(buffer_previousHash); i ++){
        printf("%u\n",(unsigned char)buffer_previousHash[i]); 
    }

    printf("\n");
    printf("rootHash is: ___________________________________\n");
    fread(buffer_rootHash, sizeof(char), 1, output_blockchain);
    for(int i=0 ; i < sizeof(buffer_rootHash); i ++){
        printf("%u\n",(unsigned char)buffer_rootHash[i]);
    }

    printf("\n");
    printf("timestamp is: #################################\n");
    fread(buffer_timestamp, sizeof(int), 1, output_blockchain);
    printf("%d\n",(int)buffer_timestamp[0]); 
    
    printf("\n");
    printf("target is: ***********************************\n");
    fread(buffer_target, sizeof(double), 1, output_blockchain);
    printf("%f\n",(double)buffer_target[0]); 

    printf("\n");
    printf("nonce is: $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    fread(buffer_nonce, sizeof(int), 1, output_blockchain);
    printf("%u\n",(unsigned int)buffer2_nonce[0]); 
    
    exit(0);
    // fclose(output_blockchain);
}