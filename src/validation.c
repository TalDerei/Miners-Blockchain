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
    unsigned char buffer_not_first_previousHash[32];
    unsigned char buffer_rootHash[64];
    unsigned char temp1[100];
    unsigned char temp2[100];
    int buffer_timestamp[1];
    double buffer_target[1];
    unsigned int buffer_nonce[1];
    
    //open file
    FILE *outputBlock; 
    outputBlock = fopen(actualFileNameBlock,"rb");
    
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

//    exit(0);
//     fread(buffer_rootHash, sizeof(unsigned char), sizeof(buffer_rootHash), outputBlock);
//     printf("buffer_rootHash is: \n");
    // for (int n = 0; n < sizeof(buffer_rootHash); n++) {
    //     if ((unsigned char)buffer_rootHash[n] <= 0x0f) {
    //             printf("0%x", (unsigned char)buffer_rootHash[n]);
    //     } else{
    //             printf("%x", (unsigned char)buffer_rootHash[n]);
    //     }
    // }
    // printf("\n");

    // printf("block2->rootHashHeader is: \n");
    // for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
    //     if ((unsigned char)block2->rootHashHeader[n] <= 0x0f) {
    //             printf("0%x", (unsigned char)block2->rootHashHeader[n]);
    //     } else{
    //             printf("%x", (unsigned char)block2->rootHashHeader[n]);
    //     }
    // }
    // printf("\n");
    
    // printf("size of block2->rootHashHeader is: %d\n", sizeof(block2->rootHashHeader));
    // printf("size of buffer_rootHash is: %d\n", sizeof(buffer_rootHash));

    // int returnValue = memcpy(block2->rootHashHeader, buffer_rootHash, sizeof(buffer_rootHash));
    // printf("returnValue is: %d\n", returnValue);
    // if (returnValue != 0) {
    //     printf("*******************error with root hash\n");
    //     // return 0;
    // }
    // printf("\n");


    // exit(0);

    int lengths;
    fseek(outputBlock, 15, SEEK_CUR);
    fgets(buffer_rootHash, BUFFER, outputBlock);
    lengths = strlen(buffer_rootHash);
    if( buffer_rootHash[lengths-1] == '\n') {
        buffer_rootHash[lengths-1] = '\0';
    }
    // fread(buffer_rootHash, sizeof(unsigned char), sizeof(buffer_rootHash), outputBlock);
    FILE *tempOutput; 
    tempOutput = fopen("tempOutput.txt","w+b");
    for (int n = 0; n < sizeof(buffer_rootHash); n++) {
        if ((unsigned char)buffer_rootHash[n] <= 0x0f) {
                fprintf(tempOutput, "0%c", (unsigned char)buffer_rootHash[n]);
        } else{
                fprintf(tempOutput, "%c", (unsigned char)buffer_rootHash[n]);
        }
    }
    printf("\n");
    FILE *bufferOutput; 
    bufferOutput = fopen("bufferOutput.txt","w+b");
    printf("block2->rootHash is: ****************************\n");
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
        if ((unsigned char)block2->rootHash[i] <= 0x0f) {
                fprintf(bufferOutput, "0%x", (unsigned char)block2->rootHash[i]);
        } else{
                fprintf(bufferOutput, "%x", (unsigned char)block2->rootHash[i]);
        }
    }
    printf("\n");

    rewind(tempOutput);
    rewind(bufferOutput);
    printf("printing out values!!!!!!!!!!!!!!\n");
    printf("temp1 is: \n");
    int i=0;
    char l;
    while((l=fgetc(tempOutput))!=EOF) {
      temp1[i] = l;
      printf("%c", temp1[i]);
      i++;
   }
   printf("\n i is: %d\n", i);
   printf("\n something random at line 139: %c\n", temp1[1]);

    for(int jjj = 0; jjj < 32; jjj++){
    //printf("%c\n", temp1[jjj]);
        printf("%c", temp1[jjj]);

    }
    printf("\n");
    printf("temp2 is: \n");
    int j =0;
   char t;
   while((t=fgetc(bufferOutput))!=EOF) {
        temp2[j] = t;
        printf("%c", temp2[j]);
        j++;
   }
    printf("\n");

    int len1 = strlen(temp1);
    if( temp1[31] == '\n') {
        temp1[31] = '\0';
    }

     int len2 = strlen(temp2);
    if( temp2[31] == '\n') {
        temp2[31] = '\0';
    }
    printf("\n");

    printf("temp1 is:\n");
    for (int i = 0; i < sizeof(temp1); i++) {
        printf("%c", (unsigned char)temp1[i]);
    }
    printf("\ntemp2 is:\n");
    for (int i = 0; i < sizeof(temp2); i++) {
        printf("%c", (unsigned char)temp2[i]);
    }
    printf("\n");

    printf("size of temp1 is: %d\n", sizeof(temp1));
    printf("size of temp2 is: %d\n", sizeof(temp2));

    int ret = memcmp(temp1, temp2, 32);
    printf("value of ret is: %d\n", ret);

    if(ret > 0) {
      printf("str2 is less than str1");
   } else if(ret < 0) {
      printf("str1 is less than str2");
   } else {
      printf("str1 is equal to str2");
   }

   exit(0);


    // printf("buffer_timestamp is: %f", buffer_timestamp);
    // int length = strlen(buffer_timestamp);
    // if( buffer_timestamp[length] == '\n') {
    //     buffer_timestamp[length] = '\0';
    // }
    // int buffer_timestamp_converted = atoi(buffer_timestamp);
    // printf("buffer_timestamp_converted is: %d\n", buffer_timestamp_converted); 
    //printf("block2->timestamp is: %d\n", block2->timestamp);

//     int ret2 = memcmp(buffer_timestamp, block2->timestamp, sizeof(block2->timestamp));
//     if(ret2 > 0) {
//       printf("str2 is less than str1");
//    } else if(ret2 < 0) {
//       printf("str1 is less than str2");
//    } else {
//       printf("str1 is equal to str2");
//    }
//    exit(0);

    // fseek(outputBlock, 12, SEEK_CUR);
    // char s;
    // for (int i = 0; i < sizeof(temp); i++) {
    //     temp[i] = fgetc(outputBlock);
    // }
    // printf("temp is: \n");
    // for (int j = 0; j < sizeof(temp); j++) {
    //     printf("%c", temp[j]);
    // }
    // printf("\n");

    // fread(buffer_target, sizeof(double), sizeof(buffer_target), outputBlock);

    
    // fread(temp1, sizeof(double), sizeof(temp1), tempOutput);
    // fread(temp2, sizeof(double), sizeof(temp2), bufferOutput);

    // printf("%f", temp1);
    // printf("\n");
    // printf("%f", temp2);
    
    // int len = strlen(temp);
    // if( temp[len] == '\n') {
    //     temp[len] = '\0';
    // }
    // printf("size of block2->target is: %d\n", sizeof(block2->target));
    
    // printf("size of temp is: %d\n", sizeof(temp));
    // if(memcmp(&(block2->target), temp, sizeof(temp))) {
    //     printf("*******************error with target\n");
    //     // return 0;
    // }
    // exit(0);


    fread(buffer_nonce, sizeof(unsigned int), 1, outputBlock);
    if(memcmp(block2->nonce, buffer_nonce, sizeof(unsigned int))) {
        printf("*******************error with timestamp\n");
        // return 0;
    }
    exit(0);
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