#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>   // For open()
#include <unistd.h>  // For read(), close()

#define MAX_SYMBOLS 256

typedef struct {
    char symbol;
    int frequency;
    double probability;
    double cumulativeProbability;
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int size;
    int totalFrequency;
} FrequencyTable;

void initFrequencyTable(FrequencyTable *table) {
    table->size = 0;
    table->totalFrequency = 0;
}

void updateFrequencyTable(FrequencyTable *table, const char *input) {
    int freq[MAX_SYMBOLS] = {0};

    // Count frequency of each symbol
    for (const char *p = input; *p; p++) {
        freq[(unsigned char)*p]++;
    }

    // Populate frequency table
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (freq[i] > 0) {
            table->symbols[table->size].symbol = (char)i;
            table->symbols[table->size].frequency = freq[i];
            table->totalFrequency += freq[i];
            table->size++;
        }
    }

    // Calculate probabilities and cumulative probabilities
    double cumulative = 0.0;
    for (int i = 0; i < table->size; i++) {
        table->symbols[i].probability = (double)table->symbols[i].frequency / table->totalFrequency;
        table->symbols[i].cumulativeProbability = cumulative;
        cumulative += table->symbols[i].probability;
    }
}

void encode(const char *input, FrequencyTable *table, double *encodedValue) {
    double low = 0.0, high = 1.0;

    for (const char *p = input; *p; p++) {
        char symbol = *p;

        for (int i = 0; i < table->size; i++) {
            if (table->symbols[i].symbol == symbol) {
                double range = high - low;
                high = low + range * (table->symbols[i].cumulativeProbability + table->symbols[i].probability);
                low = low + range * table->symbols[i].cumulativeProbability;
                break;
            }
        }
    }

    *encodedValue = (low + high) / 2; // Final encoded value
}

void decode(double encodedValue, FrequencyTable *table, int originalLength) {
    char decoded[originalLength + 1];
    decoded[originalLength] = '\0';
    double low = 0.0, high = 1.0;

    for (int i = 0; i < originalLength; i++) {
        double range = high - low;
        double scaledValue = (encodedValue - low) / range;

        for (int j = 0; j < table->size; j++) {
            if (scaledValue < table->symbols[j].cumulativeProbability + table->symbols[j].probability) {
                decoded[i] = table->symbols[j].symbol;

                high = low + range * (table->symbols[j].cumulativeProbability + table->symbols[j].probability);
                low = low + range * table->symbols[j].cumulativeProbability;
                break;
            }
        }
    }

    printf("Decoded output: %s\n", decoded);
}
/*
int main() {
    const char *input = "Payal Kotkar";
    FrequencyTable table;
    
    initFrequencyTable(&table);
    updateFrequencyTable(&table, input);

    double encodedValue;
    encode(input, &table, &encodedValue);
    printf("Encoded value: %f\n", encodedValue);

    decode(encodedValue, &table, strlen(input));

    return 0;
}
*/
int main() {
    int file = open("msg.txt", O_RDONLY);
    if (file == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char buffer;
    char *input = malloc(25600000); // Adjust size as needed
    int i = 0;

    // Read the file character by character
    while (read(file, &buffer, 1) > 0) {
        input[i++] = buffer;
    }
    input[i] = '\0'; // Null-terminate the string

    close(file); // Close the file

    FrequencyTable table;
    initFrequencyTable(&table);
    updateFrequencyTable(&table, input);

    double encodedValue;
    encode(input, &table, &encodedValue);
    printf("Encoded value: %f\n", encodedValue);

    decode(encodedValue, &table, strlen(input));

    free(input); // Free allocated memory

    return 0;
}
