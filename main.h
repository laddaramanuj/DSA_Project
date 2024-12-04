#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


void compressUsingRLE(const char *inputFile, const char *outputFile);
void decompressUsingRLE(const char *inputFile, const char *outputFile);
void compressUsingHauffText(char *inputFile, char *outputFile);
void decompressingHauffText(char *inputFile, char *outputFile);
void compressingHauffImage(char *inputFile, char *outputFile);
void decompressingHauffImage(char *inputFile, char *outputFile);