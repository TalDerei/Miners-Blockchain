/* IMPLEMENTATION OF READING USER FILE AND SORTING CONTENTS */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *Fopen(const char *file, const char *permission) {
    FILE *fp = NULL;
    fp = fopen(file, permission);
    if (fp == NULL) {
        printf("error opening!!!!!!!!");
        exit(0);
    }
    return fp;
}

size_t Fread(void * buffer, size_t size, size_t nmemb, FILE *file) {
    size_t returnBytes = fread(buffer, size, nmemb, file);

    if (returnBytes == 0) {
        printf("empty file!!!!!!!");
    }
    if (ferror(file)) {
        printf("error reading file!!!!!!");
    }
    return returnBytes;
}

int lineCount(FILE * file) {
    int counter = 0;
    char character;
    for (character = getc(file); character != EOF; character = getc(file)) {
        if (character == '\n') { 
            counter = counter + 1; 
        }
    }
    return counter; 
}  

size_t Fwrite(void *buffer, size_t size, size_t nmemb, FILE *file) {
    size_t written = 0;
    while ((written = fwrite(buffer, size, nmemb, file) == 0)) {
        if (ferror(file) | fileno(file)) {
            printf("error writing to file!!!!!!");
            exit(0);
        }
    }
    return written;
}

void Fclose(FILE *fp) {
    if (fp && !fclose(fp)) { //unable to 
        printf("error closing file");
    }
}