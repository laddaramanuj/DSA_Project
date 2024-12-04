#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "RLE.h"
int main() {
    const char *inputFile = "sample.txt";
    const char *compressedFile = "compress.txt";
    const char *decompressedFile = "decompress.txt";

    compressUsingRLE(inputFile, compressedFile);
    printf("Compression using RLE completed successfully\n");

    decompressUsingRLE(compressedFile, decompressedFile);
    printf("Decompression using RLE completed successfully\n");

    return 0;
}
