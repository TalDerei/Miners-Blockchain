/* IMPLEMENTATION OF MERKLE TREE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "node.h"
#include "merkleTree.h"
#include "sha_256.h"


LeafNode *merkletree(char *SortedArray[], int count) {
	struct leaf_node leafnode[count];
	for(int i = 0; i < count; i++){
		leafnode[i].value = SortedArray[i];
		leafnode[i].hash = SortedArray[i];
	}
	for(int i = 0; i < count; i++){
		printf("LeafNode value \n%s\n", leafnode[i].value);
	}
	
	return leafnode;
}


InternalNode *merkleTreeRoot(LeafNode *merkleTree, int count){
	//get ceiling
	InternalNode newInternal[(count/2)+(count%2)];
	LeafNode *newLeafNode[(count/2)+(count%2)];
	int parents = count/2 + count%2;
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
/* 
char *sorting(ContentArray *){
}*/

/* 
size_t hash(size_t key) {
  uint32_t hash = 5381;
  unsigned char* rbytes = (unsigned char*)(&key); //rbytes is a character pointer, where it takes the address of the key and typecasts it to an unsigned char *
   for (size_t i = 0; i < 8; ++i) { //we want to iteratively scan through each of the 8 bits, given the for loop of size 8
        hash = ((hash << 5) + hash) ^ (rbytes[i]); //part of the hashvalue is given, and rybytes at index i is xor with the hash value
   }
   return hash;
}
*/

/*int sort(const void * a, const void * b) {
		return ( *(int*)a - *(int*)b );
}*/