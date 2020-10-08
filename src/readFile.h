#ifndef READFILE_DEF
#define READFILE_DEF

FILE *Fopen(const char *, const char *);
void Fclose(FILE *);
size_t Fread(void *, size_t, size_t, FILE *); 
//size_t Fwrite(void *, size_t, size_t, FILE *);
//void ReadMultipleFiles(char **, char **, int);
void ReadOneFile(char**, char *);
int *GetLineNumbers(char **, int);

#endif
