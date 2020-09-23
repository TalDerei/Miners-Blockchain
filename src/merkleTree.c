/* IMPLEMENTATION OF MERKLE TREE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "node.h"
#include "merkleTree.h"
#include "sha256.h"
#define BUFFER 100

LeafNode *createLeafNodes(LeafNode *leafnode, char **SortedArray, int count) {
	for(int i = 0; i < count; i++){
		printf("----\n");
        strcpy(leafnode[i].value, SortedArray[i]);
		printf("LeafNode value \n%s\n", &leafnode[i].value);
        strcpy(leafnode[i].hash, SortedArray[i]);//will need to apply the hash function here
		printf("LeafNode hash \n%x\n", leafnode[i].hash);
    }
    return leafnode;
}

InternalNode *convertLeaftoInternal(LeafNode *LeafNode, int count){
	InternalNode *newInternal = malloc(count * sizeof(InternalNode));
	for(int i = 0; i < count; i++){
		strcpy(newInternal[i].hash, &LeafNode[i].hash);
		strcpy(newInternal[i].leftEdge, &LeafNode[i].value);
		strcpy(newInternal[i].rightEdge, &LeafNode[i].value);
	}
	return newInternal;
}

InternalNode *merkleTreeRoot(InternalNode *leafNodes, int count){
	//get ceiling
	printf("Count is: %d\n",count);
	int parents = count/2 + count%2;
	InternalNode *newInternal = malloc(((count/2)+(count%2))*sizeof(InternalNode));
	
	int j = 0;
	for(int i = 0; i < count; i+=2){
		if(i != count-1){
			char *temp = &(leafNodes[i].hash);
			printf("temp is: %s\n" , temp);
			printf("i+1 is: %s\n" , leafNodes[i+1].hash);
			strcpy(newInternal[j].hash, strcat(temp, leafNodes[i+1].hash));
			printf("Strcat test: %s\n",newInternal[j].hash);
			//printf("dereference of newinternal is: *s compare to the origional: %s",*newInternal[j].hash);
			newInternal[j].leftChild = &leafNodes[i].hash;
			newInternal[j].rightChild = &leafNodes[i+1].hash;
			strcpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge);
			printf("---------The left edge is: %s\n",newInternal[j].leftEdge);
			strcpy(newInternal[j].rightEdge, &leafNodes[i+1].rightEdge);
			printf("---------The right edge is: %s\n",newInternal[j].rightEdge);
		}else{
			char *temp = &(leafNodes[i].hash);
			printf("LeafNodes[i] is %s, temp is: %s\n",leafNodes[i].hash,temp);
			strcpy(newInternal[j].hash,temp);//will need to apply the hash function here
			newInternal[j].leftChild = &leafNodes[i];
			newInternal[j].rightChild = NULL;
			strcpy(newInternal[j].leftEdge, &leafNodes[i].rightEdge);
			printf("---------The right edge is: %s\n",newInternal[j].rightEdge);
		}
		j++;
		if(parents == 1){
			printf("Root is: %s\n", newInternal[0].hash);
			return newInternal;
		}
	}
	return merkleTreeRoot(newInternal, parents);
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
