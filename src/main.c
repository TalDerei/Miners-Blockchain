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
    for (int i = 0; i < count; i ++){
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
    char output_fileName[] = "serialization.blockchain.txt";
    FILE *output_blockchain = fopen(output_fileName,"wb");

    /* actualFileNameMerkleTree and actualFileNameBlock are arrays storing file names for merkleTree and Block filenames */
    char actualFileNameMerkleTree[count][255]; 
    char actualFileNameBlock[count][255]; 
    for(int i = 0 ; i < count; i++){
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
    InternalNode **internalNode = (InternalNode **)malloc(count *sizeof(InternalNode *));
    InternalNode **TreeRoot = (InternalNode **)malloc(count * sizeof(InternalNode *));
    for(int i = 0; i < count; i++){
        int fileNameCounter = strlen(fileNames[i]) - 4;
        for(int j = 0; j < fileNameCounter; j++){
            actualFileNameMerkleTree[i][j] = fileNames[i][j];
        }
        strcat(actualFileNameMerkleTree[i], ".out.txt"); /* contains the output for merkle tree */

        /* outputMerkleTree = file containing the contents of a merkle tree associatd with block[i] */
        FILE *outputMerkleTree; 
        outputMerkleTree = fopen(actualFileNameMerkleTree[i],"w"); 
        for(int j = 0; j < fileNameCounter; j++){
            actualFileNameBlock[i][j] = fileNames[i][j];
        }
        strcat(actualFileNameBlock[i], ".block.txt"); /* contains the output for the entire block contents */
        
        /* output_block = file containing the contents of a block's header contents + merkle tree associated with block[i] */
        FILE *output_block; 
        output_block = fopen(actualFileNameBlock[i],"w");

        ReadOneFile(arr, fileNames[i]);
        leafNodes[i] = (LeafNode *)malloc(lineNum[i] * sizeof(LeafNode)); 
        block[i] = (Block *)malloc(sizeof(Block));                                  /* malloc for block[i] */
        block[i]->header = (Header *)malloc(sizeof(Header));                        /* mallocing for header contents */
        createLeafNodes(leafNodes[i], arr, lineNum[i]);
        internalNode[i] = (InternalNode *)malloc(lineNum[i] * sizeof(InternalNode));
        convertLeaftoInternal(internalNode[i], leafNodes[i],lineNum[i]);
        TreeRoot[i] = (InternalNode *)malloc(sizeof(InternalNode));
        TreeRoot[i] = merkleTreeRoot(internalNode[i],lineNum[i]);

        /* printing out the merkle tree */
        print_merkle_tree_value(arr, lineNum[i] , outputMerkleTree);    	
        print_merkle_tree(TreeRoot[i], 1, outputMerkleTree); 

        if(i != 0){
            /* add another block to the blockchain */
            create_block(block[i], TreeRoot[i], block[i-1], output_block, output_blockchain, pointerToZero);
        }else{
            /* add first block to the blockchain */
            initialize_block(block[i], TreeRoot[i], pointerToZero, output_block, output_blockchain);
        }

        /* reset the contents of the array for re-use */
        for(int i = 0 ; i < 100; i++){
            for( int j = 0; j < 100; j++){
                arr[i][j] = NULL;
            }
        }
    }

    fclose(output_blockchain);
    
    /* inserting serialized data into Block2 structure to rebuild blocks from disk */
    FILE *write_blockchain2 = fopen(output_fileName,"rb");
    Block2 **block2= (Block **)malloc(count * sizeof(Block2 *));
    for( int i = 0; i < count ; i ++) {
        block2[i] = (Block2 *)malloc(sizeof(Block2));
        if(i == 0){
            rebuild_first_block(write_blockchain2, block2[i]);
        }else{
            rebuild_block(write_blockchain2, block2[i]);
        }
    }

    /* Fclose(output_blockchain); */
    /* Fclose(write_blockchain2); */
    /* free_merkle_tree(leafNodes); */
}

