/* IMPLEMENTATION FOR PRINTING MERKLE TREE */

#include "printMerkleTree.h"

void print_merkle_tree(InternalNode *root, int ID, FILE *output) {
	fprintf(output,"#######");
	fprintf(output,"iD is:%d\n", ID);
	
	
	if(strcmp(root->leftEdge, root->rightEdge)){
		fprintf(output,"left edge is: %s\n", root->leftEdge);
	}

	fprintf(output,"hash is:");
	for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
		fprintf(output,"%x", (unsigned char) root->hash[n]);
	}
	fprintf(output,"\n");

	if(strcmp(root->leftEdge, root->rightEdge)){
		fprintf(output,"right edge is: %s\n", root->rightEdge);
	}else{
		fprintf(output, "leafNode value is: %s\n", root->leftEdge);
		fprintf(output,"leaf node\n");
	}

	if (root->leftChild != NULL) {
		print_merkle_tree(root->leftChild, 2*ID, output);
	}
	if (root->rightChild != NULL) {
		print_merkle_tree(root->rightChild, 2*ID+1, output);
	}
}