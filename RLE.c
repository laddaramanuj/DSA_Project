#include "main.h"

/**
 * run command -> 
 *      ./a.out c1 <file-path>
 *      ./a.out d1 ./image-files/compressedRle.txt
 */


/*

    Time Complexity: 
        O(n), where n is the number of characters in the input file. 
        The function processes each character exactly once.
    Space Complexity:
         O(n), where n is the number of characters in the input file, 
         primarily due to the output file storage.

*/
// Function to perform Run-Length Encoding (RLE) compression
void compressUsingRLE(const char *inputFile, const char *outputFile) {
    // Open input file for reading
    FILE *in = fopen(inputFile, "r");
    // Open output file for writing
    FILE *out = fopen(outputFile, "w");

    // Check if files were opened successfully
    if (in == NULL || out == NULL) {
        perror("Error opening file");
        exit(1); // Exit if file cannot be opened
    }

    // Read the first character from the input file
    int currentChar = fgetc(in);
    // If the file is empty, no compression is needed
    if (currentChar == EOF) {
        fclose(in);  // Close input file
        fclose(out); // Close output file
        return;
    }

    int count = 1;  // Initialize the count of consecutive characters

    // Main loop to process the file character by character
    while (1) {
        // Read the next character from the input file
        int nextChar = fgetc(in);

        // Check if the end of file is reached or characters are different
        if (nextChar == EOF || nextChar != currentChar) {
            // If the count is greater than 1, write the character and its count
            if (count > 1) {
                // Write the character and the count to the output file
                if (fprintf(out, "%c%d", currentChar, count) < 0) {
                    perror("Error writing to file");
                    fclose(in);  // Close input file
                    fclose(out); // Close output file
                    exit(1);     // Exit if write fails
                }
            } else {
                if (fputc(currentChar, out) == EOF) {
                    perror("Error writing to file");
                    fclose(in);  // Close input file
                    fclose(out); // Close output file
                    exit(1);     // Exit if write fails
                }
            }

            // If we reached the end of the file, exit the loop
            if (nextChar == EOF) break;

            // Set currentChar to nextChar and reset count to 1
            currentChar = nextChar;
            count = 1;
        } else {
            // If characters are the same, increment the count
            count++;
        }
    }
    // Close the input and output files
    fclose(in);
    fclose(out);
}



/*

    Time Complexity: 
        O(n), where n is the number of characters in the input file. 
        The function processes each character exactly once, and each operation (reading, processing, writing) is done in constant time.
    Space Complexity: 
        O(n), where n is the number of characters in the decompressed output file. 
        The space is required to store the output file, which could be larger than the input file in the case of high repetition counts.

*/


// Function to perform Run-Length Encoding (RLE) decompression
void decompressUsingRLE(const char *inputFile, const char *outputFile) {
    // Open the input file for reading
    FILE *in = fopen(inputFile, "r");
    // Open the output file for writing
    FILE *out = fopen(outputFile, "w");

    // Check if files are opened successfully
    if (in == NULL || out == NULL) {
        perror("Error opening file");
        exit(1);  // Exit if file cannot be opened
    }

    // Main loop to process the compressed file
    while (1) {
        // Read the character representing the symbol
        int head = fgetc(in);
        if (head == EOF) break;  // Exit the loop if EOF is reached

        // Read the next character, which could be the digit or part of it
        int nextChar = fgetc(in);
        if (nextChar == EOF) {
            // If there's no digit after the character, just write the symbol once
            if (fputc(head, out) == EOF) {
                perror("Error writing to file");
                fclose(in);  // Close input file
                fclose(out); // Close output file
                exit(1);     // Exit if writing fails
            }
            break;  // End of file reached, exit
        }
       
        // Check if the next character is a digit (count for repetitions) 
        if (isdigit(nextChar)) {
            // If the next character is a digit, put it back to the stream
            ungetc(nextChar, in);
            int count;
            // Read the number (repetition count) from the input file
            if (fscanf(in, "%d", &count) != 1) {
                perror("Error reading count");
                fclose(in);  // Close input file
                fclose(out); // Close output file
                exit(1);     // Exit if writing fails
            }
            // Write the character 'head' to the output file 'count' times
            for (int i = 0; i < count; i++) {
                if (fputc(head, out) == EOF) {
                    perror("Error writing to file");
                    fclose(in);  // Close input file
                    fclose(out); // Close output file
                    exit(1);     // Exit if writing fails
                }
            }
        } else {
            // If the next character is not a digit, just write the symbol once
            if (fputc(head, out) == EOF) {
                perror("Error writing to file");
                fclose(in);  // Close input file
                fclose(out); // Close output file
                exit(1);     // Exit if writing fails
            }
            // Put the non-digit character back into the input stream
            ungetc(nextChar, in);
        }
    }

    // Close the input and output files
    fclose(in);
    fclose(out);
}
