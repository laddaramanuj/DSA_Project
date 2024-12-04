#include "minHeap.h"

#define MAX 32 

//structure for node in hauffman tree
typedef struct Tree {
    char g;
    int len;
    int dec;
    struct Tree* f;
    struct Tree* r;
} Tree;

//it is structure for node in linked-list
typedef struct code {
    char k;
    int l;
    int d;
    int code_arr[16];
    struct code* p;
} code;


Tree *tree, *tree_temp, *t;

int insertInMeanHeap(MinHeap* h, int* freq);
node* buildHuffmanTree(MinHeap* h);
void freeHuffmanTree(Tree* node);
int convertBinaryToDecimal(int arr[], int n);
void printCodesIntoFile(FILE* fd2, node* root,int t[], int top );
void freeCodeList(code *head);
void writeHeader(FILE* fd2, FILE* file, int count);//for image compression only
void compressFile(FILE* fd1, FILE* fd2, unsigned char a);
void convertDecimalToBinary(int bin[], int decimal, int length);
void ExtractCodesFromFile(FILE* fd1);//for image compression only
void ReBuildHuffmanTree(FILE* fd1, int size);
int isLeaf(Tree* node);
void decompressFile(FILE* fd1, FILE* fd2, int f);
void writingHeader(FILE* fd3, FILE* fd2);
