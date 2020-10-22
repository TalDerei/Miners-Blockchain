/* IMPLEMENTATION FOR SERIALIZING BLOCKCHAIN TO FILE */

#include "serialize.h"

/* serialize_first_block serializes the first block from memory to disk (header contents and merkle tree) */
void serialize_first_blockchain(Block *block, FILE *write_blockchain){
    /* write data in binary representation to disk */
    Fwrite(block->header->previousHash, sizeof(unsigned char), 1, write_blockchain);
    Fwrite(block->header->rootHash, sizeof(unsigned char), 32, write_blockchain);
    Fwrite(&(block->header->timestamp), sizeof(unsigned int), 1, write_blockchain);
    Fwrite(&(block->header->target), sizeof(double), 1, write_blockchain);
    Fwrite(&(block->header->nonce), sizeof(unsigned int), 1, write_blockchain);

    print_merkle_tree(block->rootHash->hash, 1, write_blockchain);
}

/* rebuild_first_block rebuilds the first block in a linked-list after reading the data from disk */
void rebuild_first_block(FILE *output_blockchain, Block2 *block){
    unsigned char buffer_previousHash[1];
    unsigned char buffer_rootHash[32];
    int buffer_timestamp[1];
    double buffer_target[1];
    unsigned int buffer_nonce[1];

    /* fread to read back in the contents stored in disk and store in respective buffer */
    fread(buffer_previousHash, sizeof(unsigned char), 1, output_blockchain);
    fread(buffer_rootHash, sizeof(unsigned char), sizeof(buffer_rootHash), output_blockchain);
    fread(buffer_timestamp, sizeof(int), 1, output_blockchain);
    fread(buffer_target, sizeof(double), 1, output_blockchain);
    fread(buffer_nonce, sizeof(unsigned int), 1, output_blockchain);

    /* rebuild block from disk and store in Block2 structure */
    memcpy(block->previousHash, buffer_previousHash, 1);
    memcpy(block->rootHashHeader, buffer_rootHash, SHA256_BLOCK_SIZE);
    block->timestamp = buffer_timestamp[0];
    block->target = buffer_target[0];
    memcpy(block->rootHash, buffer_rootHash, SHA256_BLOCK_SIZE);
    block->nonce = buffer_nonce[0];
}

/* serialize_blockchain serializes block[i] (not the first block) from memory to disk (header contents and merkle tree) */
void serialize_blockchain(Block *block, FILE *write_blockchain) {
    Fwrite(block->header->previousHash, sizeof(unsigned char), 32, write_blockchain);
    Fwrite(block->header->rootHash, sizeof(unsigned char), 32, write_blockchain);
    Fwrite(&(block->header->timestamp), sizeof(unsigned int), 1, write_blockchain);
    Fwrite(&(block->header->target), sizeof(double), 1, write_blockchain);
    Fwrite(&(block->header->nonce), sizeof(unsigned int), 1, write_blockchain);

    print_merkle_tree(block->rootHash->hash, 1, write_blockchain);
} 

/* rebuild_first_block rebuilds the block[i] (not the first block) in a linked-list after reading the data from disk */
void rebuild_block(FILE *output_blockchain, Block2 *block) {
    unsigned char buffer_previousHash[32];
    unsigned char buffer_rootHash[32];
    int buffer_timestamp[1];
    double buffer_target[1];
    unsigned int buffer_nonce[1];

    /* fread to read back in the contents stored in disk and store in respective buffer */
    fread(buffer_previousHash, sizeof(unsigned char), SHA256_BLOCK_SIZE, output_blockchain);
    fread(buffer_rootHash, sizeof(unsigned char), sizeof(buffer_rootHash), output_blockchain);
    fread(buffer_timestamp, sizeof(int), 1, output_blockchain);
    fread(buffer_target, sizeof(double), 1, output_blockchain);
    fread(buffer_nonce, sizeof(unsigned int), 1, output_blockchain);

    /* rebuild block from disk and store in Block2 structure */
    memcpy(block->previousHash, buffer_previousHash, SHA256_BLOCK_SIZE);
    memcpy(block->rootHashHeader, buffer_rootHash, SHA256_BLOCK_SIZE);
    block->timestamp = buffer_timestamp[0];
    block->target = buffer_target[0];
    memcpy(block->rootHash, buffer_rootHash, SHA256_BLOCK_SIZE);
    block->nonce = buffer_nonce[0];
}