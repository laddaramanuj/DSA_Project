#include "logic.h"

int main() {
    MinHeap *heap = createMinHeap(256);
    int file = open("msg.txt", O_RDONLY);
    if (file == -1) {
        perror("Error opening file");
        return 1;
    }
    char buffer;
    int freq[256] = {0};
    int i = 0;
    int total = 0;
    while(read(file, &buffer, 1) > 0) {
        freq[(unsigned char)buffer]++;
        //printf("'%c'", buffer);
        total++;
    }
    printf("\n");
    printf("Total : %d\n", total);

    int count = insertInMeanHeap(heap, freq);

    i = 0;
    while(i < 256) {
        if(freq[i] > 0) {
            //printf("'%c'(%d) : %d\n", i, i, freq[i]);
        }
        i++;
    }

    //printf("above built\n");
    node* root = buildHuffmanTree(heap);
    

    int fd2 = open("compressedText.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 == -1) {
        perror("Error opening output file");
        return 1;
    }
    
    
    printf("count : %d\n", count);
    write(fd2, &count, sizeof(int));
    int t[16];
    printCodesIntoFile(fd2, root, t, 0);
    
    lseek(file, 0, SEEK_SET);
    unsigned char a = 0;
    compressFile(file, fd2, a);

    lseek(fd2, 0, SEEK_SET);
    fd2 = open("compressedText.bin", O_RDONLY);
    if (fd2 == -1) {
        perror("Error opening compressed file for reading");
        return 1;
    }
    
    int fd3 = open("DecompressedText.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd3 == -1) {
        perror("Error opening output file");
        return 1;
    }
    
    int n1;
    read(fd2, &n1, sizeof(int));
    ReBuildHuffmanTree(fd2, n1);
    printf("i have built tree\n");
    decompressFile(fd2, fd3, total);
    printf("i have decompressed\n");

    closeAllFiles(file, fd2, fd3);
    freeData(NULL, 0, heap);
    return 0;
}
