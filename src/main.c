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
        output_block = fopen(actualFileNameBlock[i],"wb");

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
            if ((unsigned char)TreeRoot[i]->hash[n] <= 0x0f) {
                    printf("0%x", (unsigned char)TreeRoot[i]->hash[n]);
            } else{
                    printf("%x", (unsigned char)TreeRoot[i]->hash[n]);
            }
        }

		// FILE *output = fopen(strncat(output,".block.out", 1), "w");
        print_merkle_tree_value(arr, lineNum[i] , outputMerkleTree);
    	print_merkle_tree(TreeRoot[i], 1, outputMerkleTree); //print merkle tree
        fclose(outputMerkleTree);
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
    for(int i = 0; i < count ; i ++) {
        block2[i] = (Block2 *) malloc(sizeof(Block2));
        if(i == 0){
            rebuild_first_block(write_blockchain2, block2[i]);
        }else{
            rebuild_block(write_blockchain2, block2[i]);
        }
    }

    //add count to differentiate between first block2 
    int validation_result; 
    //validation goes here:
    for (int i = 0; i < count; i++) {
        // printf("actualFileNameMerkleTree: %s\n", actualFileNameMerkleTree[i]);
        // printf("actualFileNameBlock: %s\n", actualFileNameBlock[i]);
        validation_result = validation(block2[i], actualFileNameMerkleTree[i], actualFileNameBlock[i], i);
        printf("\nvalidation_result is: %d\n", validation_result);
        //exit(0);
        //printf("%d", validation_result);
    }

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



    //check the header contents, except the previous hash
    //check the merkle tree -- comparing good merkle tree (in file) to bad merkle tree (in another file)
    //compare line 11 in merkletree.txt to root hash in block.txt -- immutability
    
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

