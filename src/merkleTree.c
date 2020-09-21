/* IMPLEMENTATION OF MERKLE TREE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "node.h"
#include "merkleTree.h"
#include "sha_256.h"
#define BUFFER 100

LeafNode *createLeafNodes(LeafNode *leafnode, char *SortedArray[], int count) {
	for(int i = 0; i < count; i++){
        leafnode[i].value = SortedArray[i];
        leafnode[i].hash = SortedArray[i];
    }
    for(int i = 0; i < count; i++) {
        printf("LeafNode value \n%c\n", leafnode[i].value);
        printf("LeafNode hash \n%zu\n", leafnode[i].hash);
    }
    return leafnode;
}

/*InternalNode *merkleTreeRoot(LeafNode *leafNodes[], int count){
	//get ceiling
	leafNodes = (LeafNode *)malloc(count * sizeof(LeafNode))
	InternalNode newInternal[(count/2)+(count%2)];
	LeafNode *newLeafNode[(count/2)+(count%2)];
	int parents = count/2 + count%2;

	LeafNode current = merkleTree[0];

	int j = 0;
	for(int i = 0; i < count; i+=2){
		if(i = count-1){
			size_t temp =merkleTree[i].hash;
			newInternal[j].hash = strcat(temp, merkleTree[i].hash);
			newInternal[j].leftChild = *merkleTree[i].hash;
			newInternal[j].rightChild = NULL;

		}else{
			size_t temp =merkleTree[i].hash;
			newInternal[j].hash = strcat(temp, merkleTree[i+1].hash);
			newInternal[j].leftChild = *merkleTree[i].hash;
			newInternal[j].rightChild = *merkleTree[i+1].hash;
		}
		newLeafNode[j]->hash = newInternal[j].hash;
		j++;
	}
	if(parents == 1){
		return newInternal;
	}
	merkleTreeRoot(newLeafNode, parents);
}

static void print_merkle_tree(InternalNode *root, int height, FILE *output) {
	//height of tree = # nodes / 2
	//start at root
	//print hashes of each node
	
	printf("\n leafNodes 2 are: %zu \n", root->hash);
	if (root.leftchild != NULL) {
		print_merkle_tree(root.leftchild, height - 1);
	}
	if (root.rightchild != NULL) {
		print_merkle_tree(root.rightchild, height -1);
	}
}

free_merkle_tree(InternalNode *internalNodes) {
	free(internalNodes);
}
*/