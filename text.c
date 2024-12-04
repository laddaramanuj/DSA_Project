#include "logic.h"
//run command -> 
//gcc text.c minHeap.c logic.c

void compressUsingHauffText(char *inputFile, char *outputFile) {

    MinHeap *heap = createMinHeap(256);

    FILE* file = fopen(inputFile, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer;
    int freq[256] = {0};
    int i = 0;
    int total = 0;
    
    while (fread(&buffer, 1, 1, file) > 0) {
        freq[(unsigned char)buffer]++;
        //printf("'%c'", buffer);
        total++;
    }
    printf("\n");

    printf("Count of char in text file : %d\n", total);


    int count = insertInMeanHeap(heap, freq);

    i = 0;
    while (i < 256) {
        if (freq[i] > 0) {
            //printf("'%c'(%d) : %d\n", i, i, freq[i]);
        }
        i++;
    }
    
    node* root = buildHuffmanTree(heap);
    
    FILE* fd2 = fopen(outputFile, "wb");
    if (fd2 == NULL) {
        perror("Error opening output file");
        return;
    }

    //writing count of total char 
    //it will be helpful in decompression
    fwrite(&total, sizeof(int), 1, fd2); 

    printf("count of unique char in file : %d\n", count);
    //writing count of unique char in compressed file
    //it will be helpful in rebuilting hauffman tree
    fwrite(&count, sizeof(int), 1, fd2); 


    int t[16];
    //function to print each char and its lenth of its hauffman code, decimal equivalent of code
    //it will be helpful in rebuilting hauffman tree
    printCodesIntoFile(fd2, root, t, 0);
    
    //moving cursur to start to compress char one by one
    fseek(file, 0, SEEK_SET);

    unsigned char a = 0;
    compressFile(file, fd2, a);
    
    fclose(fd2); 
    fclose(file);
    freeHeap(heap);
}

void decompressingHauffText(char *inputFile, char *outputFile) {
    FILE* fd2 = fopen(inputFile, "rb");
    if (fd2 == NULL) {
        perror("Error opening compressed file for reading");
        return;
    }
    
    FILE* fd3 = fopen(outputFile, "w");
    if (fd3 == NULL) {
        perror("Error opening output file");
        fclose(fd2);
        return;
    }

    int n1, total;
    //reading how many total char are there
    fread(&total, sizeof(int), 1, fd2); 

    //reading how many unique char are there
    //then for each there is char, binary code len, binary code decimal equivalent
    //which will be read in ReBuildHuffmanTree
    fread(&n1, sizeof(int), 1, fd2); 
    ReBuildHuffmanTree(fd2, n1);


    decompressFile(fd2, fd3, total);
    
    fclose(fd2);
    fclose(fd3);
    freeHuffmanTree(tree);//tree is diclared globally
}

