#include "main.h"

void compressUsingRLE(const char *inputFile, const char *outputFile) {
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
                if (fprintf(out, "%c%d", currentChar, count) < 0) {
                    perror("Error writing to file");
                    fclose(in);
                    fclose(out);
                    exit(1);
                }
            } else {
                if (fputc(currentChar, out) == EOF) {
                    perror("Error writing to file");
                    fclose(in);
                    fclose(out);
                    exit(1);
                }
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

void decompressUsingRLE(const char *inputFile, const char *outputFile) {
    FILE *in = fopen(inputFile, "r");
    FILE *out = fopen(outputFile, "w");
    if (in == NULL || out == NULL) {
        perror("Error opening file");
        exit(1);
    }

    while (1) {
        int head = fgetc(in);
        if (head == EOF) break;

        int nextChar = fgetc(in);
        if (nextChar == EOF) {
            if (fputc(head, out) == EOF) {
                perror("Error writing to file");
                fclose(in);
                fclose(out);
                exit(1);
            }
            break;
        }

        if (isdigit(nextChar)) {
            ungetc(nextChar, in);
            int count;
            if (fscanf(in, "%d", &count) != 1) {
                perror("Error reading count");
                fclose(in);
                fclose(out);
                exit(1);
            }
            for (int i = 0; i < count; i++) {
                if (fputc(head, out) == EOF) {
                    perror("Error writing to file");
                    fclose(in);
                    fclose(out);
                    exit(1);
                }
            }
        } else {
            if (fputc(head, out) == EOF) {
                perror("Error writing to file");
                fclose(in);
                fclose(out);
                exit(1);
            }
            ungetc(nextChar, in);
        }
    }

    fclose(in);
    fclose(out);
}
