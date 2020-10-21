/* IMPLEMENTATION FOR SERIALIZING BLOCKCHAIN TO FILE */

#include "serialize.h"

void serialize_first_blockchain(Block *block, FILE *write_blockchain){
    //printf("\n$$$$$$$$$$$$$$$$$$$$ serialize_first_blockchain $$$$$$$$$$$$$$$$$$$$$$$$\n");
    //serialize block's merkleTree to file
    //printf("\n-------HEADER: previousHash---------\n");
    Fwrite(block->header->previousHash, sizeof(unsigned char), 1, write_blockchain);
    //printf("\n-------HEADER: rootHash---------\n");
    Fwrite(block->header->rootHash, sizeof(unsigned char), 32, write_blockchain);
    //printf("\n-------HEADER: timestamp---------\n");
    Fwrite(&(block->header->timestamp), sizeof(unsigned int), 1, write_blockchain);
    //printf("\n-------HEADER: target---------\n");
    Fwrite(&(block->header->target), sizeof(double), 1, write_blockchain);
    //printf("\n-------HEADER: nonce---------\n");
    Fwrite(&(block->header->nonce), sizeof(unsigned int), 1, write_blockchain);

    //serialize block's header contents to file
    //printf("\n-------MERKLETREE: hash of Merkle Tree---------\n");  
    //print_merkle_tree(block->rootHash->hash, 1, write_blockchain);

    // printf("PrevHash is: %c\n",*(block->header->previousHash));
    // printf("rootHash is: \n");
    // for(int i=0 ; i < SHA256_BLOCK_SIZE; i++){
    //     printf("%x",(unsigned char)block->header->rootHash[i]);
    // }
    // printf("\nserialize_blockchain: Nonce is: %u", block->header->nonce);
    // printf("\n");
}

void rebuild_first_block(FILE *output_blockchain, Block2 *block){
    printf("\n\n\n$$$$$$$$$$$$$$$$$$$$ rebuild_first_block $$$$$$$$$$$$$$$$$$$$$$$$\n");
    unsigned char buffer_previousHash[1];
    unsigned char buffer_rootHash[32];
    int buffer_timestamp[1];
    double buffer_target[1];
    unsigned int buffer_nonce[1];

    fread(buffer_previousHash, sizeof(unsigned char), 1, output_blockchain);
    printf("previousHash is: \n");
    for(int i = 0; i < 1; i ++){
        printf("%c",buffer_previousHash[i]); 
    }

    printf("\n");
    printf("rootHash is: \n");
    fread(buffer_rootHash, sizeof(unsigned char), sizeof(buffer_rootHash), output_blockchain);
    for(int i=0 ; i < sizeof(buffer_rootHash); i ++){
        printf("%x",buffer_rootHash[i]);
    }
    printf("\n");
    
    printf("timestamp is: \n");
    fread(buffer_timestamp, sizeof(int), 1, output_blockchain);
    printf("%d",(int)buffer_timestamp[0]); 
    
    printf("\n");
    printf("target is: \n");
    fread(buffer_target, sizeof(double), 1, output_blockchain);
    printf("%f",buffer_target[0]); 

    printf("\n");
    printf("nonce is: \n");
    fread(buffer_nonce, sizeof(unsigned int), 1, output_blockchain);
    printf("%u",buffer_nonce[0]); 
    printf("\n");

    //rebuild block from buffer
    memcpy(block->previousHash, buffer_previousHash, 1);
    memcpy(block->rootHashHeader, buffer_rootHash, SHA256_BLOCK_SIZE);
    block->timestamp = buffer_timestamp[0];
    block->target = buffer_target[0];
    memcpy(block->rootHash, buffer_rootHash, SHA256_BLOCK_SIZE);
    block->nonce = buffer_nonce[0];
}

void serialize_blockchain(Block *block, FILE *write_blockchain) {
    //printf("\n$$$$$$$$$$$$$$$$$$$$ serialize_blockchain $$$$$$$$$$$$$$$$$$$$$$$$\n");
    //serialize block's merkleTree to file
    //printf("\n-------HEADER: previousHash---------\n");
    Fwrite(block->header->previousHash, sizeof(unsigned char), 32, write_blockchain);
    //printf("\n-------HEADER: rootHash---------\n");
    Fwrite(block->header->rootHash, sizeof(unsigned char), 32, write_blockchain);
    //printf("\n-------HEADER: timestamp---------\n");
    Fwrite(&(block->header->timestamp), sizeof(unsigned int), 1, write_blockchain);
    //printf("\n-------HEADER: target---------\n");
    Fwrite(&(block->header->target), sizeof(double), 1, write_blockchain);
    //printf("\n-------HEADER: nonce---------\n");
    Fwrite(&(block->header->nonce), sizeof(unsigned int), 1, write_blockchain);

    //serialize block's header contents to file
    //printf("\n-------MERKLETREE: hash of Merkle Tree---------\n");  
    //print_merkle_tree(block->rootHash->hash, 1, write_blockchain);

    // printf("PrevHash is: %c\n",*(block->header->previousHash));
    // printf("rootHash is: \n");
    // for(int i=0 ; i < SHA256_BLOCK_SIZE; i++){
    //     printf("%x",(unsigned char)block->header->rootHash[i]);
    // }
    // printf("\n serialize_blockchain: Nonce is: %u", block->header->nonce);
    // printf("\n");
} 

void rebuild_block(FILE *output_blockchain, Block2 *block) {
    printf("\n\n\n$$$$$$$$$$$$$$$$$$$$ rebuild_block $$$$$$$$$$$$$$$$$$$$$$$$\n");
    unsigned char buffer_previousHash[32];
    unsigned char buffer_rootHash[32];
    int buffer_timestamp[1];
    double buffer_target[1];
    unsigned int buffer_nonce[1];

    fread(buffer_previousHash, sizeof(unsigned char), 32, output_blockchain);
    printf("previousHash is: \n");
    for(int i = 0; i < 32; i ++){
        printf("%x",buffer_previousHash[i]); 
    }

    printf("\n");
    printf("rootHash is: \n");
    fread(buffer_rootHash, sizeof(unsigned char), sizeof(buffer_rootHash), output_blockchain);
    for(int i=0 ; i < sizeof(buffer_rootHash); i ++){
        printf("%x",buffer_rootHash[i]);
    }
    printf("\n");
    
    printf("timestamp is: \n");
    fread(buffer_timestamp, sizeof(int), 1, output_blockchain);
    printf("%d",(int)buffer_timestamp[0]); 
    
    printf("\n");
    printf("target is: \n");
    fread(buffer_target, sizeof(double), 1, output_blockchain);
    printf("%f",buffer_target[0]); 

    printf("\n");
    printf("nonce is: \n");
    fread(buffer_nonce, sizeof(unsigned int), 1, output_blockchain);
    printf("%u",buffer_nonce[0]); 
    printf("\n");

    memcpy(block->previousHash, buffer_previousHash, SHA256_BLOCK_SIZE);
    memcpy(block->rootHashHeader, buffer_rootHash, SHA256_BLOCK_SIZE);
    block->timestamp = buffer_timestamp[0];
    block->target = buffer_target[0];
    memcpy(block->rootHash, buffer_rootHash, SHA256_BLOCK_SIZE);
    block->nonce = buffer_nonce[0];
}