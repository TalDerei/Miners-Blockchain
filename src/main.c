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
	//scanf("%d",&count);	//make sure user can ONLY enter an int (not char) while handling any potential errors
    count = 1;
    printf("number of files entered: %d\n", count); 

    char **fileNames = (char **)malloc(count * sizeof(FILE *));
    for(int i = 0; i < count; i++){
        fileNames[i] = (char *)malloc(BUFFER);
    }
    fileNames[0] = "input.txt";
    //fileNames[1] = "input2.txt";
    
    // for (int i = 0; i < count; i ++){
    //     printf("enter the filename[%d]: ", i); 
    //     scanf("%s", fileNames[i]);
        // fileNames[0] = "input.txt";
        // fileNames[1] = "input2.txt";

        //int k = strlen(fileNames[i]);
        /*for (int j = 0; j < k; j++) {
            printf("Filename entered: %c \n", fileNames[i][j]);
        }*/
    //     printf("Filename entered: %s \n", fileNames[i]);
    // }
    int * lineNum = malloc(count * sizeof(int));
    lineNum = GetLineNumbers(fileNames, count);
    for(int i = 0 ; i < count; i++){
        printf("########lineNum at %d is: %d\n", i, lineNum[i]);
    }

    char **arr = (char **)malloc(100* sizeof(char*));
    for (int i = 0; i < 100; i++)
    {
        arr[i] = malloc(100);
    }

    //output_blockchain = serialization of the blockchain
    char output_fileName[] = "output.blockchain.txt";
    FILE *output_blockchain = fopen(output_fileName,"wb"); //b = open file for writing in binary formate

    //outputMerkleTree for printing merkle tree
	FILE *outputMerkleTree; 
	outputMerkleTree = fopen("output.merkletree.txt","w"); 

    //outputBlock for printing block
    FILE *output_block; 
	output_block = fopen("output.block.txt","w");

    unsigned char *pointerToZero = (unsigned char *)malloc(sizeof(char));
    strncpy(pointerToZero, "0", 1);
    Block **block = (Block **)malloc(count * sizeof(Block *));
    LeafNode **leafNodes = (LeafNode **)malloc(count * sizeof(LeafNode *));
    InternalNode **internalNode = (InternalNode **)malloc(count *sizeof(InternalNode *));
    InternalNode **TreeRoot = (InternalNode **)malloc(count * sizeof(InternalNode *));
    for(int i = 0; i < count; i++){
        ReadOneFile(arr, fileNames[i]);
        leafNodes[i] = (LeafNode *)malloc(lineNum[i] * sizeof(LeafNode));
        block[i] = (Block *)malloc(sizeof(Block)); //mallocing for block (pointer to root and header) -- 16 Bytes
        block[i]->header = malloc(sizeof(Header)); //mallocing for header contents
        createLeafNodes(leafNodes[i], arr, lineNum[i]);
        for (int k = 0; k < lineNum[i]; k++) {
            for (int j = 0; j < SHA256_BLOCK_SIZE; j++) {
                if ((unsigned char)leafNodes[i][k].hash[j] <= 0x0f) {
                    printf("0%x", (unsigned char) leafNodes[i][k].hash[j]);
                }else{
                    printf("%x", (unsigned char) leafNodes[i][k].hash[j]);
                }
            }
            printf("\n");
        }
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
            printf("CREATE BLOCK: \n");
            create_block(block[i], TreeRoot[i], block[i-1], output_block, output_blockchain);
        }else{
            printf("INITIALIZE BLOCK: \n");
            initialize_block(block[i], TreeRoot[i], pointerToZero, output_block, output_blockchain); //first block, previous block pointing to 0
        }

        //reset arr for re-use
        for(int i = 0 ; i < 100; i++){
            for( int j = 0; j < 100; j++){
                arr[i][j] = NULL;
            }
        }
    }

    Block2 **block2= (Block **)malloc(count * sizeof(Block2 *));
    for( int i = 0; i < count ; i ++){
        block2[i] = (Block *) malloc(sizeof(Block2));
    }
    //rebuild_block(output_fileName, block_count, block2);

    //Fclose(output);
    //free_merkle_tree(leafNodes);
    //print_block(block, count, fileNames);
}

