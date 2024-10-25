#include<stdio.h>
#include<stdlib.h>
//#include<math.h>
#include <fcntl.h>   // For open()
#include <unistd.h>  // For read(), close()
//#include <stdbool.h>
#include <string.h>

// Define the structure for a node in the min-heap
typedef struct node {
    int freq;
    char ch;
    struct node* l;
    struct node* r;
} node;

typedef struct MinHeap {
    int size;// actual size / len
    int capacity;//how many element can be stored
    struct node** array;
}MinHeap;


node* newNode(char data, int freq);
MinHeap* createMinHeap(int capacity);
void swapMinHeapNode(node** a, node** b);
void minHeapify(MinHeap* minHeap, int idx);
node* extractMin(struct MinHeap* minHeap);
void insertMinHeap(MinHeap* minHeap, node* minHeapNode);
void buildMinHeap(struct MinHeap* minHeap);
void freeHeap(MinHeap *heap);
int isSizeOne(MinHeap *heap);