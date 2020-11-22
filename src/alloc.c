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
    printf("input the number of files you want opened: "); 
	scanf("%d",&count);
    char **fileNames = (char **)malloc(count * sizeof(FILE *));
    for(int i = 0; i < count; i++){
        fileNames[i] = (char *)malloc(BUFFER);
    }
    for (int i = 0; i < count; i++){
        printf("enter the filename[%d]: ", i); 
        scanf("%s", fileNames[i]);
    }
    int *lineNum = malloc(count * sizeof(int));
    lineNum = GetLineNumbers(fileNames, count);
    char **arr = (char **)malloc(100 * sizeof(char *));
    for (int i = 0; i < BUFFER; i++)
    {
        arr[i] = malloc(BUFFER);
    }

    /* output_blockchain = file containing the serialization of the entire blockchain */
    char output_fileName[] = "output.blockchain.txt";
    FILE *output_blockchain = fopen(output_fileName,"wb"); 

    /* actualFileNameMerkleTree and actualFileNameBlock are arrays storing file names for merkleTree and Block filenames */
    char actualFileNameMerkleTree[count][255]; 
    char actualFileNameBlock[count][255]; 
    for(int i = 0; i < count; i++){
        for( int j = 0; j < 266; j++){
            actualFileNameMerkleTree[i][j] = NULL;
            actualFileNameBlock[i][j] = NULL;
        }   
    }

    /* previous block of the first block = 0 */
    unsigned char *pointerToZero = (unsigned char *)malloc(sizeof(char));
    strncpy(pointerToZero, "0", 1);

    /* Block **block, LeafNode **leafNodes, InternalNode **internalNode, InternalNode **TreeRoot are 2D arrays */
    Block **block = (Block **)malloc(count * sizeof(Block *));
    LeafNode **leafNodes = (LeafNode **)malloc(count * sizeof(LeafNode *));
    InternalNode **internalNode = (InternalNode **)malloc(count * sizeof(InternalNode *));
    InternalNode **TreeRoot = (InternalNode **)malloc(count * sizeof(InternalNode *));
    for(int i = 0; i < count; i++){
        int fileNameCounter = strlen(fileNames[i]) - 4;
        for(int j = 0; j < fileNameCounter; j++){
            actualFileNameMerkleTree[i][j] = fileNames[i][j];
        }
        strcat(actualFileNameMerkleTree[i], ".merkletree.txt"); /* contains the output for merkle tree */

        /* outputMerkleTree = file containing the contents of a merkle tree associatd with block[i] */
        FILE *outputMerkleTree; 
        outputMerkleTree = fopen(actualFileNameMerkleTree[i],"w"); 
        for(int j = 0; j < fileNameCounter; j++){
            actualFileNameBlock[i][j] = fileNames[i][j];
        } 
        strcat(actualFileNameBlock[i], ".block.txt"); /* contains the output for the entire block contents */
        
        /* output_block = file containing the contents of a block's header contents + merkle tree associated with block[i] */
        FILE *output_block; 
        output_block = fopen(actualFileNameBlock[i],"wb"); 

        ReadOneFile(arr, fileNames[i]);
        leafNodes[i] = (LeafNode *)malloc(lineNum[i] * sizeof(LeafNode));
        block[i] = (Block *)malloc(sizeof(Block));                           /* malloc for block[i] */
        block[i]->header = (Header *)malloc(sizeof(Header));                 /* mallocing for header contents */
        createLeafNodes(leafNodes[i], arr, lineNum[i]);
        internalNode[i] = (InternalNode *)malloc(lineNum[i]*sizeof(InternalNode));
        convertLeaftoInternal(internalNode[i], leafNodes[i],lineNum[i]);
        TreeRoot[i] = (InternalNode *)malloc(sizeof(InternalNode));
        TreeRoot[i] = merkleTreeRoot(internalNode[i],lineNum[i]);

		 /* printing out the merkle tree */
        print_merkle_tree_value(arr, lineNum[i] , outputMerkleTree);
    	print_merkle_tree(TreeRoot[i], 1, outputMerkleTree); 
        fclose(outputMerkleTree);

        if(i != 0){
            /* add another block to the blockchain */
            create_block(block[i], TreeRoot[i], block[i-1], output_block, output_blockchain, pointerToZero);
        }else{
            /* add first block to the blockchain */
            initialize_block(block[i], TreeRoot[i], pointerToZero, output_block, output_blockchain); //first block, previous block pointing to 0
        }
        
        /* reset the contents of the array for re-use */
        for(int i = 0 ; i < 100; i++){
            for( int j = 0; j < 100; j++){
                arr[i][j] = NULL;
            }
        }
    }

    /* close file */
    fclose(output_blockchain);
    
    /* inserting serialized data into Block2 structure to rebuild blocks from disk */
    FILE *write_blockchain2 = fopen(output_fileName, "rb");
    Block2 **block2= (Block **)malloc(count * sizeof(Block2 *));
    for(int i = 0; i < count ; i ++) {
        block2[i] = (Block2 *)malloc(sizeof(Block2));
        if(i == 0){
            rebuild_first_block(write_blockchain2, block2[i]);
        }else{
            rebuild_block(write_blockchain2, block2[i]);
        }
    }

    /* validating the correctness of the blockchain */
    int validation_result; 
    for (int i = 0; i < count; i++) {
        validation_result = validation(block2[i], actualFileNameMerkleTree[i], actualFileNameBlock[i], i);
        if (validation_result == 0) {
            printf("\nvalidation of the blockchain failed!");
        }
    }
    printf("\nblockchain validated!\n");

    /* proof of membership of a query string in blockchain */
    char search_string[BUFFER];
    int inchain_result = 0;
    int block_number = 0;
    printf("Enter a query to search the contents of the blockchain: "); 
	scanf("%s",search_string);	//make sure user can ONLY enter an int (not char) while handling any potential errors
    for (int i = 0; i < count; i++) {
        if ((inchain_result = inchain(actualFileNameBlock[i], search_string)) == 1) {
            printf("Query found in blockchain!\n");
            printf("query '%s' found in block %d. Merkle Proof stored in: %s\n", search_string, block_number, actualFileNameBlock[block_number]);
            exit(0);
        }
        block_number++;
    }
    printf("Query NOT found in blockchain");

    /* free previously malloced memory */
    free(fileNames);
    free(lineNum);
    for (int i = 0; i < 100; i++) {
        free(arr[i]);
    }
    free(arr);
    for(int i = 0; i < count; i++){
        free(block[i]);
    }
    for(int i = 0; i < count; i++){
        free(leafNodes[i]);
    }
    for(int i = 0; i < count; i++){
        free(internalNode[i]);
    }
    for(int i = 0; i < count; i++){
        free(TreeRoot[i]);
    } 
    free(block);
    free(leafNodes);
    free(internalNode);
    free(TreeRoot);
}

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
/* HEADER FILE FOR BLOCK.C */

#ifndef BLOCK_DEF
#define BLOCK_DEF

#include "sha256.h"
#include "merkleTree.h"

/* header struct */
struct header {
  unsigned char *previousHash; 
  unsigned char rootHash[32];
  int timestamp;
  double target;
  unsigned int nonce;
};
typedef struct header Header;

/* block struct */
struct block {
  Header *header;
  InternalNode *rootHash;
};
typedef struct block Block;

/* function prototypes for block */
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

/* different hash function depending on the size of the data passed-in */

unsigned char *hash(unsigned char *arr) {
    BYTE* buf = (BYTE *)malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    /* initialize, update, and final functions to produce 32-byte hash digest */
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, strlen(arr));
	sha256_final(&ctx, buf);
    return (unsigned char *)buf;
}

unsigned char *hash64(unsigned char *arr) {
    BYTE* buf = (BYTE *)malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    /* initialize, update, and final functions to produce 32-byte hash digest */
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, 2*SHA256_BLOCK_SIZE);
	sha256_final(&ctx, buf);
    return (unsigned char *)buf;
}

unsigned char *hash112(unsigned char *arr) {
    BYTE* buf = (BYTE *)malloc(SHA256_BLOCK_SIZE);
	SHA256_CTX ctx;

    /* initialize, update, and final functions to produce 32-byte hash digest */
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, BlockSize);
	sha256_final(&ctx, buf);
    return (unsigned char *)buf;
}
/* HEADER FILE FOR HASH IMPLEMENTATION */

#ifndef HASH_DEF
#define HASH_DEF

unsigned char* hash(unsigned char *);
unsigned char* hash64(unsigned char *);
unsigned char* hash112(unsigned char *);

#endif
/* IMPLEMENTATION OF MERKLE TREE */

#include "printMerkleTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sha256.h"
#include "hash.h"

/* createLeafNodes will create the leaf nodes of merkle tree */
void createLeafNodes(LeafNode *leafnode, char **SortedArray, int count) {
	for(int i = 0; i < count; i++){
		strcpy(leafnode[i].value, SortedArray[i]);				   /* copy value into leafnode */
		unsigned char *returned_str = hash(SortedArray[i]); 
		memcpy(leafnode[i].hash, returned_str, SHA256_BLOCK_SIZE); /* copy hash into leafnode */
    }
}

/* convertLeaftoInternal will convert leaf nodes to internal nodes to simplify recursion in merkleTreeRoot() function */
void convertLeaftoInternal(InternalNode *newInternal, LeafNode *LeafNode, int count){
	for(int i = 0; i < count; i++){
		memcpy(newInternal[i].hash, &LeafNode[i].hash, SHA256_BLOCK_SIZE);
		strncpy(newInternal[i].leftEdge, &LeafNode[i].value, 100);
		strncpy(newInternal[i].rightEdge, &LeafNode[i].value, 100);
		newInternal[i].leftChild = NULL;
		newInternal[i].rightChild = NULL;
	}
}

/* merkleTreeRoot constructs merkle tree */
InternalNode *merkleTreeRoot(InternalNode *leafNodes, int count){
	int parents = count/2 + count%2;
	InternalNode *newInternal = malloc(((count/2)+(count%2))*sizeof(InternalNode));
	unsigned char *temp = (unsigned char *) malloc(2*SHA256_BLOCK_SIZE);
	int j = 0;
	for(int i = 0; i < count; i+=2){
		if(i != count-1){	
			memcpy(temp, leafNodes[i].hash, SHA256_BLOCK_SIZE);
			memcpy(temp+SHA256_BLOCK_SIZE, leafNodes[i+1].hash, SHA256_BLOCK_SIZE);
			memcpy(newInternal[j].hash, hash64(temp), SHA256_BLOCK_SIZE);
			newInternal[j].leftChild = &leafNodes[i].hash;
			newInternal[j].rightChild = &leafNodes[i+1].hash;
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
			strncpy(newInternal[j].rightEdge, &leafNodes[i+1].rightEdge, 100);
		}else{
			/* odd case for single leafnode */
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
/* HEADER FILE FOR FUNCTIONS ASSOCIATED WITH MERKLE TREE */

#ifndef MERKLETREE_DEF
#define MERKLETREE_DEF

#include <stdio.h>
#include "sha256.h"
#define BUFFER 100

/* leaf node struct */
struct leaf_node {
  char hash[SHA256_BLOCK_SIZE + 1];
  char value[100];
};
typedef struct leaf_node LeafNode;

/* internal node struct */
struct node {
  char hash[2 * SHA256_BLOCK_SIZE + 1];
  struct node *leftChild;
  struct node *rightChild;
  char leftEdge[100];
  char rightEdge[100];
};
typedef struct node InternalNode;

/* function prototypes for merkle-tree */
void createLeafNodes(LeafNode *, char **, int);
void convertLeaftoInternal(InternalNode *, LeafNode *, int);
InternalNode *merkleTreeRoot(InternalNode *, int);

#endif
/* IMPLEMENTATION FOR PRINTING BLOCK TO FILE */

#include "printBlock.h"

/* printing out the first block */
void print_first_block(Block *B_ptr, int ID_K, FILE *out){
    fprintf(out, "BEGIN BLOCK\n");
    fprintf(out, "BEGIN HEADER\n");
    fprintf(out, "previous hash is: %s", B_ptr->header->previousHash);
    fprintf(out, "\nroot hash is: ");
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        if ((unsigned char)B_ptr->header->rootHash[i] <= 0x0f) {
            fprintf(out, "0%x", (unsigned char)B_ptr->header->rootHash[i]);
        } else{
            fprintf(out, "%x", (unsigned char)B_ptr->header->rootHash[i]);
        }
    }
    fprintf(out, "\ntimestamp is: %d\n", B_ptr->header->timestamp);
    fprintf(out, "target value is: %f\n", B_ptr->header->target);
    fprintf(out, "nonce is: %d\n", B_ptr->header->nonce);
    fprintf(out, "END HEADER\n");
    print_merkle_tree(B_ptr->rootHash, ID_K, out);
    fprintf(out, "END BLOCK");
}

/* printing out additional blocks */
void print_block(Block* B_ptr, int ID_K, FILE* out){
    fprintf(out, "BEGIN BLOCK\n");
    fprintf(out, "BEGIN HEADER\n");
    fprintf(out, "previous hash is: ");
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        if ((unsigned char)B_ptr->header->previousHash[i] <= 0x0f) {
            fprintf(out, "0%x", (unsigned char)B_ptr->header->previousHash[i]);
        } else{
            fprintf(out, "%x", (unsigned char)B_ptr->header->previousHash[i]);
        }
    }
    fprintf(out, "\n");
    fprintf(out, "root hash is: ");
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        if ((unsigned char)B_ptr->header->rootHash[i] <= 0x0f) {
            fprintf(out, "0%x", (unsigned char)B_ptr->header->rootHash[i]);
        } else{
            fprintf(out, "%x", (unsigned char)B_ptr->header->rootHash[i]);
        }
    }
    fprintf(out, "\ntimestamp is: %d\n", B_ptr->header->timestamp);
    fprintf(out, "target value is: %f\n", B_ptr->header->target);
    fprintf(out, "nonce is: %d\n", B_ptr->header->nonce);
    fprintf(out, "END HEADER\n");
    print_merkle_tree(B_ptr->rootHash, ID_K, out);
    fprintf(out, "END BLOCK");
}
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

/* function prototypes for printing blocks */
void print_first_block(Block *, int, FILE *);
void print_block(Block *, int, FILE *);

#endif
/* IMPLEMENTATION FOR PRINTING MERKLE TREE */

#include "printMerkleTree.h"

/* printing out merkle tree associated with file contents */
void print_merkle_tree(InternalNode *root, int ID, FILE *output) {
	fprintf(output,"ID is: %d\n", ID);
	if(root->leftChild != NULL || root->rightChild != NULL){
		fprintf(output,"left edge is: %s\n", root->leftEdge);
	}
	fprintf(output,"hash is: ");
	for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		fprintf(output,"%x", (unsigned char) root->hash[n]);
	}
	fprintf(output,"\n");

	if(root->leftChild != NULL || root->rightChild != NULL){
		fprintf(output,"right edge is: %s\n", root->rightEdge);
	}else{
		fprintf(output, "leafNode value is: %s\n", root->leftEdge);
		fprintf(output,"leaf node\n");
	}

	if (root->rightChild != NULL) {
		fprintf(output, "leftChild is: %d\n", 2*ID);
		fprintf(output, "rightChild is: %d\n",  2*ID+1);
		print_merkle_tree(root->leftChild, 2*ID, output);
		print_merkle_tree(root->rightChild, 2*ID+1, output);
	}else if (root->leftChild != NULL) {
		fprintf(output, "leftChild is: %d\n", 2*ID);
		fprintf(output, "rightChild is NULL\n");
		print_merkle_tree(root->leftChild, 2*ID, output);
	}
}

/* print original file contents */
void print_merkle_tree_value(char** arr,int lineNum, FILE *output){
	fprintf(output, "%d\n", lineNum); 
	for(int i = 0; i < lineNum; i++){
		fprintf(output, "%s\n",  arr[i]);
	}
}
/* HEADER FILE FOR PRINTING MERKLE TREE */  

#ifndef PRINT_MERKLETREE_DEF
#define PRINT_MERKLETREE_DEF

#include "merkleTree.h"

/* function prototypes for printing merkle tree */
void print_merkle_tree(InternalNode *, int, FILE *);
void print_merkle_tree_value(char **, int, FILE *);

#endif
/* HEADER FILE FOR READFILE.C */

#ifndef READFILE_DEF
#define READFILE_DEF

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sort.h"

/* function prototypes for reading/writing files */
FILE *Fopen(const char *, const char *);
void Fclose(FILE *);
size_t Fread(void *, size_t, size_t, FILE *); 
size_t Fwrite(void *, size_t, size_t, FILE *);
void ReadOneFile(char**, char *);
int *GetLineNumbers(char **, int);

#endif

/* IMPLEMENTATION OF READING USER FILE AND SORTING CONTENTS */

#include "readFile.h"

/* Fopen (wrapper for fopen) for opening file with error handling */
FILE *Fopen(const char *file, const char *permission) {
    FILE *fp = fopen(file, permission);
    if (fp == NULL) {
        printf("error opening one or more files!");
        exit(0);
    }
    return fp;
}

/* Fclose (wrapper for fclose) for closing file with error handling */
void Fclose(FILE *fp) {
    if (fp && !fclose(fp)) {
        printf("error closing one or more files!");
    }
}

/* Fread (wrapper for fread) for reading from file with error handling */
size_t Fread(void *buffer, size_t size, size_t nmemb, FILE *file) {
    size_t readBytes = fread(buffer, size, nmemb, file);
    if (readBytes == 0) {
        printf("one or more files is empty!");
    }
    if (ferror(file)) {
        printf("error reading one or more files!");
    }
    return readBytes;
}

/* Fwrite (wrapper for fwrite) for writing to file with error handling */
size_t Fwrite(void *buffer, size_t size, size_t nmemb, FILE *file) {
    size_t writtenBytes = 0;
    while ((writtenBytes = fwrite(buffer, size, nmemb, file) == 0)) {
        if (ferror(file) | fileno(file)) {
            printf("error writing to one or more files!");
            exit(0);
        }
    }
    return writtenBytes;
}

/* returns number of lines in a file */
int lineCount(FILE * inputFile) {
    int counter = 0;
    char character;
    for (character = getc(inputFile); character != EOF; character = getc(inputFile)) {
        if (character == '\n') { 
            counter++;
        }
    }
    return counter; 
}  

/* reads the contents of a file */
void ReadOneFile(char** arr, char *filename) {
    FILE *fp = Fopen(filename, "r");
    int count = (int)lineCount(fp);
    fp = Fopen(filename, "r");                   /* open file again or rewind() to reset pointer */
    int z = 0;
    while ( fgets(arr[z], 100, fp) != NULL ){
        size_t len = strlen(arr[z]);
        if( arr[z][len-1] == '\n') {
            arr[z][len-1] = '\0';
        }
        z++;
    }
    fclose(fp);

    /* sort file contents */
    sort(arr, count);   
}

/* calling function for reading number of lines in a file */
int *GetLineNumbers(char **filename, int count) {
    int *lineNum = (int *)malloc(count*sizeof(int));
    FILE *fp;
    for(int i = 0; i < count; i++){
        fp = fopen(filename[i], "r");
        lineNum[i] = (int) lineCount(fp);
    }
    return lineNum;
}
#!/bin/bash
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

/* serialize_first_block serializes the first block from memory to disk (header contents and merkle tree) */
void serialize_first_blockchain(Block *block, FILE *write_blockchain){
    /* ***write data in BINARY representation to disk -- the data will look garbaled*** */
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
    fread(buffer_previousHash, sizeof(unsigned char), 32, output_blockchain);
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
/* HEADER FILE FOR SERIALIZING BLOCK */

#ifndef SERIALIZE_DEF
#define SERIALIZE_DEF

#include "block.h"
#include "printMerkleTree.h"
#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Block2 struct */
struct block2 {
  unsigned char previousHash[SHA256_BLOCK_SIZE]; 
  unsigned char rootHashHeader[SHA256_BLOCK_SIZE];
  int timestamp;
  double target;
  unsigned int nonce;
  unsigned char rootHash[SHA256_BLOCK_SIZE];
  struct block2 *prevBlock;
  struct block2 *nextBlock;
};
typedef struct block2 Block2;

/* function prototype for serializing blockchain */
void serialize_blockchain(Block *, FILE *);     
void rebuild_block(FILE *, Block2 *);          
void serialize_first_blockchain(Block *, FILE *);
void rebuild_first_block(FILE *, Block2 *); 
void rebuild_merkleTree(Block *);

#endif
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

/* function prototype for sort */
void sort(char**, int);

#endif
/* HEADER FILE FOR VALIDATING BLOCK */

#ifndef VALIDATE_DEF
#define VALIDATE_DEF

#include "serialize.h"
#include "block.h"
#include "merkleTree.h"

/* function prototypes for validating blockchain */
int validation(Block2 *, char [], char [], int);
int validate_header(Block2 *, char [], int);
int validate_merkle_tree(Block2 *, char []);
int compareMerkleTree(InternalNode *, FILE *, char *, int);
void clearBuffer(char *);

#endif

/* IMPLEMENTATION FOR VALIDATING BLOCK */

#include "validation.h"
#define BUFFER 119

/* validation validates entire blockchain block by block */
int validation(Block2 *block2, char actualFileNameMerkleTree[], char actualFileNameBlock[], int counter) {
    validate_header(block2, actualFileNameBlock, counter);
    validate_merkle_tree(block2, actualFileNameMerkleTree);

    if (validate_header(block2, actualFileNameBlock, counter) && validate_merkle_tree(block2, actualFileNameMerkleTree)) {
        return 1;
    }
    else {
        return 0;
    }
}

/* validate header contents of a block */
int validate_header(Block2 *block2, char actualFileNameBlock[], int counter) {
    /* buffers used for comparing contents of block with contents stored in input.block.txt and input.merkletree.txt */
    unsigned char buffer_first_previousHash[1];    
    unsigned char buffer_not_first_previousHash[64];
    unsigned char buffer_rootHash[64];
    int buffer_timestamp[8];
    double buffer_target[8];
    unsigned int buffer_nonce[10];
    unsigned char temp1_rootHash[100];
    unsigned char temp2_rootHash[100];
    unsigned char temp1_timestamp[100];
    unsigned char temp2_timestamp[100];
    unsigned char temp1_target[100];
    unsigned char temp2_target[100];
    unsigned char temp1_nonce[100];
    unsigned char temp2_nonce[100];

    /* open file associated with outputBlock file handler */
    FILE *outputBlock; 
    outputBlock = fopen(actualFileNameBlock,"rb");
    
    /* validate previous hash */
    fseek(outputBlock, 43, SEEK_SET);
    if (counter == 0) {
        fread(buffer_first_previousHash, sizeof(unsigned char), sizeof(buffer_first_previousHash), outputBlock);
    }
    else {
        fread(buffer_not_first_previousHash, sizeof(unsigned char), sizeof(buffer_not_first_previousHash), outputBlock);
    }

    /* validate roothash */
    fseek(outputBlock, 15, SEEK_CUR);
    fgets(buffer_rootHash, BUFFER, outputBlock);
    FILE *bufferRoothash; 
    bufferRoothash = fopen("bufferOutput.txt","w+b");
    for (int n = 0; n < sizeof(buffer_rootHash); n++) {
        fprintf(bufferRoothash, "%c", (unsigned char)buffer_rootHash[n]);
    }

    FILE *block2Roothash; 
    block2Roothash = fopen("block2Output.txt","w+b");
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        if ((unsigned char)block2->rootHash[i] <= 0x0f) {
                fprintf(block2Roothash, "0%x", (unsigned char)block2->rootHash[i]);
        } else{
                fprintf(block2Roothash, "%x", (unsigned char)block2->rootHash[i]);
        }
    }
    rewind(bufferRoothash);
    rewind(block2Roothash);

    int i=0; char l;
    while((l=fgetc(bufferRoothash))!=EOF) {
        temp1_rootHash[i] = l;
        i++;
    }
    int j =0; char t;
    while((t=fgetc(block2Roothash))!=EOF) {
        temp2_rootHash[j] = t;
        j++;
    }
    int retRoothash = memcmp(temp1_rootHash, temp2_rootHash, sizeof(buffer_rootHash));
    if(retRoothash != 0) {
        printf("Validation failed! Error with roothash!\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    /* validate timestamp */
    printf("\n");
    fseek(outputBlock, 14, SEEK_CUR);
    fgets(buffer_timestamp, BUFFER, outputBlock);
    FILE *bufferTimestamp; 
    bufferTimestamp = fopen("bufferOutput.txt","w+b");
    int timestamp_converted = atoi(buffer_timestamp);
    fprintf(bufferTimestamp, "%d", timestamp_converted);

    FILE *block2Timestamp; 
    block2Timestamp = fopen("block2Output.txt","w+b");
    fprintf(block2Timestamp, "%d", (int)block2->timestamp);

    rewind(bufferTimestamp);
    rewind(block2Timestamp);

    int a = 0; char b;
    while((b=fgetc(bufferTimestamp))!=EOF) {
        temp1_timestamp[a] = b;
        a++;
    }
    int c = 0; char d;
    while((d=fgetc(block2Timestamp))!=EOF) {
        temp2_timestamp[c] = d;
        c++;
    }

    int retTimestamp = memcmp(temp1_timestamp, temp2_timestamp, 8);
    if(retTimestamp != 0) {
        printf("Valation failed! Error with timestamp!\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    /* validate target */
    fseek(outputBlock, 17, SEEK_CUR);
    fgets(buffer_target, BUFFER, outputBlock);
    FILE *bufferTarget; 
    bufferTarget = fopen("bufferOutput.txt","w+b");
    double target_converted = atof(buffer_target);
    fprintf(bufferTarget, "%f", (double)target_converted);

    FILE *block2Target; 
    block2Target = fopen("block2Output.txt","w+b");
    fprintf(block2Target, "%f", (double)block2->target);

    rewind(bufferTarget);
    rewind(block2Target);

    int e = 0; char f;
    while((f=fgetc(bufferTarget))!=EOF) {
        temp1_target[e] = f;
        e++;
    }
    int g = 0; char h;
    while((h=fgetc(block2Target))!=EOF) {
        temp2_target[g] = h;
        g++;
    }

    int retTarget = memcmp(temp1_target, temp2_target, 8);
    if(retTimestamp != 0) {
        printf("Validation Failed! Error with target\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    /* validate nonce */
    fseek(outputBlock, 10, SEEK_CUR);
    fgets(buffer_nonce, BUFFER, outputBlock);
    FILE *bufferNonce; 
    bufferNonce = fopen("bufferOutput.txt","w+b");
    int nonce_converted = atoi(buffer_nonce);
    fprintf(bufferNonce, "%d", (int)nonce_converted);

    FILE *block2Nonce; 
    block2Nonce = fopen("block2Output.txt","w+b");
    fprintf(block2Nonce, "%d", (int)block2->nonce);

    rewind(bufferNonce);
    rewind(block2Nonce);
    int k = 0; char z;
    while((z=fgetc(bufferNonce))!=EOF) {
        temp1_nonce[k] = z;
        k++;
    }
    int m = 0; char n;
    while((n=fgetc(block2Nonce))!=EOF) {
        temp2_nonce[m] = n;
        m++;
    }

    int retNonce = memcmp(temp1_nonce, temp2_nonce, 9);
    if(retNonce != 0) {
        printf("Validation failed! Error with nonce\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    return 1;
}

/* validate merkle tree of a block */
int validate_merkle_tree(Block2 *block2, char actualFileNameMerkleTree[]) {
    /* open file associated with outputMerkleTree file handler */
    FILE *outputMerkleTree; 
    outputMerkleTree = fopen(actualFileNameMerkleTree,"r");

    int bufferCount[1]; 
    char *compareBuffer = (char *)malloc(BUFFER);
    fgets(bufferCount, sizeof(int), outputMerkleTree);
    int buffer_counter = atoi(bufferCount);
    printf("buffer_converter is: %d\n", (int)buffer_counter);
    char **buffer_value = (char **)malloc(sizeof(char *));
    for (int i = 0; i < buffer_counter; i++){
        buffer_value[i] = (char *)malloc(BUFFER);
        fgets(buffer_value[i], BUFFER, outputMerkleTree);
    }

    /* createLeafNodes, convertLeaftoInternal, merkleTreeRoot -- 3 function calls */
    InternalNode *internalNode = (InternalNode *)malloc(buffer_counter * sizeof(InternalNode)); 
    LeafNode *leafNodes = (LeafNode *)malloc(buffer_counter * sizeof(LeafNode));
    createLeafNodes(leafNodes, buffer_value, buffer_counter);
    convertLeaftoInternal(internalNode, leafNodes, buffer_counter);
    InternalNode *root = merkleTreeRoot(internalNode, buffer_counter); 
    return compareMerkleTree(root, outputMerkleTree, compareBuffer,1);
}

/* validation of the merkletree associated with a block */
int compareMerkleTree(InternalNode *root, FILE *fp, char *compareBuffer, int ID) {
    /* same code-base as in printMerkleTree.c, except modified for validation rather than printing */
    int len;
    fgets(compareBuffer, BUFFER, fp);
    len = strlen(compareBuffer);
    if( compareBuffer[len-1] == '\n') {
        compareBuffer[len-1] = '\0';
    }
    int compareBuffer_int1 = atoi(compareBuffer + 7);
    if (compareBuffer_int1 != ID) {
        printf("validation failed at ID\n");
    }
    clearBuffer(compareBuffer);

	if(root->leftChild != NULL || root->rightChild != NULL){
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        if(!memcmp(compareBuffer+14, root->leftEdge, 100)){
            printf("validation failed at leftEdge\n");
            return 0;
        }
        clearBuffer(compareBuffer);
	}

    fgets(compareBuffer, BUFFER, fp);
    len = strlen(compareBuffer);
    if( compareBuffer[len-1] == '\n') {
        compareBuffer[len-1] = '\0';
    }
    if(!memcmp(compareBuffer+9, root->hash, SHA256_BLOCK_SIZE)){
        printf("validation failed at hash\n");
        return 0;
    }
    clearBuffer(compareBuffer);

	if(root->leftChild != NULL || root->rightChild != NULL){
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        if(!memcmp(compareBuffer+15, root->rightEdge, 100)){
            printf("validation failed at rightEdge\n");
            return 0;
        }
        clearBuffer(compareBuffer);
	}else{
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        if(!memcmp(compareBuffer+19, root->leftEdge, 100)){
            printf("validation failed at leftEdge 146\n");
            return 0;
        }
        clearBuffer(compareBuffer);

        fgets(compareBuffer, BUFFER, fp);
        clearBuffer(compareBuffer);
	}

	if (root->rightChild != NULL) {
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        int compareBuffer_int2 = atoi(compareBuffer + 14);
        if(compareBuffer_int2 != 2*ID){
            printf("validation failed at left child ID 164\n");
            return 0;
        }
        clearBuffer(compareBuffer);

        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] == '\0';
        }
        int compareBuffer_int3 = atoi(compareBuffer + 15);
        if(compareBuffer_int3 != 2*ID+1){
            printf("validation failed at right child ID 176\n");
            return 0;
        }
        clearBuffer(compareBuffer);
        
        compareMerkleTree(root->leftChild, fp, compareBuffer, 2*ID);

        compareMerkleTree(root->rightChild, fp, compareBuffer, 2*ID+1);

	}else if (root->leftChild != NULL) {
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        int compareBuffer_int4 = atoi(compareBuffer + 14);
        if(compareBuffer_int4 != 2*ID){
            printf("validation failed at left child ID 194\n");
            return 0;
        }
        clearBuffer(compareBuffer);

        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        int compareBuffer_int5 = atoi(compareBuffer + 15);
        if(compareBuffer_int5 != 0){
            printf("validation failed at right child ID 206\n");
            return 0;
        }
        clearBuffer(compareBuffer);

        compareMerkleTree(root->leftChild, fp, compareBuffer, 2*ID);
	}
    return 1;
}

/* clear buffer for re-use */
void clearBuffer(char *buffer) {
    for(int i = 0 ; i < BUFFER; i++){
        buffer[i] = NULL;
    }
}
/* IMPLEMENTATION FOR SEARCHING FOR STRING IN A BLOCKCHAIN */

#include "indexStructure.h"
#define BUFFER 100

/* inchain provides proof of membership */
int inchain(char search_block[], char search_string[]) {
    char buffer[BUFFER];
    FILE *outputBlock; 
    outputBlock = fopen(search_block,"rb");

    while (fgets(buffer, BUFFER, outputBlock) != NULL) {
        if ((strstr(buffer, search_string)) != NULL) {
            return 1;
        }
        clear_buffer(buffer);
    }
    return 0;
}

/* clear buffer for re-use */
void clear_buffer(char buffer[]) {
    for(int i = 0 ; i < BUFFER; i++){
        buffer[i] = NULL;
    }
}


/* HEADER FILE INDEXSTRUCTURE.C */

#ifndef INDEXSTRUCTURE_DEF
#define INDEXSTRUCTURE_DEF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* function prototypes for proof of membership */
int inchain(char [], char []);
void clear_buffer(char []);

#endif
prog: main.o merkleTree.o readFile.o sha256.o hash.o sort.o block.o printMerkleTree.o printBlock.o serialize.o validation.o indexStructure.o
	gcc -g -o prog merkleTree.o readFile.o sha256.o hash.o block.o sort.o printMerkleTree.o printBlock.o serialize.o validation.o indexStructure.o main.o

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

validation.o: validation.h validation.c
	gcc -g -Wall -Wextra -Wwrite-strings -c validation.c

indexStructure.o: indexStructure.h indexStructure.c
	gcc -g -Wall -Wextra -Wwrite-strings -c indexStructure.c

clean:
	rm -f prog
	rm -f *.o

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