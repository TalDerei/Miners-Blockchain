/* IMPLEMENTATION OF MERKLE TREE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "merkleTree.h"
#include "sha256.h"
#include "hash.h"

void createLeafNodes(LeafNode *leafnode, char *SortedArray, int count) {
	for(int i = 0; i < count; i++){
		printf("inside createLeafNodes\n");
		printf("Sorted Array is: %s\n", SortedArray);
		exit(0);
        strcpy(leafnode[i].value, SortedArray[i]);//correct
		printf("LeafNode value: %s\n", leafnode[i].value);//correct
		unsigned char *returned_str = hash(SortedArray[i]);
		for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
       		printf("%x", returned_str[n]);
		}   
		printf("**************\n");
		memcpy(leafnode[i].hash, returned_str, SHA256_BLOCK_SIZE);
		for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
			printf("%x", (unsigned char) leafnode[i].hash[n]);
		}
		printf("***********************\n");
		//strcpy(leafnode[i].hash, SortedArray[i]);
		//printf("\nLeafNode hash: %s\n", leafnode[i].hash);
    }
}

void convertLeaftoInternal(InternalNode *newInternal, LeafNode *LeafNode, int count){
	for(int i = 0; i < count; i++){
		memcpy(newInternal[i].hash, &LeafNode[i].hash, SHA256_BLOCK_SIZE);
		strncpy(newInternal[i].leftEdge, &LeafNode[i].value, 100);
		strncpy(newInternal[i].rightEdge, &LeafNode[i].value, 100);
	}
}

InternalNode *merkleTreeRoot(InternalNode *leafNodes, int count){
	//get ceiling
	printf("********Count is: %d\n",count);
	int parents = count/2 + count%2;
	InternalNode *newInternal = malloc(((count/2)+(count%2))*sizeof(InternalNode));
	
	int j = 0;
	for(int i = 0; i < count; i+=2){
		printf("\nI = %d\n",i);
		if(i != count-1){
			unsigned char *temp = (unsigned char *) malloc(2*SHA256_BLOCK_SIZE+1);
			printf("\nhash[i] is:");
			for	 (int n = 0; n < SHA256_BLOCK_SIZE; n ++) {
				printf("%x", (unsigned char) leafNodes[i].hash[n]);
			}
			printf("\n");
			memcpy(temp, leafNodes[i].hash,SHA256_BLOCK_SIZE);
			printf("\ntemp is:");
			for (int n = 0; n < SHA256_BLOCK_SIZE; n ++) {
				printf("%x", (unsigned char) temp[n]);
			}
			printf("\n");
			printf("\ni+1 is:\n");
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				printf("%x", (unsigned char) leafNodes[i+1].hash[n]);
			}
			printf("\n");

			strncpy(newInternal[j].hash, hash(strcat(temp, leafNodes[i+1].hash)), SHA256_BLOCK_SIZE);
			printf(".....temp after strcat is");
			for (int n = 0; n < 2*SHA256_BLOCK_SIZE; n++) {
				printf("%x", temp[n]);
			}
			printf("\nStrcat test: \n");
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				printf("%x", (unsigned char) newInternal[j].hash[n]);
			}
			newInternal[j].leftChild = &leafNodes[i].hash;
			newInternal[j].rightChild = &leafNodes[i+1].hash;
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
			//printf("\n---------The left edge is: %s\n",newInternal[j].leftEdge);
			strncpy(newInternal[j].rightEdge, &leafNodes[i+1].rightEdge, 100);
			//printf("\n---------The right edge is: %s\n",newInternal[j].rightEdge);
		}else{
			unsigned char *temp = &(leafNodes[i].hash);
			printf("\nZZZZZZZZZZZ ODD CASE: temp is:\n");
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				printf("%x", (unsigned char) temp[n]);
			}
			printf("\n");
			strncpy(newInternal[j].hash, hash(temp), SHA256_BLOCK_SIZE);
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				printf("%x", (unsigned char) newInternal[j].hash[n]);
			}
			newInternal[j].leftChild = &leafNodes[i];
			newInternal[j].rightChild = NULL;
			strncpy(newInternal[j].rightEdge, &leafNodes[i].rightEdge, 100);
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
			//printf("\n---------The right edge is: %s\n",newInternal[j].rightEdge);
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

void print_merkle_tree(InternalNode *root, int ID) {

	printf("#######");
	printf("%d", ID);
	printf("%s", root->leftEdge);
	printf("%s", root->rightEdge);
	if (root->leftChild != NULL) {
		print_merkle_tree(root->leftChild, 2*ID);
	}
	if (root->rightChild != NULL) {
		print_merkle_tree(root->rightChild, 2*ID+1);
	}
}

/*void print_internal_node(){

}*/

/* free_merkle_tree(InternalNode *internalNodes) {
	free(internalNodes);
} */
