#include "minHeap.h"

// Function to create a new min heap node
node* newNode(char data, int freq) {
    node* temp = (node*)malloc(sizeof(node));
    temp->ch = data;
    temp->freq = freq;
    temp->l = temp->r = NULL;
    return temp;
}

// Function to create a min heap of given capacity
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (node**)malloc(minHeap->capacity * sizeof(node*));
    return minHeap;
}

// Function to swap two min heap nodes
void swapMinHeapNode(node** a, node** b) {
    node* t = *a;
    *a = *b;
    *b = t;
}

// Heapify at a given index
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Extract the minimum node from the heap
node* extractMin(struct MinHeap* minHeap) {
    node* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert a new node into the heap
void insertMinHeap(MinHeap* minHeap, node* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Build a min heap from a frequency array
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void freeHeap(MinHeap *heap) {
    for (int i = 0; i < heap->size; i++) {
        free(heap->array[i]); 
    }
    free(heap->array);
    free(heap);
}

int isSizeOne(MinHeap *heap) {
    return heap->size == 1 ? 1 : 0;
}