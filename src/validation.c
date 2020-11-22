/* IMPLEMENTATION FOR VALIDATING BLOCK */

#include "validation.h"
#define BUFFER 119

/* validation validates entire blockchain block by block */
int validation(Block2 *block2, char actualFileNameMerkleTree[], char actualFileNameBlock[], int counter) {
    if (validate_header(block2, actualFileNameBlock, counter) && validate_merkle_tree(block2, actualFileNameMerkleTree)) {
        return 1;
    }
    else {
        return 0;
    }
}

/* validate header contents of a block */
int validate_header(Block2 *block2, char actualFileNameBlock[], int counter) {
    /* buffers used for comparing contents of block with contents stored in input.block.txt and input.merkletree.txt */
    unsigned char buffer_first_previousHash[1];    
    unsigned char buffer_not_first_previousHash[64];
    unsigned char buffer_rootHash[64];
    int buffer_timestamp[8];
    double buffer_target[8];
    unsigned int buffer_nonce[10];
    unsigned char temp1_rootHash[100];
    unsigned char temp2_rootHash[100];
    unsigned char temp1_timestamp[100];
    unsigned char temp2_timestamp[100];
    unsigned char temp1_target[100];
    unsigned char temp2_target[100];
    unsigned char temp1_nonce[100];
    unsigned char temp2_nonce[100];

    /* open file associated with outputBlock file handler */
    FILE *outputBlock; 
    outputBlock = fopen(actualFileNameBlock,"rb");
    
    /* validate previous hash */
    fseek(outputBlock, 43, SEEK_SET);
    if (counter == 0) {
        fread(buffer_first_previousHash, sizeof(unsigned char), sizeof(buffer_first_previousHash), outputBlock);
    }
    else {
        fread(buffer_not_first_previousHash, sizeof(unsigned char), sizeof(buffer_not_first_previousHash), outputBlock);
    }

    /* validate roothash */
    fseek(outputBlock, 15, SEEK_CUR);
    fgets(buffer_rootHash, BUFFER, outputBlock);
    FILE *bufferRoothash; 
    bufferRoothash = fopen("bufferOutput.txt","w+b");
    for (int n = 0; n < sizeof(buffer_rootHash); n++) {
        fprintf(bufferRoothash, "%c", (unsigned char)buffer_rootHash[n]);
    }

    FILE *block2Roothash; 
    block2Roothash = fopen("block2Output.txt","w+b");
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        if ((unsigned char)block2->rootHash[i] <= 0x0f) {
                fprintf(block2Roothash, "0%x", (unsigned char)block2->rootHash[i]);
        } else{
                fprintf(block2Roothash, "%x", (unsigned char)block2->rootHash[i]);
        }
    }
    rewind(bufferRoothash);
    rewind(block2Roothash);

    int i=0; char l;
    while((l=fgetc(bufferRoothash))!=EOF) {
        temp1_rootHash[i] = l;
        i++;
    }
    int j =0; char t;
    while((t=fgetc(block2Roothash))!=EOF) {
        temp2_rootHash[j] = t;
        j++;
    }
    int retRoothash = memcmp(temp1_rootHash, temp2_rootHash, sizeof(buffer_rootHash));
    if(retRoothash != 0) {
        printf("Validation failed! Error with roothash!\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    /* validate timestamp */
    fseek(outputBlock, 14, SEEK_CUR);
    fgets(buffer_timestamp, BUFFER, outputBlock);
    FILE *bufferTimestamp; 
    bufferTimestamp = fopen("bufferOutput.txt","w+b");
    int timestamp_converted = atoi(buffer_timestamp);
    fprintf(bufferTimestamp, "%d", timestamp_converted);

    FILE *block2Timestamp; 
    block2Timestamp = fopen("block2Output.txt","w+b");
    fprintf(block2Timestamp, "%d", (int)block2->timestamp);

    rewind(bufferTimestamp);
    rewind(block2Timestamp);

    int a = 0; char b;
    while((b=fgetc(bufferTimestamp))!=EOF) {
        temp1_timestamp[a] = b;
        a++;
    }
    int c = 0; char d;
    while((d=fgetc(block2Timestamp))!=EOF) {
        temp2_timestamp[c] = d;
        c++;
    }

    int retTimestamp = memcmp(temp1_timestamp, temp2_timestamp, 8);
    if(retTimestamp != 0) {
        printf("Valation failed! Error with timestamp!\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    /* validate target */
    fseek(outputBlock, 17, SEEK_CUR);
    fgets(buffer_target, BUFFER, outputBlock);
    FILE *bufferTarget; 
    bufferTarget = fopen("bufferOutput.txt","w+b");
    double target_converted = atof(buffer_target);
    fprintf(bufferTarget, "%f", (double)target_converted);

    FILE *block2Target; 
    block2Target = fopen("block2Output.txt","w+b");
    fprintf(block2Target, "%f", (double)block2->target);

    rewind(bufferTarget);
    rewind(block2Target);

    int e = 0; char f;
    while((f=fgetc(bufferTarget))!=EOF) {
        temp1_target[e] = f;
        e++;
    }
    int g = 0; char h;
    while((h=fgetc(block2Target))!=EOF) {
        temp2_target[g] = h;
        g++;
    }

    int retTarget = memcmp(temp1_target, temp2_target, 8);
    if(retTimestamp != 0) {
        printf("Validation Failed! Error with target\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    /* validate nonce */
    fseek(outputBlock, 10, SEEK_CUR);
    fgets(buffer_nonce, BUFFER, outputBlock);
    FILE *bufferNonce; 
    bufferNonce = fopen("bufferOutput.txt","w+b");
    int nonce_converted = atoi(buffer_nonce);
    fprintf(bufferNonce, "%d", (int)nonce_converted);

    FILE *block2Nonce; 
    block2Nonce = fopen("block2Output.txt","w+b");
    fprintf(block2Nonce, "%d", (int)block2->nonce);

    rewind(bufferNonce);
    rewind(block2Nonce);
    int k = 0; char z;
    while((z=fgetc(bufferNonce))!=EOF) {
        temp1_nonce[k] = z;
        k++;
    }
    int m = 0; char n;
    while((n=fgetc(block2Nonce))!=EOF) {
        temp2_nonce[m] = n;
        m++;
    }

    int retNonce = memcmp(temp1_nonce, temp2_nonce, 9);
    if(retNonce != 0) {
        printf("Validation failed! Error with nonce\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    return 1;
}

/* validate merkle tree of a block */
int validate_merkle_tree(Block2 *block2, char actualFileNameMerkleTree[]) {
    /* open file associated with outputMerkleTree file handler */
    FILE *outputMerkleTree; 
    outputMerkleTree = fopen(actualFileNameMerkleTree,"r");

    int bufferCount[1]; 
    char *compareBuffer = (char *)malloc(BUFFER);
    fgets(bufferCount, sizeof(int), outputMerkleTree);
    int buffer_counter = atoi(bufferCount);
    char **buffer_value = (char **)malloc(sizeof(char *));
    for (int i = 0; i < buffer_counter; i++){
        buffer_value[i] = (char *)malloc(BUFFER);
        fgets(buffer_value[i], BUFFER, outputMerkleTree);
    }

    /* createLeafNodes, convertLeaftoInternal, merkleTreeRoot -- 3 function calls */
    InternalNode *internalNode = (InternalNode *)malloc(buffer_counter * sizeof(InternalNode)); 
    LeafNode *leafNodes = (LeafNode *)malloc(buffer_counter * sizeof(LeafNode));
    createLeafNodes(leafNodes, buffer_value, buffer_counter);
    convertLeaftoInternal(internalNode, leafNodes, buffer_counter);
    InternalNode *root = merkleTreeRoot(internalNode, buffer_counter); 
    return compareMerkleTree(root, outputMerkleTree, compareBuffer,1);
}

/* validation of the merkletree associated with a block */
int compareMerkleTree(InternalNode *root, FILE *fp, char *compareBuffer, int ID) {
    /* same code-base as in printMerkleTree.c, except modified for validation rather than printing */
    int len;
    fgets(compareBuffer, BUFFER, fp);
    len = strlen(compareBuffer);
    if( compareBuffer[len-1] == '\n') {
        compareBuffer[len-1] = '\0';
    }
    int compareBuffer_int1 = atoi(compareBuffer + 7);
    if (compareBuffer_int1 != ID) {
        printf("validation failed at ID\n");
    }
    clearBuffer(compareBuffer);

	if(root->leftChild != NULL || root->rightChild != NULL){
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        if(!memcmp(compareBuffer+14, root->leftEdge, 100)){
            printf("validation failed at leftEdge\n");
            return 0;
        }
        clearBuffer(compareBuffer);
	}

    fgets(compareBuffer, BUFFER, fp);
    len = strlen(compareBuffer);
    if( compareBuffer[len-1] == '\n') {
        compareBuffer[len-1] = '\0';
    }
    if(!memcmp(compareBuffer+9, root->hash, SHA256_BLOCK_SIZE)){
        printf("validation failed at hash\n");
        return 0;
    }
    clearBuffer(compareBuffer);

	if(root->leftChild != NULL || root->rightChild != NULL){
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        if(!memcmp(compareBuffer+15, root->rightEdge, 100)){
            printf("validation failed at rightEdge\n");
            return 0;
        }
        clearBuffer(compareBuffer);
	}else{
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        if(!memcmp(compareBuffer+19, root->leftEdge, 100)){
            printf("validation failed at leftEdge\n");
            return 0;
        }
        clearBuffer(compareBuffer);

        fgets(compareBuffer, BUFFER, fp);
        clearBuffer(compareBuffer);
	}

	if (root->rightChild != NULL) {
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        int compareBuffer_int2 = atoi(compareBuffer + 14);
        if(compareBuffer_int2 != 2*ID){
            printf("validation failed at left child ID\n");
            return 0;
        }
        clearBuffer(compareBuffer);

        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] == '\0';
        }
        int compareBuffer_int3 = atoi(compareBuffer + 15);
        if(compareBuffer_int3 != 2*ID+1){
            printf("validation failed at right child ID\n");
            return 0;
        }
        clearBuffer(compareBuffer);
        
        compareMerkleTree(root->leftChild, fp, compareBuffer, 2*ID);

        compareMerkleTree(root->rightChild, fp, compareBuffer, 2*ID+1);

	}else if (root->leftChild != NULL) {
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        int compareBuffer_int4 = atoi(compareBuffer + 14);
        if(compareBuffer_int4 != 2*ID){
            printf("validation failed at left child ID\n");
            return 0;
        }
        clearBuffer(compareBuffer);

        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        int compareBuffer_int5 = atoi(compareBuffer + 15);
        if(compareBuffer_int5 != 0){
            printf("validation failed at right child ID\n");
            return 0;
        }
        clearBuffer(compareBuffer);

        compareMerkleTree(root->leftChild, fp, compareBuffer, 2*ID);
	}
    return 1;
}

/* clear buffer for re-use */
void clearBuffer(char *buffer) {
    for(int i = 0 ; i < BUFFER; i++){
        buffer[i] = NULL;
    }
}