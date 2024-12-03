#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "RLE.h"
int main() {
    const char *inputFile = "data.txt";
    const char *outputFile = "compress.txt";
    const char *decompressedFile = "decompress.txt";

    compressUsingRLE(inputFile, outputFile);
    printf("Compression using RLE completed successfully\n");

    decompressUsingRLE(outputFile, decompressedFile);
    printf("Decompression using RLE completed successfully\n");

    return 0;
}
