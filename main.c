#include "main.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <command> <filename>\n", argv[0]);
        printf("Commands:\n");
        printf("  c1: Compress using RLE\n");
        printf("  d1: Decompress using RLE\n");
        printf("  c2: Compress text using Huffman\n");
        printf("  d2: Decompress text using Huffman\n");
        printf("  c3: Compress image using Huffman\n");
        printf("  d3: Decompress image using Huffman\n");
        return 1;
    }

    char *command = argv[1];
    char *filename = argv[2];

    if (strcmp(command, "c1") == 0) {
        //this is to handle compression using RLE
        //it will take file to be compressed
        char *output = "./image-files/compressedRle.txt";
        printf("Compressing '%s' using RLE...\n", filename);
        compressUsingRLE(filename, output);
        printf("Successfully compressed as '%s'\n\n", output);
    } 
    else if (strcmp(command, "d1") == 0) {
        //this is to handle decompression using RLE
        //it will take file to be decompressed
        char *output = "./image-files/decompressedRle.txt";
        printf("Decompressing '%s' using RLE...\n", filename);
        decompressUsingRLE(filename, output);
        printf("Successfully decompressed as '%s'\n\n", output);
    } 
    else if (strcmp(command, "c2") == 0) {
        //this is to handle compression of text file using hauffman
        //it will take file to be compressed
        char *output = "./image-files/compressedHauffText.txt";
        printf("Compressing '%s' using Huffman (text)...\n", filename);
        compressUsingHauffText(filename, output);
        printf("Successfully compressed as '%s'\n\n", output);
    } 
    else if (strcmp(command, "d2") == 0) {
        //this is to handle decompression of text using hauffman
        //it will take file to be decompressed
        char *output = "./image-files/decompressedHauffText.txt";
        printf("Decompressing '%s' using Huffman (text)...\n", filename);
        decompressingHauffText(filename, output); 
        printf("Successfully decompressed as '%s'\n\n", output);
    } 
    else if (strcmp(command, "c3") == 0) {
        //this is to handle compression of image using hauffman
        //it will take file to be compressed
        char *output = "./image-files/compressedHauffImage.bin";
        printf("Compressing '%s' using Huffman (image)...\n", filename);
        compressingHauffImage(filename, output);
        printf("Successfully compressed as '%s'\n\n", output);
    } 
    else if (strcmp(command, "d3") == 0) {
        //this is to handle decompression of image using RLE
        //it will take file to be decompressed
        char *output = "./image-files/decompressedHauffImage.bmp";
        printf("Decompressing '%s' using Huffman (image)...\n", filename);
        decompressingHauffImage(filename, output);
        printf("Successfully decompressed as '%s'\n\n", output);
    } 
    else {
        printf("Invalid command: %s\n", command);
        printf("Use one of the following commands:\n");
        printf("  c1: Compress using RLE\n");
        printf("  d1: Decompress using RLE\n");
        printf("  c2: Compress text using Huffman\n");
        printf("  d2: Decompress text using Huffman\n");
        printf("  c3: Compress image using Huffman\n");
        printf("  d3: Decompress image using Huffman\n");
        return 1;
    }

    return 0;
}
