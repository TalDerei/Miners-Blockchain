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