/* IMPLEMENTATION OF MERKLE TREE */

#include "printMerkleTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sha256.h"
#include "hash.h"

// createLeafNodes will create the leaf nodes of merkle tree
void createLeafNodes(LeafNode *leafnode, char **SortedArray, int count) {
	for(int i = 0; i < count; i++){
		strcpy(leafnode[i].value, SortedArray[i]);
		unsigned char *returned_str = hash(SortedArray[i]);
    }
}

// convertLeaftoInternal will convert leaf nodes to internal nodes to simplify recursion
void convertLeaftoInternal(InternalNode *newInternal, LeafNode *LeafNode, int count){
	for(int i = 0; i < count; i++){
		memcpy(newInternal[i].hash, &LeafNode[i].hash, SHA256_BLOCK_SIZE);
		strncpy(newInternal[i].leftEdge, &LeafNode[i].value, 100);
		strncpy(newInternal[i].rightEdge, &LeafNode[i].value, 100);
		newInternal[i].leftChild = NULL;
		newInternal[i].rightChild = NULL;
	}
}

// merkleTreeRoot constructs merkle tree
InternalNode *merkleTreeRoot(InternalNode *leafNodes, int count){
	int parents = count/2 + count%2;
	InternalNode *newInternal = malloc(((count/2)+(count%2))*sizeof(InternalNode));
	unsigned char *temp = (unsigned char *) malloc(2*SHA256_BLOCK_SIZE);
	int j = 0;
	for(int i = 0; i < count; i+=2){
		if(i != count-1){	
			memcpy(temp, leafNodes[i].hash, SHA256_BLOCK_SIZE);
			memcpy(temp+SHA256_BLOCK_SIZE, leafNodes[i+1].hash, SHA256_BLOCK_SIZE);
			memcpy(newInternal[j].hash, hash64(temp), SHA256_BLOCK_SIZE);
			newInternal[j].leftChild = &leafNodes[i].hash;
			newInternal[j].rightChild = &leafNodes[i+1].hash;
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
			strncpy(newInternal[j].rightEdge, &leafNodes[i+1].rightEdge, 100);
		}else{
			unsigned char *temp = &(leafNodes[i].hash);
			memcpy(newInternal[j].hash, hash(hash(temp)), SHA256_BLOCK_SIZE);
			newInternal[j].leftChild = &leafNodes[i];
			newInternal[j].rightChild = NULL;
			strncpy(newInternal[j].rightEdge, &leafNodes[i].rightEdge, 100);
			strncpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge, 100);
		}
		j++;
		if(parents == 1){
			return newInternal;
		}
	}
	return merkleTreeRoot(newInternal, parents);
}

/*
free_merkle_tree(InternalNode *internalNodes) {
	free(internalNodes);
}
*/
