/* CONTAINS ENTIRE CODEBASE */

/* ROOT -- MAIN.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merkleTree.h"
#include "block.h"
#include "printBlock.h"
#include "printMerkleTree.h"
#include "serialize.h"
#include "sha256.h"
#include "readFile.h"
#include "sort.h"
#include "hash.h"
#define BUFFER 100

int main(int argc, char *argv[]) {
    int count;
    //printf("input the number of files you want opened: "); 
	//scanf("%d",&count);	//make sure user can ONLY enter an int (not char) while handling any potential errors
    count = 2;
    //printf("number of files entered: %d\n", count); 

    char **fileNames = (char **)malloc(count * sizeof(FILE *));
    for(int i = 0; i < count; i++){
        fileNames[i] = (char *)malloc(BUFFER);
    }
    fileNames[0] = "input.txt";
    fileNames[1] = "input2.txt";
    
    // for (int i = 0; i < count; i ++){
    //     printf("enter the filename[%d]: ", i); 
    //     scanf("%s", fileNames[i]);
    //     int k = strlen(fileNames[i]);
    //     printf("Filename entered: %s", fileNames[i]);
    // }
    int * lineNum = malloc(count * sizeof(int));
    lineNum = GetLineNumbers(fileNames, count);
    // for(int i = 0 ; i < count; i++){
    //     printf("########lineNum at %d is: %d\n", i, lineNum[i]);
    // }

    char **arr = (char **)malloc(100* sizeof(char*));
    for (int i = 0; i < 100; i++)
    {
        arr[i] = malloc(100);
    }

    //output_blockchain = serialization of the blockchain
    char output_fileName[] = "output.blockchain.txt";
    FILE *output_blockchain = fopen(output_fileName,"wb"); //b = open file for writing in binary format

    char actualFileNameMerkleTree[count][255]; //array for storing file names for merkle trees
    char actualFileNameBlock[count][255]; //array for stotring file names for blocks
    for(int i = 0 ; i < count; i++){
        for( int j = 0; j < 266; j++){
            actualFileNameMerkleTree[i][j] = NULL;
            actualFileNameBlock[i][j] = NULL;
        }   
    }

    unsigned char *pointerToZero = (unsigned char *)malloc(sizeof(char));
    strncpy(pointerToZero, "0", 1);
    Block **block = (Block **)malloc(count * sizeof(Block *));
    LeafNode **leafNodes = (LeafNode **)malloc(count * sizeof(LeafNode *));
    InternalNode **internalNode = (InternalNode **)malloc(count *sizeof(InternalNode *));
    InternalNode **TreeRoot = (InternalNode **)malloc(count * sizeof(InternalNode *));
    for(int i = 0; i < count; i++){
        printf("\n\n\n");
        int fileNameCounter = strlen(fileNames[i]) - 4;
        //printf("fileNameCounter is %d\n", fileNameCounter);
        for(int j = 0; j < fileNameCounter; j++){
            actualFileNameMerkleTree[i][j] = fileNames[i][j];
        }
        strcat(actualFileNameMerkleTree[i], ".merkletree.txt");
        //printf("file name after strcat is: \n");
        //printf("%s\n", actualFileNameMerkleTree[i]);

        //output for printing merkle tree to file
        FILE *outputMerkleTree; 
        outputMerkleTree = fopen(actualFileNameMerkleTree[i],"w"); 

        for(int j = 0; j < fileNameCounter; j++){
            actualFileNameBlock[i][j] = fileNames[i][j];
        }
        strcat(actualFileNameBlock[i], ".block.txt");
        //printf("file name after strcat is: \n");
        //printf("%s\n", actualFileNameBlock[i]);
        
        // //outputBlock for printing block to file
        FILE *output_block; 
        output_block = fopen(actualFileNameBlock[i],"w");

        ReadOneFile(arr, fileNames[i]);
        leafNodes[i] = (LeafNode *)malloc(lineNum[i] * sizeof(LeafNode));
        block[i] = (Block *)malloc(sizeof(Block)); //mallocing for block (pointer to root and header) -- 16 Bytes
        block[i]->header = malloc(sizeof(Header)); //mallocing for header contents
        createLeafNodes(leafNodes[i], arr, lineNum[i]);
        // for (int k = 0; k < lineNum[i]; k++) {
        //     for (int j = 0; j < SHA256_BLOCK_SIZE; j++) {
        //         if ((unsigned char)leafNodes[i][k].hash[j] <= 0x0f) {
        //             printf("%x", (unsigned char) leafNodes[i][k].hash[j]);
        //         }else{
        //             printf("%x", (unsigned char) leafNodes[i][k].hash[j]);
        //         }
        //     }
        //     printf("\n");
        // }
        internalNode[i] = malloc(lineNum[i]*sizeof(InternalNode));
        convertLeaftoInternal(internalNode[i], leafNodes[i],lineNum[i]);
        TreeRoot[i] = malloc(sizeof(InternalNode));
        TreeRoot[i] = merkleTreeRoot(internalNode[i],lineNum[i]);
        printf("\n-------root is: ----\n");
        for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
            printf("%x", (unsigned char) TreeRoot[i]->hash[n]);
        }
        printf("\n");

		// FILE *output = fopen(strncat(output,".block.out", 1), "w");
    	print_merkle_tree(TreeRoot[i], 1, outputMerkleTree); //print merkle tree
        //print_block(block, 1, outputBlock); //print block

        //create individual blocks part of the blockchain 
        if(i != 0){
            //printf("CREATE BLOCK: \n");
            create_block(block[i], TreeRoot[i], block[i-1], output_block, output_blockchain, pointerToZero);
        }else{
            //printf("INITIALIZE BLOCK: \n");
            initialize_block(block[i], TreeRoot[i], pointerToZero, output_block, output_blockchain); //first block, previous block pointing to 0
        }
        
        //reset arr for re-use
        for(int i = 0 ; i < 100; i++){
            for( int j = 0; j < 100; j++){
                arr[i][j] = NULL;
            }
        }
    }

    fclose(output_blockchain);
    
    //inserting serialized data into blocks (Block2)
    FILE *write_blockchain2 = fopen(output_fileName, "rb");
    Block2 **block2= (Block **)malloc(count * sizeof(Block2 *));
    for( int i = 0; i < count ; i ++) {
        block2[i] = (Block2 *) malloc(sizeof(Block2));
        if(i == 0){
            rebuild_first_block(write_blockchain2, block2[i]);
        }else{
            rebuild_block(write_blockchain2, block2[i]);
        }
    }

    //validation starts here:
    
    //Fwrite binary-data blockchain to output file
    //1. need to read back in unsigned chars from file produced by serialize blockchain. 
    //2. link block (doubly linked list in read_block)
    //3. rebuild blockchain by initialization + populate blocks 
    //arugments: filename, pointer to block_counter, and array of blocks

    //rebuild_merkle_tree:
    //1. read in merkle Tree array representation
    //2. sort by ID
    //3. search for key-word leaf node
    //4. rebuild bottom-up
    //5. reassign pointers to children nodes 

    //Fclose(output);
    //free_merkle_tree(leafNodes);
    //print_block(block, count, fileNames);
}

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
    //print_block(block, 1, outputBlock); //print block
    //print_merkle_tree(Treeroot, 1, outputBlock); //print merkle tree
    
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
    //print_merkle_tree(Treeroot, 1, outputBlock); //print merkle tree
    //print_block(block, 1, outputBlock); //print block
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

/* HEADER FILE FOR BLOCK.C */

#ifndef BLOCK_DEF
#define BLOCK_DEF

#include "sha256.h"
#include "merkleTree.h"
//header struct
struct header {
  unsigned char *previousHash; 
  unsigned char rootHash[32];
  int timestamp;
  double target;
  unsigned int nonce;
};
typedef struct header Header;

//block struct
struct block {
  Header *header;
  InternalNode *rootHash;
};
typedef struct block Block;

void create_block(Block *, InternalNode *, Block *, FILE *, FILE *, unsigned char *);
void initialize_block(Block *, InternalNode *, unsigned char *, FILE *, FILE *);
void populate_header(Block *, InternalNode *, Block *, unsigned char *);
void initialize_header(Block *, InternalNode *, unsigned char *);
void generate_nonce(Header *, InternalNode *);
int *timestamp();

#endif

/* HASH IMPLEMENTATION */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "sha256.h"
#include "merkleTree.h"
#include "hash.h"
#define BlockSize (3*SHA256_BLOCK_SIZE + 4*sizeof(int))

unsigned char *hash(unsigned char *arr) {
    //printf("content passed in: %s\n", arr);
    BYTE* buf = malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    //initialize, update, and final functions to produce 32-byte hash digest
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, strlen(arr));
	sha256_final(&ctx, buf);
    // printf ("\ncontent passed out:\n");
    // for(int i = 0 ; i < SHA256_BLOCK_SIZE; i++){
    //     printf("%x", buf[i]);
    // }
    // printf("\n");
    return (unsigned char *)buf;
}

//for some reason, when passing in a strcat of two hashes, it reads strlen(arr) 30 instead of 64
//So we hard coded 64 into the code with 2*SHA256_BLOCK_SIZE
//This has to do with strlen()!
unsigned char *hash64(unsigned char *arr) {
    //printf("content passed in: %s\n", arr);
    BYTE* buf = malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    //initialize, update, and final functions to produce 32-byte hash digest
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, 2*SHA256_BLOCK_SIZE);
	sha256_final(&ctx, buf);
    // printf ("\ncontent passed out:\n");
    // for(int i = 0 ; i < SHA256_BLOCK_SIZE; i++){
    //     printf("%x", buf[i]);
    // }
    // printf("\n");
    return (unsigned char *)buf;
}


unsigned char *hash112(unsigned char *arr) {
    //printf("content passed in: %s\n", arr);
    BYTE* buf = malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    //initialize, update, and final functions to produce 32-byte hash digest
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, BlockSize);
	sha256_final(&ctx, buf);
    // printf ("\ncontent passed out:\n");
    // for(int i = 0 ; i < SHA256_BLOCK_SIZE; i++){
    //     printf("%x", buf[i]);
    // }
    // printf("\n");
    return (unsigned char *)buf;
}

/* HEADER FILE FOR HASH IMPLEMENTATION */

#ifndef HASH_DEF
#define HASH_DEF

unsigned char* hash(unsigned char *);
unsigned char* hash64(unsigned char *);
unsigned char* hash112(unsigned char *);

#endif

prog: main.o merkleTree.o readFile.o sha256.o hash.o sort.o block.o printMerkleTree.o printBlock.o serialize.o
	gcc -g -o prog merkleTree.o readFile.o sha256.o hash.o block.o sort.o printMerkleTree.o printBlock.o serialize.o main.o

main.o: main.c
	gcc -g -Wall -Wextra -Wwrite-strings -c main.c

sort.o: sort.h sort.c
	gcc -g -Wall -Wextra -Wwrite-strings -c sort.c

readFile.o: readFile.h sort.h readFile.c
	gcc -g -Wall -Wextra -Wwrite-strings -c readFile.c

merkleTree.o: merkleTree.h sha256.h merkleTree.c
	gcc -g -Wall -Wextra -Wwrite-strings -c merkleTree.c

sha256.o: sha256.h sha256.c
	gcc -g -Wall -Wextra -Wwrite-strings -c sha256.c

hash.o: hash.h sha256.h hash.c
	gcc -g -Wall -Wextra -Wwrite-strings -c hash.c

block.o: block.h merkleTree.h sha256.h block.c
	gcc -g -Wall -Wextra -Wwrite-strings -c block.c

printMerkleTree.o: printMerkleTree.h printMerkleTree.c
	gcc -g -Wall -Wextra -Wwrite-strings -c printMerkleTree.c

printBlock.o: printBlock.h printBlock.c
	gcc -g -Wall -Wextra -Wwrite-strings -c printBlock.c

serialize.o: serialize.h serialize.c
	gcc -g -Wall -Wextra -Wwrite-strings -c serialize.c

clean:
	rm -f prog
	rm -f *.o

/* IMPLEMENTATION OF MERKLE TREE */

#include "printMerkleTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sha256.h"
#include "hash.h"

void createLeafNodes(LeafNode *leafnode, char **SortedArray, int count) {
	printf("\n###### createLeafNodes ######\n");
	for(int i = 0; i < count; i++){
		//printf("Sorted Array is: %s\n", SortedArray[i]);     
		strcpy(leafnode[i].value, SortedArray[i]);//correct
		printf("LeafNode value: %s\n", leafnode[i].value);//correct
		unsigned char *returned_str = hash(SortedArray[i]);
		for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
       		printf("%x", returned_str[n]);
		}   
		printf("\n");
		// printf("**************\n");
		// memcpy(leafnode[i].hash, returned_str, SHA256_BLOCK_SIZE);
		// for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		// 	printf("%x", (unsigned char) leafnode[i].hash[n]);
		// }
		// printf("***********************\n");
		//strcpy(leafnode[i].hash, SortedArray[i]);
		//printf("\nLeafNode hash: %s\n", leafnode[i].hash);
    }
}

void convertLeaftoInternal(InternalNode *newInternal, LeafNode *LeafNode, int count){
	for(int i = 0; i < count; i++){
		//printf("!!!!!!!!!!leafNode[i] is: %s", LeafNode[i].value);
		memcpy(newInternal[i].hash, &LeafNode[i].hash, SHA256_BLOCK_SIZE);
		strncpy(newInternal[i].leftEdge, &LeafNode[i].value, 100);
		//printf("\n*******************Left edge assigned is: %s\n", newInternal[i].leftEdge);
		strncpy(newInternal[i].rightEdge, &LeafNode[i].value, 100);
		//printf("\n*******************Right edge assigned is: %s\n", newInternal[i].rightEdge);
		newInternal[i].leftChild = NULL;
		newInternal[i].rightChild = NULL;
	}
}

InternalNode *merkleTreeRoot(InternalNode *leafNodes, int count){
	//get ceiling
	int parents = count/2 + count%2;
	InternalNode *newInternal = malloc(((count/2)+(count%2))*sizeof(InternalNode));
	unsigned char *temp = (unsigned char *) malloc(2*SHA256_BLOCK_SIZE);
	int j = 0;
	for(int i = 0; i < count; i+=2){
		if(i != count-1){	
			memcpy(temp, leafNodes[i].hash, SHA256_BLOCK_SIZE);
			memcpy(temp+SHA256_BLOCK_SIZE, leafNodes[i+1].hash, SHA256_BLOCK_SIZE);//up until this point, this is corret
			memcpy(newInternal[j].hash, hash64(temp), SHA256_BLOCK_SIZE); //memory shift by SHA256_BLOCK_SIZE
			newInternal[j].leftChild = &leafNodes[i].hash;
			newInternal[j].rightChild = &leafNodes[i+1].hash;
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
			strncpy(newInternal[j].rightEdge, &leafNodes[i+1].rightEdge, 100);
		}else{
			unsigned char *temp = &(leafNodes[i].hash);
			memcpy(newInternal[j].hash, hash(hash(temp)), SHA256_BLOCK_SIZE);
			newInternal[j].leftChild = &leafNodes[i];
			newInternal[j].rightChild = NULL;
			strncpy(newInternal[j].rightEdge, &leafNodes[i].rightEdge, 100);
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
		}
		j++;
		if(parents == 1){
			return newInternal;
		}
	}
	return merkleTreeRoot(newInternal, parents);
}



//This is a copy of the merkleTreeRoot that will print out to terminal all the info
/*
InternalNode *merkleTreeRoot(InternalNode *leafNodes, int count){
	printf("\n###### merkleTreeRoot ######\n");
	//get ceiling
	//printf("********Count is: %d\n",count);
	int parents = count/2 + count%2;
	InternalNode *newInternal = malloc(((count/2)+(count%2))*sizeof(InternalNode));
	unsigned char *temp = (unsigned char *) malloc(2*SHA256_BLOCK_SIZE);
	int j = 0;
	for(int i = 0; i < count; i+=2){
		printf("\nI = %d\n",i);
		if(i != count-1){	
			printf("\nhash[i] is: \n");
			for	 (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				printf("%x", (unsigned char) leafNodes[i].hash[n]);
			}
			printf("\n");
			memcpy(temp, leafNodes[i].hash, SHA256_BLOCK_SIZE);
			printf("\ntemp is: \n");
			for (int n = 0; n < SHA256_BLOCK_SIZE; n ++) {
				printf("%x", (unsigned char) temp[n]);
			}
			printf("\n");
			printf("\ni+1 is: \n");
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				if ((unsigned char)leafNodes[i+1].hash[n] <= 0x0f) {
						printf("0%x", (unsigned char)leafNodes[i+1].hash[n]);
				}else{
						printf("%x", (unsigned char)leafNodes[i+1].hash[n]);
				}
			}
			printf("\n");
			memcpy(temp+SHA256_BLOCK_SIZE, leafNodes[i+1].hash, SHA256_BLOCK_SIZE);//up until this point, this is corret
			memcpy(newInternal[j].hash, hash64(temp), SHA256_BLOCK_SIZE); //memory shift by SHA256_BLOCK_SIZE
			printf(".....temp after strcat is: \n");
			for (int n = 0; n < 2*SHA256_BLOCK_SIZE; n++) {
				if ((unsigned char)temp[n] <= 0x0f) {
						printf("0%x", temp[n]);
				}else{
						printf("%x", temp[n]);
				}
			}
			printf("\nhash of the concatenation: \n");
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				if ((unsigned char)newInternal[j].hash[n] <= 0x0f) {
						printf("0%x", (unsigned char)newInternal[j].hash[n]);
				}else{
						printf("%x", (unsigned char)newInternal[j].hash[n]);
				}
			}
			newInternal[j].leftChild = &leafNodes[i].hash;
			newInternal[j].rightChild = &leafNodes[i+1].hash;
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
			printf("\n---------The left edge is: %s\n",newInternal[j].leftEdge);
			strncpy(newInternal[j].rightEdge, &leafNodes[i+1].rightEdge, 100);
			printf("\n---------The right edge is: %s\n",newInternal[j].rightEdge);
		}else{
			unsigned char *temp = &(leafNodes[i].hash);
			printf("\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ODD CASE: temp is:\n");
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				printf("%x", (unsigned char) temp[n]);
			}
			printf("\n");
			memcpy(newInternal[j].hash, hash(hash(temp)), SHA256_BLOCK_SIZE);
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				if ((unsigned char)newInternal[i].hash[n] <= 0x0f) {
						printf("0%x", (unsigned char) newInternal[j].hash[n]);
				}else{
						printf("%x", (unsigned char) newInternal[j].hash[n]);
				}
			}
			newInternal[j].leftChild = &leafNodes[i];
			newInternal[j].rightChild = NULL;
			strncpy(newInternal[j].rightEdge, &leafNodes[i].rightEdge, 100);
			printf("\n!!!!!!!!!!!!Left edge assigned is: %s\n", newInternal[j].leftEdge);
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
			printf("\n!!!!!!!!!!!!Right edge assigned is: %s\n",newInternal[j].rightEdge);
		}
		j++;
		if(parents == 1){
			//printf("\n*****Root left edge  is: %s\n", newInternal[0].leftEdge);
			//printf("\n*****Root right edge  is: %s\n", newInternal[0].rightEdge);
			//memcpy(returnedNode[0].hash, newInternal[0].hash,SHA256_BLOCK_SIZE);
			return newInternal;
		}
	}
	return merkleTreeRoot(newInternal, parents);
}
*/


/*
free_merkle_tree(InternalNode *internalNodes) {
	free(internalNodes);
}
*/

/* HEADER FILE FOR FUNCTIONS ASSOCIATED WITH MERKLE TREE */

#ifndef MERKLETREE_DEF
#define MERKLETREE_DEF

#include <stdio.h>
#include "sha256.h"
#define BUFFER 100

//leaf node struct
struct leaf_node {
  char hash[SHA256_BLOCK_SIZE + 1];
  char value[100];
};
typedef struct leaf_node LeafNode;

//internal node struct
struct node {
  char hash[2*SHA256_BLOCK_SIZE + 1];
  struct node *leftChild;
  struct node *rightChild;
  char leftEdge[100];
  char rightEdge[100];
};
typedef struct node InternalNode;

void createLeafNodes(LeafNode *, char **, int);
void convertLeaftoInternal(InternalNode *, LeafNode *, int);
InternalNode *merkleTreeRoot(InternalNode *, int);

#endif

/* HEADER FILE FOR PRINTING BLOCK */

#ifndef PRINTBLOCK_DEF
#define PRINTBLOCK_DEF

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "block.h"
#include "merkleTree.h"
#include "printMerkleTree.h"
#include "printBlock.h"
#include "sha256.h"
#include "time.h"
#include "hash.h"

void print_block(Block*, int, FILE*);

#endif

/* IMPLEMENTATION FOR PRINTING BLOCK TO FILE*/

#include "printBlock.h"

//NEED TO FIX THIS FILE

void print_block(Block* B_ptr, int ID_K, FILE* out){
    printf("entered print_block!!!!!!!!!!!!!!!!!\n");
    fprintf(out, "header: \n");
    fprintf(out, "PreviousHash: %s, RootHash: %s", B_ptr->header->previousHash, B_ptr->header->rootHash);
    fprintf(out, "");
    fprintf(out, "TimeStamp: %d, Target: %s, nonce: %d", B_ptr->header->timestamp, B_ptr->header->target, B_ptr->header->nonce);
    fprintf(out, "");
    fprintf(out, "content: \n");
    print_merkle_tree(B_ptr->rootHash, ID_K, out);
}

/* IMPLEMENTATION FOR PRINTING MERKLE TREE */

#include "printMerkleTree.h"

//NEED TO FIX THIS FILE

void print_merkle_tree(InternalNode *root, int ID, FILE *output) {
	fprintf(output,"#######");
	fprintf(output,"iD is:%d\n", ID);
	
	if(strcmp(root->leftEdge, root->rightEdge)){
		fprintf(output,"left edge is: %s\n", root->leftEdge);
	}
	
	fprintf(output,"hash is:");
	for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		fprintf(output,"%x", (unsigned char) root->hash[n]);
	}
	fprintf(output,"\n");

	if(strcmp(root->leftEdge, root->rightEdge)){
		fprintf(output,"right edge is: %s\n", root->rightEdge);
	}else{
		fprintf(output, "leafNode value is: %s\n", root->leftEdge);
		fprintf(output,"leaf node\n");
	}

	if (root->leftChild != NULL) {
		print_merkle_tree(root->leftChild, 2*ID, output);
        fprintf(output, "leftChild is: %d\n", 2*ID);
	}
	if (root->rightChild != NULL) {
		print_merkle_tree(root->rightChild, 2*ID+1, output);
        fprintf(output, "rightChild is: %d\n",  2*ID+1);
	}
}

/* HEADER FILE FOR PRINTING MERKLE TREE */  

#ifndef PRINT_MERKLETREE_DEF
#define PRINT_MERKLETREE_DEF

#include "merkleTree.h"

void print_merkle_tree(InternalNode *, int, FILE *);

#endif

/* IMPLEMENTATION OF READING USER FILE AND SORTING CONTENTS */

#include "readFile.h"

FILE *Fopen(const char *file, const char *permission) {
    FILE *fp = fopen(file, permission);
    if (fp == NULL) {
        printf("error opening!!!!!!!!");
        exit(0);
    }
    return fp;
}

void Fclose(FILE *fp) {
    if (fp && !fclose(fp)) {
        printf("error closing file");
    }
}

size_t Fread(void *buffer, size_t size, size_t nmemb, FILE *file) {
    size_t readBytes = fread(buffer, size, nmemb, file);
    if (readBytes == 0) {
        printf("empty file!!!!!!!");
    }
    if (ferror(file)) {
        printf("error reading file!!!!!!");
    }
    return readBytes;
}

size_t Fwrite(void *buffer, size_t size, size_t nmemb, FILE *file) {
    size_t writtenBytes = 0;
    while ((writtenBytes = fwrite(buffer, size, nmemb, file) == 0)) {
        if (ferror(file) | fileno(file)) {
            printf("error writing to file!!!!!!");
            exit(0);
        }
    }
    return writtenBytes;
}

int lineCount(FILE * inputFile) {
    int counter = 0;
    char character;
    for (character = getc(inputFile); character != EOF; character = getc(inputFile)) {
        if (character == '\n') { 
            counter = counter + 1;
        }
    }
    if(character != '\n'){
        counter++;
    }
    return counter; 
}  

void ReadOneFile(char** arr, char *filename)
{
    //printf("file name %s\n", filename);
    FILE *fp = Fopen(filename, "r");
    int count = (int)lineCount(fp);
    //printf("number of lines: %d\n", count);
    
    //***fp must be open twice OR use rewind() to reset pointer
    fp = Fopen(filename, "r"); //open the file
    int z = 0;
    while ( fgets(arr[z], 100, fp) != NULL ){
        size_t len = strlen(arr[z]);
        if( arr[z][len-1] == '\n') {
            arr[z][len-1] = '\0';
        }
        //printf ("elements are: %s", arr[z]);
        z++;
    }
    //printf("Total string put in arr is: %d\n", z);
    fclose(fp);

    sort(arr, count);   

    // for (int i = 0; i < count; i++)
    // {
    //     printf("Sorted array is: %s\n", arr[i]);
    // }
}

int *GetLineNumbers(char **filename, int count) {
    int *lineNum = (int *)malloc(count*sizeof(int));
    FILE *fp;
    for(int i = 0; i < count; i++){
        fp = fopen(filename[i], "r");
        lineNum[i] = (int) lineCount(fp);
        //printf("!!!!!Assigned line number %d to %d\n", i, lineNum[i]);
    }
    return lineNum;
}

/* HEADER FILE FOR READFILE.C */

#ifndef READFILE_DEF
#define READFILE_DEF

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sort.h"

FILE *Fopen(const char *, const char *);
void Fclose(FILE *);
size_t Fread(void *, size_t, size_t, FILE *); 
size_t Fwrite(void *, size_t, size_t, FILE *);
void ReadOneFile(char**, char *);
int *GetLineNumbers(char **, int);
//size_t Fwrite(void *, size_t, size_t, FILE *);

#endif


!/bin/bash
#Bash Shell Script
#RECOMPILE

#Set EXECUTE PERMISSIONS
chmod 755 recompile

#Set path for bash script
export PATH="$PATH:."

make clean;
make;


#!/bin/bash
#Bash Shell Script
#RUN 

#Set EXECUTE PERMISSIONS
chmod 755 run

#Set path for bash script
export PATH="$PATH:."

#Run executable
prog

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

/* HEADER FILE FOR SERIALIZING BLOCK */

#ifndef SERIALIZE_DEF
#define SERIALIZE_DEF

#include "block.h"
#include "printMerkleTree.h"
#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct block2 {
  unsigned char previousHash[SHA256_BLOCK_SIZE]; 
  unsigned char rootHashHeader[SHA256_BLOCK_SIZE];
  int timestamp;
  double target;
  unsigned int nonce;
  unsigned char rootHash[SHA256_BLOCK_SIZE];//same as line 15
  struct block2 *prevBlock;
  struct block2 *nextBlock;
};
typedef struct block2 Block2;

void serialize_blockchain(Block *, FILE *);     
void rebuild_block(FILE *, Block2 *);          
void serialize_first_blockchain(Block *, FILE *);
void rebuild_first_block(FILE *, Block2 *); 
void rebuild_merkleTree(Block *);

#endif

/*********************************************************************
* Filename:   sha256.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the SHA-256 hashing algorithm.
              SHA-256 is one of the three algorithms in the SHA2
              specification. The others, SHA-384 and SHA-512, are not
              offered in this implementation.
              Algorithm specification can be found here:
               * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
			   * https://www.daemon-systems.org/man/SHA256_Update.3.html
              This implementation uses little endian byte order.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdlib.h>
#include <memory.h>
#include "sha256.h"

/****************************** MACROS ******************************/
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

/**************************** VARIABLES *****************************/
static const WORD k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

/*********************** FUNCTION DEFINITIONS ***********************/
void sha256_transform(SHA256_CTX *ctx, const BYTE data[])
{
	WORD a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for ( ; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len)
{
	WORD i;

	for (i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			sha256_transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void sha256_final(SHA256_CTX *ctx, BYTE hash[])
{
	WORD i;

	i = ctx->datalen;

	// Pad whatever data is left in the buffer.
	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		sha256_transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	sha256_transform(ctx, ctx->data);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i = 0; i < 4; ++i) {
		hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}


/*********************************************************************
* Filename:   sha256.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding SHA1 implementation.
*********************************************************************/

#ifndef SHA256_H
#define SHA256_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int  WORD;             // 32-bit word, change to "long" for 16-bit machines

typedef struct {
	BYTE data[64];
	WORD datalen;
	unsigned long long bitlen;
	WORD state[8];
} SHA256_CTX;

/*********************** FUNCTION DECLARATIONS **********************/
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len);
void sha256_final(SHA256_CTX *ctx, BYTE hash[]);

#endif   // SHA256_H

/* IMPLEMENTATION FOR SORTING ALGORITHM -- BUBBLE SORT */

#include <stdio.h>
#include <string.h>
#include "merkleTree.h"
#include "sort.h"

/* Double Bubble-Sort: sorting by length and alphanumeric */
void sort(char** arr, int n) { 
   int i, j; 
   for (i = 0; i < n-1; i++)
   {
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)
       {  
          if(strlen(arr[j]) >= strlen(arr[j+1]))
          {
            char* temp = arr[j]; 
            arr[j] = arr[j+1]; 
            arr[j+1] = temp;
          }
       }
   }
   for (i = 0; i < n-1; i++)
   {
       for (j = 0; j < n-i-1; j++)
       {  
          if(strlen(arr[j]) >= strlen(arr[j+1]))
          {
            if (strcmp(arr[j], arr[j+1]) > 0)
            {
              char* temp = arr[j]; 
              arr[j] = arr[j+1]; 
              arr[j+1] = temp;
            }
          }
       }
   }
} 


/* HEADER FILE FOR SORT.C */

#ifndef SORT_DEF
#define SORT_DEF

void sort(char**, int);

#endif
