#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
void compressUsingRLE(const char* inputFile, const char* outputFile) {
    FILE *in = fopen(inputFile, "r");
    FILE *out = fopen(outputFile, "w");
    
    if (in == NULL || out == NULL) {
        perror("Error opening file");
        exit(1);
    }

    int currentChar = fgetc(in);
    if (currentChar == EOF) {
        fclose(in);
        fclose(out);
        return;
    }

    int count = 1;
    while (1) {
        int nextChar = fgetc(in);
        
        if (nextChar == EOF || nextChar != currentChar) {
            if (count > 1) {
                fprintf(out, "%c%d", currentChar, count);
            } else {
                fputc(currentChar, out);
            }

            if (nextChar == EOF) break;

            currentChar = nextChar;
            count = 1;
        } else {
            count++;
        }
    }

    fclose(in);
    fclose(out);
}
void decompressUsingRLE( char* inputFile, char* outputFile) {
    FILE *in = fopen(inputFile, "r");
    FILE *out = fopen(outputFile, "w");
    if (in == NULL || out == NULL) {
        perror("Error opening file");
        exit(1);
    }

    while(1) {
        int head = fgetc(in);
        if (head == EOF) break;

        int nextChar = fgetc(in);
        if (nextChar == EOF) {
            fputc(head, out);
            break;
        }

        if (isdigit(nextChar)) {
            ungetc(nextChar, in);
            int count;
            fscanf(in, "%d", &count); 
            for (int i = 0; i < count; i++) {
                fputc(head, out);
            }
        } else {
            fputc(head, out);
            ungetc(nextChar, in); 
        }
    }

    fclose(in);
    fclose(out);
}
int main(){
    const char* inputFile = "sample.txt";       // Input file name
    const char* outputFile = "output.txt";   // Output file name
    const char* decompressedfile = "decompress.txt";
    // Call the RLE compression function with input and output file names
    compressUsingRLE(inputFile, outputFile);
    printf("Compression using RLE completed successfully\n");
    decompressUsingRLE(outputFile, decompressedfile);
    printf("Decompression using RLE completed successfully\n");
    return 0;
}
