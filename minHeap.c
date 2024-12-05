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

// Time complexity:
//   Best case: O(log n) - In the best case, the heap property is already satisfied, and only one comparison is needed. However, even in the best case, minHeapify still has to traverse down the tree. So, the time complexity is still O(log n) for this operation.
//   Worst case: O(log n) - The worst-case time complexity happens when the heap is completely unbalanced, requiring a full traversal of the height of the heap. In the worst case, this takes O(log n) time, as the height of a binary heap is logarithmic in terms of the number of nodes.
//   Average case: O(log n)- On average, minHeapify performs a logarithmic number of swaps and comparisons. It’s expected to traverse approximately half the height of the heap in a random heap

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

// Time complexity:
//   Best case: O(log n) - The extraction involves removing the root node and reordering the heap. In the best case, the heap property might only need one comparison and possibly one swap.
//   Worst case: O(log n) - Even in the worst case, extracting the minimum node and reordering the heap always requires a logarithmic number of operations, as the heap structure is balanced.
//   Average case: O(log n)- In most cases, extracting the minimum involves re-heapifying the heap, which requires logarithmic operations as the heap is typically balanced.

// Extract the minimum node from the heap
node* extractMin(struct MinHeap* minHeap) {
    node* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Time complexity:
//   Best case: O(log n) - In the best case, the inserted node will be placed near the root, and the heap property will only require moving the node up by a few levels. This still requires logarithmic time.
//   Worst case: O(log n) - In the worst case, the node might need to be moved all the way from the bottom of the heap to the top, requiring O(log n) comparisons and swaps.
//   Average case: O(log n)- On average, the number of swaps and comparisons to restore the heap property is logarithmic, as the height of the heap is log n.

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

// Time complexity:
//  Best Case: O(n)Even in the best case where the array is already sorted, the minHeapify function is applied to every non-leaf node, making this operation O(n). Heapifying each subtree from bottom to top ensures the heap property is satisfied. The O(n) time complexity results from the fact that most heapify operations will take linear time when building the heap from an unsorted array.
//  Average Case: O(n)This is a known result: building a heap from an unordered array using the minHeapify operation takes O(n) time on average. It involves multiple O(log n) heapify calls, but the time complexity aggregates to O(n).
//  Worst Case: O(n)The worst-case scenario occurs when the heap is completely unsorted, but still requires only O(n) time to build. This is because the heapify operation on each node is logarithmic, and the total number of heapify operations is linear.

// Build a min heap from a frequency array
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Time complexity:
//  Best Case: O(n)The function must iterate through all the nodes in the heap to free them. It requires O(n) time in all cases.
//  Average Case: O(n)Same as the best case, since we must iterate over all n nodes and free each one.
//  Worst Case: O(n)Same as the best case, because we have to free each node, and this operation doesn’t depend on the state of the heap.
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