/* IMPLEMENTATION OF MERKLE TREE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "merkleTree.h"
#include "sha256.h"
#include "hash.h"

void createLeafNodes(LeafNode *leafnode, char **SortedArray, int count) {
	for(int i = 0; i < count; i++){
		printf("inside createLeafNodes\n");
		printf("Sorted Array is: %s\n", SortedArray[i]);     
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
		printf("!!!!!!!!!!leafNode[i] is: %s", LeafNode[i].value);
		memcpy(newInternal[i].hash, &LeafNode[i].hash, SHA256_BLOCK_SIZE);
		strncpy(newInternal[i].leftEdge, &LeafNode[i].value, 100);
		printf("\n*******************Left edge assigned is: %s\n", newInternal[i].leftEdge);
		strncpy(newInternal[i].rightEdge, &LeafNode[i].value, 100);
		printf("\n*******************Right edge assigned is: %s\n", newInternal[i].rightEdge);
		newInternal[i].leftChild = NULL;
		newInternal[i].rightChild = NULL;
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
			unsigned char *temp = (unsigned char *) malloc(2*SHA256_BLOCK_SIZE);
			printf("\nhash[i] is:");
			for	 (int n = 0; n < SHA256_BLOCK_SIZE; n ++) {
				printf("%x", (unsigned char) leafNodes[i].hash[n]);
			}
			printf("\n");
			memcpy(temp, leafNodes[i].hash, SHA256_BLOCK_SIZE);
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
			unsigned char *tempHash = hash(memcpy(temp+SHA256_BLOCK_SIZE, leafNodes[i+1].hash, SHA256_BLOCK_SIZE));
			memcpy(newInternal[j].hash, tempHash, SHA256_BLOCK_SIZE); //memory shift by SHA256_BLOCK_SIZE
			printf(".....temp after strcat is\n");
			for (int n = 0; n < 2*SHA256_BLOCK_SIZE; n++) {
				if ((unsigned char)temp[n] <= 0x0f) {
						printf("0%x", temp[n]);
				}else{
						printf("%x", temp[n]);
				}
			}
			printf("\nhash of the concatenation test: \n");
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				if ((unsigned char)newInternal[i].hash[j] <= 0x0f) {
						printf("0%x", (unsigned char) newInternal[j].hash[n]);
				}else{
						printf("%x", (unsigned char) newInternal[j].hash[n]);
				}
			}
			// for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
			// 	printf("%x", (unsigned char) newInternal[j].hash[n]);
			// }
			newInternal[j].leftChild = &leafNodes[i].hash;
			newInternal[j].rightChild = &leafNodes[i+1].hash;
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
			printf("\n---------The left edge is: %s\n",newInternal[j].leftEdge);
			strncpy(newInternal[j].rightEdge, &leafNodes[i+1].rightEdge, 100);
			printf("\n---------The right edge is: %s\n",newInternal[j].rightEdge);
		}else{
			unsigned char *temp = &(leafNodes[i].hash);
			printf("\nZZZZZZZZZZZ ODD CASE: temp is:\n");
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				printf("%x", (unsigned char) temp[n]);
			}
			printf("\n");
			memcpy(newInternal[j].hash, hash(hash(temp)), SHA256_BLOCK_SIZE);
			for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
				printf("%x", (unsigned char) newInternal[j].hash[n]);
			}
			newInternal[j].leftChild = &leafNodes[i];
			newInternal[j].rightChild = NULL;
			strncpy(newInternal[j].rightEdge, &leafNodes[i].rightEdge, 100);
			printf("\n!!!!!!!!!!!!Left edge assigned is: %s\n", newInternal[j].leftEdge);
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
			printf("\n!!!!!!!!!!!!Right edge assigned is: %s\n",newInternal[j].rightEdge);
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
	printf("\n");
	printf("%s", root->leftEdge);
	printf("\n");
	printf("%s", root->rightEdge);
	printf("\n");
	printf("\n");
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
