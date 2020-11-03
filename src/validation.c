/* IMPLEMENTATION FOR VALIDATING BLOCK */

#include "validation.h"
#define BUFFER 119

int validation(Block2 *block2, char actualFileNameMerkleTree[], char actualFileNameBlock[], int counter) {
    printf("******** Entered validation **********\n");
    printf("actualFileNameMerkleTree is: %s\n", actualFileNameMerkleTree);
    printf("actualFileNameBlock is: %s\n", actualFileNameBlock);

    validate_header(block2, actualFileNameBlock, counter);
    exit(0);

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
    double buffer_target[1];
    unsigned int buffer_nonce[1];
    unsigned char temp1_rootHash[100];
    unsigned char temp2_rootHash[100];
    unsigned char temp1_timestamp[100];
    unsigned char temp2_timestamp[100];

    FILE *outputBlock; 
    outputBlock = fopen(actualFileNameBlock,"rb");
    
    //previous hash
    fseek(outputBlock, 43, SEEK_SET);
    if (counter == 0) {
        fread(buffer_first_previousHash, sizeof(unsigned char), sizeof(buffer_first_previousHash), outputBlock);
        printf("buffer_first_previousHash is: ");
        for(int i = 0; i < 1; i++){
            printf("%c", (unsigned char) buffer_first_previousHash[i]);
        }
        printf("\n");
    }
    else {
        fread(buffer_not_first_previousHash, sizeof(unsigned char), sizeof(buffer_not_first_previousHash), outputBlock);
        for (int n = 0; n < sizeof(buffer_not_first_previousHash); n++) {
            if ((unsigned char)buffer_not_first_previousHash[n] <= 0x0f) {
                printf("0%c", (unsigned char)buffer_not_first_previousHash[n]);
            } else{
                printf("%c", (unsigned char)buffer_not_first_previousHash[n]);
            }
        }
        printf("\n");
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
    printf("\n");
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
    printf("\temp1_rootHash is: \n");
    int i=0;
    char l;
    while((l=fgetc(bufferRoothash))!=EOF) {
        temp1_rootHash[i] = l;
        printf("%c", temp1_rootHash[i]);
        i++;
    }
    printf("\ntemp2_rootHash: \n");
    int j =0;
    char t;
    while((t=fgetc(block2Roothash))!=EOF) {
        temp2_rootHash[j] = t;
        printf("%c", temp2_rootHash[j]);
        j++;
    }
    printf("\ntemp1_rootHash is:\n");
    for (int i = 0; i < sizeof(temp1_rootHash); i++) {
        printf("%c", (unsigned char)temp1_rootHash[i]);
    }
    printf("\ntemp2_rootHash is:\n");
    for (int i = 0; i < sizeof(temp2_rootHash); i++) {
        printf("%c", (unsigned char)temp2_rootHash[i]);
    }
    printf("\nsize of temp1_rootHash is: %d\n", sizeof(temp1_rootHash));
    printf("size of temp2_rootHash is: %d\n", sizeof(temp2_rootHash));

    int retRoothash = memcmp(temp1_rootHash, temp2_rootHash, sizeof(buffer_rootHash));
    printf("value of ret is: %d\n", retRoothash);
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
    printf("temp1_timestamp is: \n");
    int a = 0;
    char b;
    while((b=fgetc(bufferTimestamp))!=EOF) {
        temp1_timestamp[a] = b;
        printf("%c", temp1_timestamp[a]);
        a++;
    }

    printf("temp2_timestamp is: \n");
    int c = 0;
    char d;
    while((d=fgetc(block2Timestamp))!=EOF) {
        temp2_timestamp[c] = d;
        printf("%c", temp2_timestamp[c]);
        c++;
    }
    printf("\n");
    printf("temp1_timestamp is:\n");
    for (int i = 0; i < sizeof(temp1_timestamp); i++) {
        printf("%c", (unsigned char)temp1_timestamp[i]);
    }
    printf("\ntemp2_timestamp is:\n");
    for (int i = 0; i < sizeof(temp2_timestamp); i++) {
        printf("%c", (unsigned char)temp2_timestamp[i]);
    }
    printf("\n");

    printf("size of temp1_timestamp is: %d\n", sizeof(temp1_timestamp));
    printf("size of temp2_timestamp is: %d\n", sizeof(temp2_timestamp));

    int retTimestamp = memcmp(temp1_timestamp, temp2_timestamp, 8);
    printf("value of retTimestamp is: %d\n", retTimestamp);

    if(retTimestamp != 0) {
    printf("*******************error with timestamp\n");
        return 0;
    }
    remove("bufferOutput.txt");
    remove("block2Output.txt");


    
    // printf("size of block2->target is: %d\n", sizeof(block2->target));
    
    // printf("size of temp is: %d\n", sizeof(temp));
    // if(memcmp(&(block2->target), temp, sizeof(temp))) {
    //     printf("*******************error with target\n");
    //     // return 0;
    // }
    // exit(0);


    // fread(buffer_nonce, sizeof(unsigned int), 1, outputBlock);
    // if(memcmp(block2->nonce, buffer_nonce, sizeof(unsigned int))) {
    //     printf("*******************error with timestamp\n");
    //     // return 0;
    // }
    // exit(0);
    // return 1;
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
        compareBuffer[len-1] = '\0';
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


	// fprintf(output,"hash is: ");
	// for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
	// 	fprintf(output,"%x", (unsigned char) root->hash[n]);
	// }
	// fprintf(output,"\n");

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
            compareBuffer[len-1] = '\0';
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
            compareBuffer[len-1] = '\0';
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