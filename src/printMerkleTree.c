/* IMPLEMENTATION FOR PRINTING MERKLE TREE */

#include "printMerkleTree.h"

/* printing out merkle tree associated with file contents */
void print_merkle_tree(InternalNode *root, int ID, FILE *output) {
	fprintf(output,"\nID is:%d\n", ID);
	if(root->leftChild != NULL || root->rightChild != NULL){
		fprintf(output,"left edge is: %s\n", root->leftEdge);
	}
	fprintf(output,"hash is: ");
	for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		fprintf(output,"%x", (unsigned char) root->hash[n]);
	}
	fprintf(output,"\n");

	if(root->leftChild != NULL || root->rightChild != NULL){
		fprintf(output,"right edge is: %s\n", root->rightEdge);
	}else{
		fprintf(output, "leafNode value is: %s\n", root->leftEdge);
		fprintf(output,"leaf node\n\n");
	}

	if (root->rightChild != NULL) {
		fprintf(output, "leftChild is: %d\n", 2*ID);
		fprintf(output, "rightChild is: %d\n",  2*ID+1);
		print_merkle_tree(root->leftChild, 2*ID, output);
		print_merkle_tree(root->rightChild, 2*ID+1, output);
	}else if (root->leftChild != NULL) {
		fprintf(output, "leftChild is: %d\n", 2*ID);
		fprintf(output, "rightChild is NULL\n");
		print_merkle_tree(root->leftChild, 2*ID, output);
	}
}

/* print original file contents */
void print_merkle_tree_value(char** arr,int lineNum, FILE *output){
	/* fprintf(output, "%d\n", lineNum); */
	fprintf(output, "BEGVALUE\n");
	for(int i = 0; i < lineNum; i++){
		fprintf(output, "%s\n",  arr[i]);
	}
	fprintf(output, "ENDVALUE\n");
}