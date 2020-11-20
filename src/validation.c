/* IMPLEMENTATION FOR VALIDATING BLOCK */

#include "validation.h"
#define BUFFER 119

int validation(Block2 *block2, char actualFileNameMerkleTree[], char actualFileNameBlock[], int counter) {
    printf("******** Entered validation **********\n");
    printf("actualFileNameMerkleTree is: %s\n", actualFileNameMerkleTree);
    printf("actualFileNameBlock is: %s\n", actualFileNameBlock);

    validate_header(block2, actualFileNameBlock, counter);
    validate_merkle_tree(block2, actualFileNameMerkleTree);
    exit(0);

    if (validate_header(block2, actualFileNameBlock, counter) && validate_merkle_tree(block2, actualFileNameMerkleTree)) {
        return 1;
    }
    else {
        return 0;
    }
}

//validate header contents of block
int validate_header(Block2 *block2, char actualFileNameBlock[], int counter) {
    printf("******** Entered validate_header **********\n");
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

    //open file associated with outputBlock file handler
    FILE *outputBlock; 
    outputBlock = fopen(actualFileNameBlock,"rb");
    
    //previous hash
    fseek(outputBlock, 43, SEEK_SET);
    if (counter == 0) {
        fread(buffer_first_previousHash, sizeof(unsigned char), sizeof(buffer_first_previousHash), outputBlock);
        // printf("buffer_first_previousHash is: ");
        // for(int i = 0; i < 1; i++){
        //     printf("%c", (unsigned char) buffer_first_previousHash[i]);
        // }
        // printf("\n");
    }
    else {
        fread(buffer_not_first_previousHash, sizeof(unsigned char), sizeof(buffer_not_first_previousHash), outputBlock);
        // for (int n = 0; n < sizeof(buffer_not_first_previousHash); n++) {
        //     if ((unsigned char)buffer_not_first_previousHash[n] <= 0x0f) {
        //         printf("0%c", (unsigned char)buffer_not_first_previousHash[n]);
        //     } else{
        //         printf("%c", (unsigned char)buffer_not_first_previousHash[n]);
        //     }
        // }
        // printf("\n");
    }

    //roothash
    fseek(outputBlock, 15, SEEK_CUR);
    fgets(buffer_rootHash, BUFFER, outputBlock);
    FILE *bufferRoothash; 
    bufferRoothash = fopen("bufferOutput.txt","w+b");
    for (int n = 0; n < sizeof(buffer_rootHash); n++) {
        if ((unsigned char)buffer_rootHash[n] <= 0x0f) {
                fprintf(bufferRoothash, "0%c", (unsigned char)buffer_rootHash[n]);
        } else{
                fprintf(bufferRoothash, "%c", (unsigned char)buffer_rootHash[n]);
        }
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
        //printf("%c", temp1_rootHash[i]);
        i++;
    }
    int j =0; char t;
    while((t=fgetc(block2Roothash))!=EOF) {
        temp2_rootHash[j] = t;
        //printf("%c", temp2_rootHash[j]);
        j++;
    }
    int retRoothash = memcmp(temp1_rootHash, temp2_rootHash, sizeof(buffer_rootHash));
    if(retRoothash != 0) {
    printf("Error with roothash***********************************\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    //timestamp
    printf("\n");
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
        //printf("%c", temp1_timestamp[a]);
        a++;
    }
    int c = 0; char d;
    while((d=fgetc(block2Timestamp))!=EOF) {
        temp2_timestamp[c] = d;
        //printf("%c", temp2_timestamp[c]);
        c++;
    }

    int retTimestamp = memcmp(temp1_timestamp, temp2_timestamp, 8);
    if(retTimestamp != 0) {
    printf("*******************error with timestamp\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    //target
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
        //printf("%c", temp1_target[e]);
        e++;
    }
    int g = 0; char h;
    while((h=fgetc(block2Target))!=EOF) {
        temp2_target[g] = h;
        //printf("%c", temp2_target[g]);
        g++;
    }

    int retTarget = memcmp(temp1_target, temp2_target, 8);
    if(retTimestamp != 0) {
    printf("*******************error with target\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");

    //nonce
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
        //printf("%c", temp1_nonce[k]);
        k++;
    }
    int m = 0; char n;
    while((n=fgetc(block2Nonce))!=EOF) {
        temp2_nonce[m] = n;
        //printf("%c", temp2_nonce[m]);
        m++;
    }

    int retNonce = memcmp(temp1_nonce, temp2_nonce, 10);
    if(retNonce != 0) {
    printf("*******************error with nonce\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");
}

//validate merkle tree of block
int validate_merkle_tree(Block2 *block2, char actualFileNameMerkleTree[]) {
    printf("******** Entered validate_merkle_tree **********\n");
    FILE *outputMerkleTree; 
    outputMerkleTree = fopen(actualFileNameMerkleTree,"r");

    //hold value for the number of lines in input file
    int bufferCount[1];
    //compareBuffer hold the line of actualFileNameMerkleTree and compares with merkleTreeRoot
    char *compareBuffer = (char *)malloc(BUFFER);
    fgets(bufferCount, sizeof(int), outputMerkleTree);
    int buffer_counter = atoi(bufferCount);
    printf("buffer_converter is: %d\n", (int)buffer_counter);

    char **buffer_value = (char **)malloc(sizeof(char *));
    for (int i = 0; i < buffer_counter; i++){
        buffer_value[i] = (char *)malloc(BUFFER);
        fgets(buffer_value[i], BUFFER, outputMerkleTree);
    }

    //createLeafNodes, convertLeaftoInternal, merkleTreeRoot -- 3 function calls
    InternalNode *internalNode = (InternalNode *)malloc(buffer_counter * sizeof(InternalNode)); 
    LeafNode *leafNodes = (LeafNode *)malloc(buffer_counter * sizeof(LeafNode));
    createLeafNodes(leafNodes, buffer_value, buffer_counter);
    convertLeaftoInternal(internalNode, leafNodes, buffer_counter);
    InternalNode *root = merkleTreeRoot(internalNode, buffer_counter); 
    compareMerkleTree(root, outputMerkleTree, compareBuffer,1);
}

int compareMerkleTree(InternalNode *root, FILE *fp, char *compareBuffer, int ID) {
    printf("******** Entered validate_merkle_tree **********\n");
    int len;
    //check for correct ID
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

    //check for correct left edge
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
		//fprintf(output,"left edge is: %s\n", root->leftEdge);
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
		//fprintf(output,"right edge is: %s\n", root->rightEdge);
	}else{
        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
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
            compareBuffer[len-1] = '\0';
        }
        int compareBuffer_int2 = atoi(compareBuffer + 14);
        if(compareBuffer_int2 != 2*ID){
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
        int compareBuffer_int3 = atoi(compareBuffer + 15);
        if(compareBuffer_int3 != 2*ID+1){
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
            compareBuffer[len-1] = '\0';
        }
        int compareBuffer_int4 = atoi(compareBuffer + 14);
        if(compareBuffer_int4 != 2*ID){
            printf("validation failed at left child ID 194\n");
            return 0;
        }
        clearBuffer(compareBuffer);
		//fprintf(output, "leftChild is: %d\n", 2*ID);

        fgets(compareBuffer, BUFFER, fp);
        len = strlen(compareBuffer);
        if( compareBuffer[len-1] == '\n') {
            compareBuffer[len-1] = '\0';
        }
        int compareBuffer_int5 = atoi(compareBuffer + 15);
        if(compareBuffer_int5 != 0){
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