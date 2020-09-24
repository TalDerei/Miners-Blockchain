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
#include "sha256.h"
#include "node.h"
#define BUFFER 100

int main(int argc, char *argv[]) {
    char input[BUFFER];
    printf("input the filename to be opened : ");
	scanf("%s",input);	
    
    FILE *fp = Fopen(input, "r"); 
    int count = (int) lineCount(fp);
    printf("number of lines: %d\n", count);
    char** arr = malloc(count * sizeof(char*));
    for(int i = 0; i < count; i++){
      arr[i] = malloc(100);
    }

    fp = Fopen(input, "r"); //open the file
    int z = 0;
    while (fgets(arr[z], 100, fp) != NULL) {
        size_t len = strlen(arr[z]);
        if( arr[z][len-1] == '\n') {
            arr[z][len-1] = '\0';
        }
        printf ("elements are: %s", arr[z]);
        z++;
    }
    printf("Total string put in arr is: %d\n",z);
    Fclose(fp);

    Sort(arr, count);

    for(int i = 0; i < count; i++){
      printf("Sorted array is: %s\n", arr[i]);
    }

    LeafNode *leafNodes = malloc(count*sizeof(LeafNode));
    createLeafNodes(leafNodes, arr, count);
    InternalNode *internalNode = malloc(count*sizeof(InternalNode));
    internalNode = convertLeaftoInternal(leafNodes,count);
    InternalNode *TreeRoot = malloc(sizeof(InternalNode));
    TreeRoot = merkleTreeRoot(internalNode,count);
    printf("\n**The returned value from merkleTreeRoot is %s\n",TreeRoot);
    //print_merkle_tree(TreeRoot, 1);


    //FILE *output = Fopen(strncat(output,".out.txt", 1), "w");    
    //Fclose(output);
    //free_merkle_tree(leafNodes);
}

