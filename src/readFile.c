/* IMPLEMENTATION OF READING USER FILE AND SORTING CONTENTS */
#include <stdio.h>
#include <string.h>

//read file function
static **readFile()
{
    char fname[100];
    FILE *fp;
    memset(fname, 0, 100);
    printf("ENTER FILE NAME: ");
    gets(fname);

    //space malloc
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
}