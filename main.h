#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 
#include <fcntl.h>   
#include <unistd.h>  
#include <stdbool.h>
#include <string.h>

#define MAX 32

typedef struct Tree {
    char g;
    int len;
    int dec;
    struct Tree* f;
    struct Tree* r;
} Tree;

typedef struct node {
    int freq;
    char ch;
    struct node* l;
    struct node* r;
} node;

typedef struct MinHeap {
    int size;
    int capacity;
    struct node** array;
} MinHeap;

typedef struct code {
    char k;
    int l;
    int d;
    int code_arr[16];
    struct code* p;
} code;

node* newNode(char data, int freq);
MinHeap* createMinHeap(int capacity);
void swapMinHeapNode(node** a, node** b);
void minHeapify(MinHeap* minHeap, int idx);
node* extractMin(struct MinHeap* minHeap);
void insertMinHeap(MinHeap* minHeap, node* minHeapNode);
void buildMinHeap(struct MinHeap* minHeap);
void freeHeap(MinHeap *heap);
int isSizeOne(MinHeap *heap);

int insertInMeanHeap(MinHeap* h, int* freq);
node* buildHuffmanTree(MinHeap* h);
int convertBinaryToDecimal(int arr[], int n);
void printCodesIntoFile(int fd2, node* root, int t[], int top);
void compressFile(int fd1, int fd2, unsigned char a);
void convertDecimalToBinary(int bin[], int decimal, int length);
void ExtractCodesFromFile(int fd1);
void ReBuildHuffmanTree(int fd1, int size);
int isroot(Tree* node);
void decompressFile(int fd1, int fd2, int f);
void freeHuffmanTree(Tree* node);
void freeCodeList(code *head);

#endif
