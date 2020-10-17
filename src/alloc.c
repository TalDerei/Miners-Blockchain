/* CONTAINS ENTIRE CODEBASE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "sha256.h"
#include "merkleTree.h"
#include "node.h"

char *hash(char arr[]) {
    printf ("content passed in: %s\n", arr);

    BYTE *buf = malloc(sizeof(BYTE));
	SHA256_CTX ctx;

    //initialize, update, and final functions to produce hash digest
	sha256_init(&ctx); 
	sha256_update(&ctx, arr, strlen(arr));
	sha256_final(&ctx, buf);
    return buf;
}

prog: main.o merkleTree.o readFile.o sort.o sha256.o hash.o
	gcc -g -o prog merkleTree.o readFile.o sort.o sha256.o hash.o main.o

sort.o: merkleTree.h sort.c
	gcc -g -Wall -Wextra -Wwrite-strings -c sort.c

readFile.o: merkleTree.h node.h sha256.h readFile.c
	gcc -g -Wall -Wextra -Wwrite-strings -c readFile.c

merkleTree.o: merkleTree.h node.h sha256.h merkleTree.c 
	gcc -g -Wall -Wextra -Wwrite-strings -c merkleTree.c

sha256.o: sha256.h sha256.c
	gcc -g -Wall -Wextra -Wwrite-strings -c sha256.c

hash.o: merkletree.h hash.c
	gcc -g -Wall -Wextra -Wwrite-strings -c hash.c

clean:
	rm -f prog
	rm -f *.o

/* IMPLEMENTATION OF MERKLE TREE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "node.h"
#include "merkleTree.h"
#include "sha256.h"
#define BUFFER 100

LeafNode *createLeafNodes(LeafNode *leafnode, char **SortedArray, int count) {
	for(int i = 0; i < count; i++){
		printf("----\n");
        strcpy(leafnode[i].value, SortedArray[i]);
		printf("LeafNode value \n%s\n", leafnode[i].value);
		//unsigned char *returned_str = hash(SortedArray[i]);
		/* for (int n = 0; n < SHA256_BLOCK_SIZE; n++) {
       		printf("%x", (unsigned char)(returned_str[n]));
			//printf("LeafNode hash \n %x\n", leafnode[i].hash); 
		}  */
		/*for (int j = 0; j < SHA256_BLOCK_SIZE; j++) { //wtf, cannot 
			strcat(leafnode[i].hash, (unsigned char)(returned_str[j]));
		}*/
        //strcpy(leafnode[i].hash, *returned_str);//will need to apply the hash function here
		strcpy(leafnode[i].hash, SortedArray[i]);
		printf("\nLeafNode hash \n %x\n", leafnode[i].hash);
    }
    return leafnode;
}

InternalNode *convertLeaftoInternal(LeafNode *LeafNode, int count){
	InternalNode *newInternal = malloc(count * sizeof(InternalNode));
	for(int i = 0; i < count; i++){
		strcpy(newInternal[i].hash, &LeafNode[i].hash);
		strcpy(newInternal[i].leftEdge, &LeafNode[i].value);
		strcpy(newInternal[i].rightEdge, &LeafNode[i].value);
	}
	return newInternal;
}

InternalNode *merkleTreeRoot(InternalNode *leafNodes, int count){
	//get ceiling
	printf("Count is: %d\n",count);
	int parents = count/2 + count%2;
	InternalNode *newInternal = malloc(((count/2)+(count%2))*sizeof(InternalNode));
	
	int j = 0;
	for(int i = 0; i < count; i+=2){
		printf("\nI = : %d\n",i);
		if(i != count-1){
			char *temp = &(leafNodes[i].hash);
			printf("\ntemp is: %s\n" , temp);
			printf("\ni+1 is: %s\n" , leafNodes[i+1].hash);
			strcpy(newInternal[j].hash, strcat(temp, leafNodes[i+1].hash));
			printf(".....temp after strcat is: %s", temp);
			printf("\nStrcat test: %s\n",newInternal[j].hash);
			//printf("dereference of newinternal is: *s compare to the origional: %s",*newInternal[j].hash);
			newInternal[j].leftChild = &leafNodes[i].hash;
			newInternal[j].rightChild = &leafNodes[i+1].hash;
			strcpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge);
			printf("\n---------The left edge is: %s\n",newInternal[j].leftEdge);
			strcpy(newInternal[j].rightEdge, &leafNodes[i+1].rightEdge);
			printf("\n---------The right edge is: %s\n",newInternal[j].rightEdge);
		}else{
			char *temp = &(leafNodes[i].hash);
			printf("\nZZZZZZZZZZZ ODD CASE: temp is: %s\n",temp);
			strcpy(newInternal[j].hash,temp);//will need to apply the hash function here
			newInternal[j].leftChild = &leafNodes[i];
			newInternal[j].rightChild = NULL;
			strcpy(newInternal[j].rightEdge, &leafNodes[i].rightEdge);
			strcpy(newInternal[j].leftEdge, &leafNodes[i].leftEdge);
			printf("\n---------The right edge is: %s\n",newInternal[j].rightEdge);
		}
		j++;
		if(parents == 1){
			printf("\n*****Root is: %s\n", newInternal[0].hash);
			printf("\n*****Root left edge  is: %s\n", newInternal[0].leftEdge);
			printf("\n*****Root right edge  is: %s\n", newInternal[0].rightEdge);
			return newInternal;
		}
	}
	return merkleTreeRoot(newInternal, parents);
}

void print_merkle_tree(InternalNode *root, int ID) {
	printf("\n ------------------------- leafNodes are: %s \n", root->hash[0]);
	printf(root->leftChild);
	if (root->leftChild != NULL) {
		print_merkle_tree(root->leftChild, 2*ID);
	}
	if (root->rightChild != NULL) {
		print_merkle_tree(root->rightChild, 2*ID+1);
	}
} 

/* free_merkle_tree(InternalNode *internalNodes) {
	free(internalNodes);
} */


/* ROOT MAIN.c 
CONTROL FLOW: (1) main will call function (e.g. read_input_file() inside inputFile.c to read-in 
		     input file (plaintext) from user, and sort the file's contents with an algorithm 
		     with efficient time complexity
		 (2) main will then call function (e.g. build_tree() inside of merkleTree.c) to build
		     an empty merkle tree.
		 (3) main will then call a function (e.g. populate_tree() inside of merkleTree.c) in order
		     to start populating the tree with the set of string inputted by the user.
		 (4) From merkleTree.c, we'll have to call the hash function (e.g. hash() inside of sha_256.c)
		     in order to hash the input, and then placing the hash inside the specified node
		 (5) main will call a function (printTree() inside of merkleTree.c) in order to print out the tree
		 (6) main will call a function (freeTree() inside of merkleTree.c) in order to free memory
		     allocated to the merkleTree and its nodes.  
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merkleTree.h"
#include "sha256.h"
#include "node.h"
#define BUFFER 100

int main(int argc, char *argv[]) {
    char input[BUFFER];
    printf("input the filename to be opened : ");
	scanf("%s",input);	
    
    FILE *fp = Fopen(input, "r"); 
    int count = (int) lineCount(fp);
    printf("number of lines: %d\n", count);
    char** arr = malloc(count * sizeof(char*));
    for(int i = 0; i < count; i++){
      arr[i] = malloc(100);
    }

    fp = Fopen(input, "r"); //open the file
    int z = 0;
    while (fgets(arr[z], 100, fp) != NULL) {
        size_t len = strlen(arr[z]);
        if( arr[z][len-1] == '\n') {
            arr[z][len-1] = '\0';
        }
        printf ("elements are: %s", arr[z]);
        z++;
    }
    printf("Total string put in arr is: %d\n",z);
    Fclose(fp);

    Sort(arr, count);

    for(int i = 0; i < count; i++){
      printf("Sorted array is: %s\n", arr[i]);
    }

    LeafNode *leafNodes = malloc(count*sizeof(LeafNode));
    createLeafNodes(leafNodes, arr, count);
    InternalNode *internalNode = malloc(count*sizeof(InternalNode));
    internalNode = convertLeaftoInternal(leafNodes,count);
    InternalNode *TreeRoot = malloc(sizeof(InternalNode));
    TreeRoot = merkleTreeRoot(internalNode,count);
    printf("\n**The returned value from merkleTreeRoot is %s\n",TreeRoot);
    //print_merkle_tree(TreeRoot, 1);


    //FILE *output = Fopen(strncat(output,".out.txt", 1), "w");    
    //Fclose(output);
    //free_merkle_tree(leafNodes);
}

/* HEADER FILE FOR FUCNTIONS ASSOCIATED WITH MERKLE TREE */
#include "node.h"

LeafNode *createLeafNodes(LeafNode *, char**, int);
InternalNode *convertLeaftoInternal(LeafNode *LeafNode, int count);
InternalNode *merkleTreeRoot(InternalNode *, int);
void print_merkle_tree(InternalNode *, int);
FILE *Fopen(const char *, const char *);
void Fclose(FILE *);
size_t Fread(void *, size_t, size_t, FILE *); 
size_t Fwrite(void *, size_t, size_t, FILE *);
void Sort(char**, int);
char* hash(char arr[]);

/* NODE STRUCTURE */	

#ifndef NODE_DEF
#define NODE_DEF

//leaf node struct
struct leaf_node {
  char hash[64];
  char value[100];
};
typedef struct leaf_node LeafNode;

//internal node struct
struct node {
  char hash[32];
  struct node *leftChild;
  struct node *rightChild;
  char leftEdge[100];
  char rightEdge[100];
};
typedef struct node InternalNode;

#endif

/* IMPLEMENTATION OF READING USER FILE AND SORTING CONTENTS */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merkleTree.h"

FILE *Fopen(const char *file, const char *permission) {
    FILE *fp = fopen(file, permission);
    if (fp == NULL) {
        printf("error opening!!!!!!!!");
        exit(0);
    }
    return fp;
}

void Fclose(FILE *fp) {
    if (fp && !fclose(fp)) {
        printf("error closing file");
    }
}

size_t Fread(void * buffer, size_t size, size_t nmemb, FILE *file) {
    size_t readBytes = fread(buffer, size, nmemb, file);
    if (readBytes == 0) {
        printf("empty file!!!!!!!");
    }
    if (ferror(file)) {
        printf("error reading file!!!!!!");
    }
    return readBytes;
}

size_t Fwrite(void *buffer, size_t size, size_t nmemb, FILE *file) {
    size_t writtenBytes = 0;
    while ((writtenBytes = fwrite(buffer, size, nmemb, file) == 0)) {
        if (ferror(file) | fileno(file)) {
            printf("error writing to file!!!!!!");
            exit(0);
        }
    }
    return writtenBytes;
}

int lineCount(FILE * inputFile) {
    int counter = 0;
    char character;
    for (character = getc(inputFile); character != EOF; character = getc(inputFile)) {
        if (character == '\n') { 
            counter = counter + 1; 
        }
    }
    return counter; 
}  

/*********************************************************************
* Filename:   sha256.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the SHA-256 hashing algorithm.
              SHA-256 is one of the three algorithms in the SHA2
              specification. The others, SHA-384 and SHA-512, are not
              offered in this implementation.
              Algorithm specification can be found here:
               * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
			   * https://www.daemon-systems.org/man/SHA256_Update.3.html
              This implementation uses little endian byte order.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdlib.h>
#include <memory.h>
#include "sha256.h"

/****************************** MACROS ******************************/
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

/**************************** VARIABLES *****************************/
static const WORD k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

/*********************** FUNCTION DEFINITIONS ***********************/
void sha256_transform(SHA256_CTX *ctx, const BYTE data[])
{
	WORD a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for ( ; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len)
{
	WORD i;

	for (i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			sha256_transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void sha256_final(SHA256_CTX *ctx, BYTE hash[])
{
	WORD i;

	i = ctx->datalen;

	// Pad whatever data is left in the buffer.
	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		sha256_transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	sha256_transform(ctx, ctx->data);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i = 0; i < 4; ++i) {
		hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

/*********************************************************************
* Filename:   sha256.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding SHA1 implementation.
*********************************************************************/

#ifndef SHA256_H
#define SHA256_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int  WORD;             // 32-bit word, change to "long" for 16-bit machines

typedef struct {
	BYTE data[64];
	WORD datalen;
	unsigned long long bitlen;
	WORD state[8];
} SHA256_CTX;

/*********************** FUNCTION DECLARATIONS **********************/
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len);
void sha256_final(SHA256_CTX *ctx, BYTE hash[]);

#endif   // SHA256_H

#include <stdio.h>
#include <string.h>
#include "merkleTree.h"
#include "node.h"

/* Double Bubble-Sort: sorting by length and alphanumeric */
void Sort(char** arr, int n) { 
   int i, j; 
   for (i = 0; i < n-1; i++)
   {
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)
       {  
          if(strlen(arr[j]) >= strlen(arr[j+1]))
          {
            char* temp = arr[j]; 
            arr[j] = arr[j+1]; 
            arr[j+1] = temp;
          }
       }
   }
   for (i = 0; i < n-1; i++)
   {
       for (j = 0; j < n-i-1; j++)
       {  
          if(strlen(arr[j]) >= strlen(arr[j+1]))
          {
            if (strcmp(arr[j], arr[j+1]) > 0)
            {
              char* temp = arr[j]; 
              arr[j] = arr[j+1]; 
              arr[j+1] = temp;
            }
          }
       }
   }
} 