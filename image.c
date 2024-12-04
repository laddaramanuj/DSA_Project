#include "logic.h"

void compressingHauffImage(char *inputFile, char *outputFile) {
    MinHeap *heap = createMinHeap(256);
    int offset,hbytes,width,height;
    long bmpsize=0,bmpdataoff=0,bpp=0;
    char id[2];

    FILE *file = fopen(inputFile, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    printf("Processing BMP Header...\n\n");
    //1 - 7 -> header
    //8 -> data

    //1 -> 0 id
    offset = 0;
    fseek(file, offset, SEEK_SET);
    fread(&id[0], 1, 1, file);
    printf("%c",id[0]);
    fread(&id[1], 1, 1, file);
    printf("%c\n",id[1]);
    
    //2 -> 2 size of file
    fread(&bmpsize, 4, 1, file);
    printf("Size of the BMP File:: %ld bytes\n\n",bmpsize);

    //3 -> 10 offset for pixel array
    offset = 10;
    fseek(file, offset, SEEK_SET);
    fread(&bmpdataoff, 4, 1, file);
    printf("Bitmap data offset: %ld\n\n",bmpdataoff);

    //4 ->  14 num of bytes in header
    fread(&hbytes, 4, 1, file);
    printf("Number of bytes in header:: %d\n\n",hbytes);

    //5 -> 18 -> width of image
    fread(&width, 4, 1, file);
    printf("Width of Image: %d\n",width);

    //6 -> 22 heightof image
    fread(&height, 4, 1, file);
    printf("Height of image: %d\n\n",height);
    if(height < 0)
        height = (-1)*height;
    
    //7 -> 28 Number of bits per pixel
    fseek(file, 2, SEEK_CUR);
    fread(&bpp, 2, 1, file);
    printf("Number of bits per pixel: %ld\n\n",bpp);

    //8 -> now we will read 
    fseek(file, bmpdataoff, SEEK_SET);
    
    int numbytes = (bmpsize - bmpdataoff)/3;
    printf("Number of bytes except header: %d \n\n",numbytes);
    
    char buffer;
    int freq[256] = {0};
    int i = 0;
    int total = 0;

    while(fread(&buffer, 1, 1, file) > 0) {
        freq[(unsigned char)buffer]++;
        //printf("'%c'", buffer);
        total++;
    }
    printf("\n");
    printf("Total : %d\n", total);

    int count = insertInMeanHeap(heap, freq);

    node* root = buildHuffmanTree(heap);
    
    FILE *fd2 = fopen(outputFile, "wb");
    if(fd2 == NULL) {
        perror("Error opening output file");
        fclose(file);
        return;
    }

    //writing count of total char 
    //it will be helpful in decompression
    fwrite(&total, sizeof(int), 1, fd2); 

    printf("count : %d\n", count);
    fwrite(&count, sizeof(int), 1, fd2);

    int t[16];
    printCodesIntoFile(fd2, root, t, 0);
    
    fseek(file, 0, SEEK_SET);
    //from actual file, write header bytes upto bmpdataoff byte to fd2
    writeHeader(fd2, file, bmpdataoff);
    
    unsigned char a = 0;
    compressFile(file, fd2, a);

    fclose(file);
    fclose(fd2);
    freeHeap(heap);
}

void decompressingHauffImage(char *inputFile, char *outputFile) {

    FILE *fd2 = fopen(inputFile, "rb");
    if(fd2 == NULL) {
        perror("Error opening output file");
        return;
    }
    
    FILE *fd3 = fopen(outputFile, "w");
    if (fd3 == NULL) {
        perror("Error opening output file");
        fclose(fd2);
        return;
    }
    
    int n1, total;
    //reading how many total char are there
    fread(&total, sizeof(int), 1, fd2); 
    printf("total : %d\n", total);
    
    //reading how many unique char are there
    //then for each there is char, binary code len, binary code decimal equivalent
    //which will be read in ReBuildHuffmanTree
    fread(&n1, sizeof(int), 1, fd2); 
    printf("header bytes count : %d\n", n1);
    ReBuildHuffmanTree(fd2, n1);
    
    //write header from compressed file to decompressed as it is
    writingHeader(fd3, fd2);
    
    decompressFile(fd2, fd3, total);
    
    fclose(fd3);
    fclose(fd2);
    freeHuffmanTree(tree);//tree is diclared globally
}
