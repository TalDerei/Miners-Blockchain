/* IMPLEMENTATION OF READING USER FILE AND SORTING CONTENTS */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readFile.h"
#include "sort.h"

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

void ReadOneFile(char** arr, char *filename)
{
    printf("file name %s\n", filename);
    FILE *fp = Fopen(filename, "r");
    int count = (int)lineCount(fp);
    printf("number of lines: %d\n", count);
    
    //***fp must be open twice OR use rewind() to reset pointer
    fp = Fopen(filename, "r"); //open the file
    int z = 0;
    while ( fgets(arr[z], 100, fp) != NULL )
    {
        size_t len = strlen(arr[z]);
        if( arr[z][len-1] == '\n') {
            arr[z][len-1] = '\0';
        }
        printf ("elements are: %s", arr[z]);
        z++;
    }
    printf("Total string put in arr is: %d\n", z);
    fclose(fp);

    sort(arr, count);   

    for (int i = 0; i < count; i++)
    {
        printf("Sorted array is: %s\n", arr[i]);
    }
}

int *GetLineNumbers(char **filename, int count) {
    int *lineNum = (int *)malloc(count*sizeof(int));
    FILE *fp;
    for(int i = 0; i < count; i++){
        fp = fopen(filename[i], "r");
        lineNum[i] = (int) lineCount(fp);
        printf("!!!!!Assigned line number %d to %d\n", i, lineNum[i]);
    }
    return lineNum;
}