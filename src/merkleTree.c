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
		printf("LeafNode value \n%s\n", leafnode[i].value);
		//unsigned char *returned_str = hash(SortedArray[i]);
		/* for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
       		printf("%x", (unsigned char)(returned_str[n]));
			//printf("LeafNode hash \n %x\n", leafnode[i].hash); 
		}  */
		/*for (int j = 0; j < SHA256_BLOCK_SIZE; j++) { //wtf, cannot 
			strcat(leafnode[i].hash, (unsigned char)(returned_str[j]));
		}*/
        //strcpy(leafnode[i].hash, *returned_str);//will need to apply the hash function here
		strcpy(leafnode[i].hash, SortedArray[i]);
		printf("\nLeafNode hash \n %x\n", leafnode[i].hash);
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
		printf("\nI = : %d\n",i);
		if(i != count-1){
			char *temp = &(leafNodes[i].hash);
			printf("\ntemp is: %s\n" , temp);
			printf("\ni+1 is: %s\n" , leafNodes[i+1].hash);
			strcpy(newInternal[j].hash, strcat(temp, leafNodes[i+1].hash));
			printf(".....temp after strcat is: %s", temp);
			printf("\nStrcat test: %s\n",newInternal[j].hash);
			//printf("dereference of newinternal is: *s compare to the origional: %s",*newInternal[j].hash);
			newInternal[j].leftChild = &leafNodes[i].hash;
			newInternal[j].rightChild = &leafNodes[i+1].hash;
			strcpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge);
			printf("\n---------The left edge is: %s\n",newInternal[j].leftEdge);
			strcpy(newInternal[j].rightEdge, &leafNodes[i+1].rightEdge);
			printf("\n---------The right edge is: %s\n",newInternal[j].rightEdge);
		}else{
			char *temp = &(leafNodes[i].hash);
			printf("\nZZZZZZZZZZZ ODD CASE: temp is: %s\n",temp);
			strcpy(newInternal[j].hash,temp);//will need to apply the hash function here
			newInternal[j].leftChild = &leafNodes[i];
			newInternal[j].rightChild = NULL;
			strcpy(newInternal[j].rightEdge, &leafNodes[i].rightEdge);
			strcpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge);
			printf("\n---------The right edge is: %s\n",newInternal[j].rightEdge);
		}
		j++;
		if(parents == 1){
			printf("\n*****Root is: %s\n", newInternal[0].hash);
			printf("\n*****Root left edge  is: %s\n", newInternal[0].leftEdge);
			printf("\n*****Root right edge  is: %s\n", newInternal[0].rightEdge);
			return newInternal;
		}
	}
	return merkleTreeRoot(newInternal, parents);
}

void print_merkle_tree(InternalNode *root, int ID) {
	printf("\n ------------------------- leafNodes are: %s \n", root->hash[0]);
	printf(root->leftChild);
	if (root->leftChild != NULL) {
		print_merkle_tree(root->leftChild, 2*ID);
	}
	if (root->rightChild != NULL) {
		print_merkle_tree(root->rightChild, 2*ID+1);
	}
} 

/* free_merkle_tree(InternalNode *internalNodes) {
	free(internalNodes);
} */
