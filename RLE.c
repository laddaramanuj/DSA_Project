#include <stdio.h>
#include <stdlib.h>

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
int main() {
    const char* inputFile = "msg.txt";       // Input file name
    const char* outputFile = "output.txt";   // Output file name

    // Call the RLE compression function with input and output file names
    compressUsingRLE(inputFile, outputFile);
    return 0;
}
/*
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    compressUsingRLE(argv[1], argv[2]);
    return 0;
}
*/
