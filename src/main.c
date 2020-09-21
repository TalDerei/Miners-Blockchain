/* ROOT MAIN.c */

/* 
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


** Haven't defined the specific functions or their inputs yet **
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merkleTree.h"
#define BUFFER 100

/* int main(int argc, char *argv[]) {
    char element1[100] = "aa";
    char element2[100] = "bb";
    char element3[100] = "cc";
    char* newArray[3] = {element1,element2,element3};
    printf("This is the input array: \n%s\n%s\n%s\n", newArray[0],newArray[1],newArray[2]);
    LeafNode *newLeafNode = merkletree(newArray, 3);
    } */

int main(int argc, char *argv[]) {
    char *inputFile = "input.txt"; //ptr to user-input on the console
    FILE *fp = NULL; //ptr to the OPENED file
    char* input = malloc(255);
    printf("input the filename to be opened : ");
	scanf("%s",input);	
    fp = Fopen(input, "r"); 
    int count = lineCount(fp);
    printf("number of lines: %d\n", count);
    char** arr = malloc(100 * sizeof(char));
    for(int i = 0; i < count; i++){
      arr[i] = malloc(100);
    }
    fp = Fopen(input, "r"); //open the file
    int z = 0;
    while (fgets(arr[z], 100, fp) != NULL)
    {
        //printf ("%s", arr[z]);
        z++;
    }
    printf("size of array buffer %lu\n", sizeof(arr)); //array of elements*/
    printf("number of elements %d\n", (sizeof(arr) / 100)); /*array of elements*/
    fclose(fp);
    //printf("contents: %x \n", &arr[4]);
    char *ptrArray[count];
    int j = 0;
    for(int i = 0; i < count; i++) {
        ptrArray[i] = arr[i];
        printf("\n contents: %s \n", arr[i]);
    }

    for(int i = 0; i < count; i++) {
        printf("\n ptr array contents: %s \n", ptrArray[i]);
    }
    //Above is all the readfile content. 

    LeafNode *newleaf = merkletree(ptrArray, count);
    merkleTreeRoot(newleaf,count);
}