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


//read file function
/* static **readFile()
{
    char fname[100];
    FILE *fp;
    memset(fname, 0, 100);
    printf("ENTER FILE NAME: ");
    gets(fname);

    //space malloc
    fopen
    size_t fileContents = fread

    char **storage = (char **)malloc(1000 * sizeof(char *));

    fp = fopen(fname, "r");
    if (fp == NULL)
    {
        printf("\nError, Unable to open the file for reading\n");
    }
    else
    {
        int cou = 0;
        while (fgets(storage[cou], 100, fp))
        {
            cou++;
        }
        //start to sort
        qsort(storage[0], cou, sizeof(char *), strcmp);
    }
    return storage;
} */

