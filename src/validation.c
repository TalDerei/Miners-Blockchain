/* IMPLEMENTATION FOR VALIDATING BLOCK */

#include "validation.h"
#define BUFFER 100+19

int VALIDATION(Block2 *block2, char actualFileNameMerkleTree[], char actualFileNameBlock[]) {
    printf("******** Entered validation **********\n");
    printf("actualFileNameMerkleTree is: %s\n", actualFileNameMerkleTree);
    printf("actualFileNameBlock is: %s\n", actualFileNameBlock);

    validate_header(block2, actualFileNameBlock);
    exit(0);


    validate_merkle_tree(block2, actualFileNameMerkleTree);
    if (validate_header(block2, actualFileNameBlock) && validate_merkle_tree(block2, actualFileNameMerkleTree)) {
        return 1;
    }
    else {
        return 0;
    }
}

//validate header contents of block
int validate_header(Block2 *block2, char actualFileNameBlock[]) {
    printf("******** Entered validate_header **********\n");
    
    unsigned char buffer_previousHash[32];
    unsigned char buffer_rootHash[32];
    int buffer_timestamp[1];
    double buffer_target[1];
    unsigned int buffer_nonce[1];
    
    //open file
    FILE *outputBlock; 
    outputBlock = fopen(actualFileNameBlock,"r");
    //rewind to rootHash in file
    fread(buffer_previousHash, sizeof(unsigned char), sizeof(buffer_previousHash), outputBlock);

    fread(buffer_rootHash, sizeof(unsigned char), sizeof(buffer_rootHash), outputBlock);
    
    if (memcmp(block2->rootHashHeader, buffer_rootHash, SHA256_BLOCK_SIZE)) {
        printf("block2->rootHashHeader is: ");
        for(int i = 0; i < SHA256_BLOCK_SIZE; i++){
            printf("%x", (unsigned char) block2->rootHashHeader[i]);
        }
        printf("\n");

        printf("buffer_rootHash is: ");
        for(int i = 0; i < SHA256_BLOCK_SIZE; i++){
            printf("%x", (unsigned char) buffer_rootHash[i]);
        }
        printf("\n");
        exit(0);

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

    return 1;
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

int compareMerkleTree(InternalNode *root, FILE *fp, char *compareBuffer, int ID) {
    int len;
    //check for correct ID
    fgets(compareBuffer, BUFFER, fp);
    len = strlen(compareBuffer);
    if( compareBuffer[len-1] == '\n') {
        compareBuffer[len-1] == '\0';
    }
    if(!(compareBuffer+7 == ID)){
        printf("validation failed at ID\n");
        return 0;
    }
    clearBuffer(compareBuffer);
    //fprintf(output,"ID is: %d\n", ID);

    //check for correct left edge
	if(root->leftChild != NULL || root->rightChild != NULL){
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] == '\0';
        }
        if(!memcmp(compareBuffer+14, root->leftEdge, 100)){
            printf("validation failed at leftEdge\n");
            return 0;
        }
        clearBuffer(compareBuffer);
		//fprintf(output,"left edge is: %s\n", root->leftEdge);
	}


    fgets(compareBuffer, BUFFER, fp);
    len = strlen(compareBuffer);
    if( compareBuffer[len-1] == '\n') {
        compareBuffer[len-1] == '\0';
    }
    if(!memcmp(compareBuffer+9, root->hash, SHA256_BLOCK_SIZE)){
        printf("validation failed at hash\n");
        return 0;
    }
    clearBuffer(compareBuffer);


	// fprintf(output,"hash is: ");
	// for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
	// 	fprintf(output,"%x", (unsigned char) root->hash[n]);
	// }
	// fprintf(output,"\n");

	if(root->leftChild != NULL || root->rightChild != NULL){
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] == '\0';
        }
        if(!memcmp(compareBuffer+15, root->rightEdge, 100)){
            printf("validation failed at rightEdge\n");
            return 0;
        }
        clearBuffer(compareBuffer);
		//fprintf(output,"right edge is: %s\n", root->rightEdge);
	}else{
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] == '\0';
        }
        if(!memcmp(compareBuffer+19, root->leftEdge, 100)){
            printf("validation failed at leftEdge 146\n");
            return 0;
        }
        clearBuffer(compareBuffer);
		//fprintf(output, "leafNode value is: %s\n", root->leftEdge);

        fgets(compareBuffer, BUFFER, fp);
        clearBuffer(compareBuffer);
		//fprintf(output,"leaf node\n\n");
	}

	if (root->rightChild != NULL) {
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] == '\0';
        }
        if(!(compareBuffer+14 == 2*ID)){
            printf("validation failed at left child ID 164\n");
            return 0;
        }
        clearBuffer(compareBuffer);
		//fprintf(output, "leftChild is: %d\n", 2*ID);

        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] == '\0';
        }
        if(!(compareBuffer+15 == 2*ID+1)){
            printf("validation failed at right child ID 176\n");
            return 0;
        }
        clearBuffer(compareBuffer);
		//fprintf(output, "rightChild is: %d\n",  2*ID+1);
        
        compareMerkleTree(root->leftChild, fp, compareBuffer, 2*ID);
		//print_merkle_tree(root->leftChild, 2*ID, output);

        compareMerkleTree(root->rightChild, fp, compareBuffer, 2*ID+1);
		//print_merkle_tree(root->rightChild, 2*ID+1, output);
	}else if (root->leftChild != NULL) {
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] == '\0';
        }
        if(!(compareBuffer+14 == 2*ID)){
            printf("validation failed at left child ID 194\n");
            return 0;
        }
        clearBuffer(compareBuffer);
		//fprintf(output, "leftChild is: %d\n", 2*ID);

        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] == '\0';
        }
        if(!(compareBuffer+15 == 2*ID+1)){
            printf("validation failed at right child ID 206\n");
            return 0;
        }
        clearBuffer(compareBuffer);
		//fprintf(output, "rightChild is NULL\n");

        compareMerkleTree(root->leftChild, fp, compareBuffer, 2*ID);
		//print_merkle_tree(root->leftChild, 2*ID, output);
	}
    return 1;
}

void clearBuffer(char *buffer) {
    for(int i = 0 ; i < BUFFER; i++){
        buffer[i] = NULL;
    }
}

// void substring(char* buffer, int index){
//     *(buffer+index) = *(buffer+index);
//     // *(buffer+index+1) = '\0';
// }