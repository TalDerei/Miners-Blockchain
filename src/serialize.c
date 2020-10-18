/* IMPLEMENTATION FOR SERIALIZING BLOCKCHAIN TO FILE */

#include "serialize.h"

void serialize_blockchain(Block *block, FILE *write_blockchain) {

    printf("\n-------inside serialize is: ----\n");
        for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
            printf("%x", (unsigned char) block->header->rootHash[n]);
    }
    printf("\n");
    // printf("\n-------inside serialize is: ----\n");
    //     for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
    //         printf("%x", (unsigned char) block->rootHash->hash[n]);
    // }
    
    exit(0);
    Fwrite(block->header->rootHash, sizeof(unsigned char), sizeof(block->header->rootHash), write_blockchain);
    exit(0);
    
        // Fwrite(write_blockchain, "header: \n");
        // Fwrite(write_blockchain, "PreviousHash: %s, RootHash: %s", blockchain[count]->header->previousHash, blockchain[count]->header->rootHash);
        // Fwrite(write_blockchain, "");
        // Fwrite(write_blockchain, "TimeStamp: %d, Target: %s, nonce: %d", blockchain[count]->header->timestamp, blockchain[count]->header->target, blockchain[count]->header->nonce);
        // Fwrite(write_blockchain, "");
        // Fwrite(write_blockchain, "content: \n");
} 

//void rebuild_block(char outputFile[], int *block_count, Block2 **block2); 