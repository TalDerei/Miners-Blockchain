/* IMPLEMENTATION FOR SERIALIZING BLOCKCHAIN TO FILE */

#include "serialize.h"

void serialize_blockchain(Block **blockchain, FILE *write_blockchain, int count) {
    printf("count passed into serialize_blockchain is: %d\n", count);

    printf("roothash is: %s\n", blockchain[count]->header->rootHash);
    exit(0);
    Fwrite(blockchain[count]->header->rootHash, sizeof(unsigned char), sizeof(blockchain[count]->header->rootHash), write_blockchain);
    exit(0);
    
    size_t written = 0;
    while (count != 0) {
        Fwrite(write_blockchain, "header: \n");
        Fwrite(write_blockchain, "PreviousHash: %s, RootHash: %s", blockchain[count]->header->previousHash, blockchain[count]->header->rootHash);
        Fwrite(write_blockchain, "");
        Fwrite(write_blockchain, "TimeStamp: %d, Target: %s, nonce: %d", blockchain[count]->header->timestamp, blockchain[count]->header->target, blockchain[count]->header->nonce);
        Fwrite(write_blockchain, "");
        Fwrite(write_blockchain, "content: \n");
        count--;
    }
} 

//void rebuild_block(char outputFile[], int *block_count, Block2 **block2); 