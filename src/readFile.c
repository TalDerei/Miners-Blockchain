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

char **ReadOneFile() {

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
    return arr;
}

char **ReadMultipleFiles(int count) {
    char ** multiplefilecontents = malloc(count * sizeof(FILE *)); //malloc count * 8 BYTES 
    int i;
    for (i = 0; i < count; i++) {
        multiplefilecontents[i] = ReadOneFile();
    }
    return multiplefilecontents;
}

//array of char **