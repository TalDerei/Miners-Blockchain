#ifndef READFILE_DEF
#define READFILE_DEF

FILE *Fopen(const char *, const char *);
void Fclose(FILE *);
size_t Fread(void *, size_t, size_t, FILE *); 
//size_t Fwrite(void *, size_t, size_t, FILE *);
char **ReadMultipleFiles(char **, int);
char **ReadOneFile(char *);
int *GetLineNumbers(char **, int);

#endif
