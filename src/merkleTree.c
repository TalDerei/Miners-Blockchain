/* IMPLEMENTATION OF MERKLE TREE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "node.h"
#include "merkleTree.h"
#include "sha_256.h"
#define BUFFER 100

LeafNode *createLeafNodes(LeafNode *leafnode, char **SortedArray, int count) {
	for(int i = 0; i < count; i++){
		printf("----\n");
        strcpy(leafnode[i].value, SortedArray[i]);
		printf("LeafNode hash \n%s\n", &leafnode[i].value);
        strcpy(leafnode[i].hash, SortedArray[i]);//will need to apply the hash function here
		//printf("LeafNode hash \n%lu\n", leafnode[i].hash);
    }
    return leafnode;
}

InternalNode *merkleTreeRoot(LeafNode *leafNodes, int count){
	//get ceiling
	printf("Count is: %d\n",count);
	int parents = count/2 + count%2;
	InternalNode *newInternal = malloc(((count/2)+(count%2))*sizeof(InternalNode));
	LeafNode *newLeafNode = malloc(((count/2)+(count%2))*sizeof(LeafNode));

	int j = 0;
	if(parents > 1){
		for(int i = 0; i < count; i+=2){
			if(i != count-1){
				char *temp = &(leafNodes[i].hash);
				printf("in normal case temp is: %s\n" , temp);
				printf("in normal case i+1 is: %s\n" , leafNodes[i+1].hash);
				strcpy(newInternal[j].hash, strcat(temp, leafNodes[i+1].hash));
				printf("dereference of newinternal is: *s compare to the origional: %s",*newInternal[j].hash);
				newInternal[j].leftChild = &leafNodes[i].hash;
				newInternal[j].rightChild = &leafNodes[i+1].hash;
			}else{
				char *temp = &(leafNodes[i].hash);
				printf("LeafNodes[i] is %s, temp is: %s\n",leafNodes[i].hash,temp);
				strcpy(newInternal[j].hash,temp);//will need to apply the hash function here
				newInternal[j].leftChild = &leafNodes[i];
				newInternal[j].rightChild = NULL;
			}
			strcpy(newLeafNode[j].hash, newInternal[j].hash);
			j++;
		}
	}
	if(parents == 1){
		printf("New Internal is: %s\n", newInternal[0].hash);
		printf("New Internal is: %s\n", newLeafNode[0].hash);
		free(newLeafNode);
		return newInternal;
	}
	return merkleTreeRoot(newLeafNode, parents);
	//free(newLeafNode); Do this in the main
}
 
/* static void print_merkle_tree(InternalNode *root, int height) {
	//height of tree = # nodes / 2
	//start at root
	//print hashes of each node
	printf("\n leafNodes 2 are: %s \n", root.hash);
	if (root.leftchild != NULL) {
		print_merkle_tree(root.leftchild, height - 1);
	}
	if (root.rightchild != NULL) {
		print_merkle_tree(root.rightchild, height -1);
	}
} */

/* free_merkle_tree(InternalNode *internalNodes) {
	free(internalNodes);
} */