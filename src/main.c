/* ROOT MAIN.c 
CONTROL FLOW: (1) main will call function (e.g. read_input_file() inside inputFile.c to read-in 
		     input file (plaintext) from user, and sort the file's contents with an algorithm 
		     with efficient time complexity
		 (2) main will then call function (e.g. build_tree() inside of merkleTree.c) to build
		     an empty merkle tree.
		 (3) main will then call a function (e.g. populate_tree() inside of merkleTree.c) in order
		     to start populating the tree with the set of string inputted by the user.
		 (4) From merkleTree.c, we'll have to call the hash function (e.g. hash() inside of sha_256.c)
		     in order to hash the input, and then placing the hash inside the specified node
		 (5) main will call a function (printTree() inside of merkleTree.c) in order to print out the tree
		 (6) main will call a function (freeTree() inside of merkleTree.c) in order to free memory
		     allocated to the merkleTree and its nodes.  
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merkleTree.h"
#include "block.h"
#include "sha256.h"
#include "readFile.h"
#include "sort.h"
#include "hash.h"
#define BUFFER 100

int main(int argc, char *argv[]) {
    int count;
    printf("input the number of files you want opened: "); 
	scanf("%d",&count);	//make sure user can ONLY enter an int (not char) while handling any potential errors
    printf("number of files entered: %d\n", count); 

    char **fileNames = (char **)malloc(count * sizeof(FILE *));
    for(int i = 0; i < count; i++){
        fileNames[i] = (char *)malloc(BUFFER);
    }
    for (int i = 0; i < count; i ++){
        printf("enter the filename[%d]: ", i); 
        scanf("%s", fileNames[i]);
        //int k = strlen(fileNames[i]);
        /*for (int j = 0; j < k; j++) {
            printf("Filename entered: %c \n", fileNames[i][j]);
        }*/
        printf("Filename entered: %s \n", fileNames[i]);
    }
    char ** arr = malloc(count * sizeof(FILE *)); //malloc count * 8 BYTES 
    arr = ReadMultipleFiles(fileNames, count);
    int * lineNum = malloc(count * sizeof(int));
    lineNum = GetLineNumbers(fileNames, count);
    for(int i = 0 ; i < count; i++){
        printf("########lineNum at %d is: %d\n", i, lineNum[i]);
    }

    Block **block = (Block **)malloc(count * sizeof(Block *));
    LeafNode **leafNodes = (LeafNode **)malloc(count * sizeof(LeafNode *));
    InternalNode **internalNode = (InternalNode **)malloc(count *sizeof(InternalNode *));
    InternalNode **TreeRoot = (InternalNode **)malloc(count * sizeof(InternalNode *));
    for(int i = 0; i < count; i++){
        leafNodes[i] = (LeafNode *)malloc(count*sizeof(LeafNode));
        block[i] = (Block *)malloc(sizeof(Block));//keep this here
        createLeafNodes(leafNodes[i], arr[i], lineNum[i]);
        printf("\nnoobs2.0\n");
        for (int k = 0; k < lineNum[i]; k++) {
            for (int j = 0; j < SHA256_BLOCK_SIZE; j++) {
                printf("%x", (unsigned char) leafNodes[i][k].hash[j]);
            }
            printf("\n");
        }
        internalNode[i] = malloc(lineNum[i]*sizeof(InternalNode));
        convertLeaftoInternal(internalNode[i], leafNodes[i],lineNum[i]);
        TreeRoot[i] = malloc(sizeof(InternalNode));
        TreeRoot = merkleTreeRoot(internalNode[i],lineNum[i]);
        printf("\n-------root is: ----\n");
        for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
            printf("%x", (unsigned char) TreeRoot[i]->hash[n]);
        }
        printf("\n");
        print_merkle_tree(TreeRoot, 1);
        //create block
        if(i != 0){
            block[i] = create_block(TreeRoot,block[i-1]);
        }else{
            block[i] = initialize_block(TreeRoot); //first block, previous block pointing to 0
        }
    }
        //FILE *output = Fopen(strncat(output,".out.txt", 1), "w");    
        //Fclose(output);
        //free_merkle_tree(leafNodes);
    //print_block(block, count, fileNames);
}

