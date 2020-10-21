/* IMPLEMENTATION FOR SORTING ALGORITHM -- BUBBLE SORT */

#include <stdio.h>
#include <string.h>
#include "merkleTree.h"
#include "sort.h"

// Double Bubble-Sort: sorting by length and alphanumeric 
void sort(char** arr, int n) { 
   int i, j; 
   for (i = 0; i < n-1; i++)
   {
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
