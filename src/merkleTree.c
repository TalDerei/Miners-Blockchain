/* IMPLEMENTATION OF MERKLE TREE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "merkleTree.h"
#include "sha_256.h"

LeafNode *merkletree(char *SortedArray[], int count) {
    printf("elements read in merkleTree.c: \n%s\n%s\n%s\n", SortedArray[0],SortedArray[1],SortedArray[2]);
	struct leaf_node leafnode[count];
	for(int i = 0; i < count; i++){
		leafnode[i].value = SortedArray[i];
		leafnode[i].hash = hash(SortedArray[i]);
	}
	printf("LeafNode hash \n%s\n%s\n%s\n", leafnode[0].hash,leafnode[1].hash,leafnode[2].hash);
	printf("LeafNode value \n%s\n%s\n%s\n", leafnode[0].value,leafnode[1].value,leafnode[2].value);
	return leafnode;
}


InternalNode *merkleTreeRoot(merkleTree *){

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