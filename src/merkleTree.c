/* IMPLEMENTATION OF MERKLE TREE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "merkleTree.h"
#include "sha_256.h"

LeafNode **merkletree(char *SortedArray, int count) {
    //printf("elements read in merkleTree.c: \n%s\n", SortedArray);
	LeafNode leafnode[7];
	printf("----");
	for(int i = 0; i < count; i++){
		leafnode[i]->hash = &SortedArray[i];
		leafnode[i]->value = &SortedArray[i];
	}
}

/* LeafNode **createLeafNode(LeafNode **leafnode, int count){
	leafnode = malloc(count * 100* sizeof(char));
	for(int i = 0; i < count; i++){
		leafnode[i] = NULL;
		//strcpy(leafnode[i].hash, NULL);
		//strcpy(leafnode[i].value, NULL);
	}
	return leafnode;
} */


/* 
InternalNode *merkleTreeRoot(merkleTree **){
}*/

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