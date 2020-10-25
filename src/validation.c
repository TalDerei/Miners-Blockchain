/* IMPLEMENTATION FOR VALIDATING BLOCK */

#include "validation.h"
#define BUFFER 100

int validation(Block2 *block2, char actualFileNameMerkleTree[], char actualFileNameBlock[]) {
    if (validate_header(block2, actualFileNameBlock) && validate_merkle_tree(block2, actualFileNameMerkleTree)) {
        return 1;
    }
    else {
        return 0;
    }
}

//validate header contents of block
int validate_header(Block2 *block2, char actualFileNameBlock[]) {
    unsigned char buffer_previousHash[32];
    unsigned char buffer_rootHash[32];
    int buffer_timestamp[1];
    double buffer_target[1];
    unsigned int buffer_nonce[1];
    
    //open file
    FILE *outputBlock; 
    outputBlock = fopen(actualFileNameBlock,"w");
    
    //rewind to rootHash in file
    rewind(SHA256_BLOCK_SIZE);
    fread(buffer_rootHash, sizeof(unsigned char), sizeof(buffer_rootHash), outputBlock);
    
    if (memcmp(block2->rootHashHeader, buffer_rootHash, SHA256_BLOCK_SIZE)) {
        printf("*******************error with root hash\n");
        return 0;
    }
    fread(buffer_timestamp, sizeof(int), 1, outputBlock);
    if(memcmp(block2->timestamp, buffer_timestamp, sizeof(int))) {
        printf("*******************error with timestamp\n");
        return 0;
    }
    fread(buffer_target, sizeof(double), 1, outputBlock);
    if(memcmp(&(block2->target), buffer_target, sizeof(double))) {
        printf("*******************error with target\n");
        return 0;
    }
    fread(buffer_nonce, sizeof(unsigned int), 1, outputBlock);
    if(memcmp(block2->nonce, buffer_nonce, sizeof(unsigned int))) {
        printf("*******************error with timestamp\n");
        return 0;
    }
}

//validate merkle tree of block
int validate_merkle_tree(Block2 *block2, char actualFileNameMerkleTree[]) {
    FILE *outputMerkleTree; 
    outputMerkleTree = fopen(actualFileNameMerkleTree,"w");

    char bufferCount[1];
    char *compareBuffer = (char *)malloc(BUFFER); //compareBuffer hold the line of actualFileNameMerkleTree and compares with merkleTreeRoot

    //read original file contents
    fgets(bufferCount[0], sizeof(char), outputMerkleTree);
    char **bufferValue = (char **)malloc(sizeof(char*));
    for (int i = 0; i < (int)bufferCount[0]; i++){
        bufferValue[i] = (char *)malloc(BUFFER);
        fgets(bufferValue[i], BUFFER, outputMerkleTree);
    }

    InternalNode *internalNode = (InternalNode *)malloc(bufferCount[0] * sizeof(InternalNode)); 
    LeafNode *leafNodes = (LeafNode *)malloc(bufferCount[0] * sizeof(LeafNode));
    
    //createLeafNodes, convertLeaftoInternal, merkleTreeRoot -- 3 function calls
    createLeafNodes(leafNodes, bufferValue, bufferCount[0]);
    convertLeaftoInternal(internalNode, leafNodes, bufferCount[0]);
    InternalNode *root = merkleTreeRoot(internalNode, (int)bufferCount[0]); 
    compareMerkleTree(merkleTreeRoot, outputMerkleTree, compareBuffer,1);
}

int compareMerkleTree(InternalNode *merkleTreeRoot, FILE *fp, char *compareBuffer, int ID) {
    int len;
    fgets(compareBuffer, BUFFER, fp);
    len = strlen(compareBuffer);
    if( compareBuffer[len-1] == '\n') {
        compareBuffer[len-1] == '\0';
    }
    strcmp(compareBuffer+7, (char)ID);
    //fprintf(output,"ID is: %d\n", ID);

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

void clearBuffer(char *buffer) {
for(int i = 0 ; i < BUFFER; i++){
    for( int j = 0; j < BUFFER; j++){
        buffer[i][j] = NULL;
    }
}

void substring(char* buffer, int index) {
    *(buffer+index) = *(buffer+index);
    // *(buffer+index+1) = '\0';
}