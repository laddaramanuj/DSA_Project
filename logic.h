#include "minHeap.h"

#define MAX 32 

typedef struct Tree {
    char g;
    int len;
    int dec;
    struct Tree* f;
    struct Tree* r;
} Tree;

Tree *tree, *tree_temp, *t;

typedef struct code {
    char k;
    int l;
    int d;
    int code_arr[16];
    struct code* p;
} code;


int insertInMeanHeap(MinHeap* h, int* freq);
node* buildHuffmanTree(MinHeap* h);
void freeHuffmanTree(Tree* node);
int convertBinaryToDecimal(int arr[], int n);
void printCodesIntoFile(int fd2, node* root,int t[], int top );
void freeCodeList(code *head);
void writeHeader(int fd2, int file, int count);//for image compression only
void compressFile(int fd1, int fd2, unsigned char a);
void convertDecimalToBinary(int bin[], int decimal, int length);
void ExtractCodesFromFile(int fd1);//for image compression only
void ReBuildHuffmanTree(int fd1, int size);
int isroot(Tree* node);
void decompressFile(int fd1, int fd2, int f);
void decompressFileGreyScale(int fd1, int fd2, int f);
void writingHeader(int fd3, int fd2);
void freeImage(unsigned char** image, int height);
void freeData(unsigned char** image, int height, MinHeap *heap);
void closeAllFiles(int file, int fd2, int fd3);