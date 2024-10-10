#include <stdio.h>
#include <stdlib.h>
#include <limits.h> 
#include <fcntl.h>   // For open()
#include <unistd.h>  // For read(), close()
#include <stdbool.h>
#include <string.h>
#include "main.h"

#define MAX 32 


typedef struct Tree {
    char g;
    int len;
    int dec;
    struct Tree* f;
    struct Tree* r;
} Tree;


Tree *tree, *tree_temp, *t;

// Define the structure for a node in the min-heap
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
}MinHeap;

typedef struct code {
    char k;
    int l;
    int d;
    int code_arr[16];
    struct code* p;
} code;


// Function prototypes
node* newNode(char data, int freq);
MinHeap* createMinHeap(int capacity);
void swapMinHeapNode(node** a, node** b) ;
void minHeapify(MinHeap* minHeap, int idx) ;
node* extractMin(struct MinHeap* minHeap);
void insertMinHeap(MinHeap* minHeap, node* minHeapNode);
void buildMinHeap(struct MinHeap* minHeap);
void freeHeap(MinHeap *heap);
int isSizeOne(MinHeap *heap);

//freq contain frequncy of char 'ch' at freq[ch]
//creat min-heap using this, we need function
    //insert
    //delete
    //and other required for this like empty, etc

int insertInMeanHeap(MinHeap* h, int* freq) {
    int i = 0, count = 0;
    while(i < 256) {
        if(freq[i] > 0) {
            insertMinHeap(h, newNode(i, freq[i]));
            count++;
        }
        i++;
    }
    return count;
}

// Function to build Huffman Tree
node* buildHuffmanTree(MinHeap* h){
    node *l, *r, *top;
    while(!isSizeOne(h)) {
        l = extractMin(h);
        r = extractMin(h);
        top = newNode('$', l->freq + r->freq);
        top->l = l;
        top->r = r;
        insertMinHeap(h, top);
    }
    //printf("i am out of loop in built\n");
    return extractMin(h);
}

int convertBinaryToDecimal(int arr[], int n) {
    int decimal = 0;
    for (int i = 0; i < n; i++) {
        decimal = (decimal << 1) | arr[i];
    }
    return decimal;
}


int k = 0;
code *front = NULL, *rear = NULL;

// Function to print codes into file
void printCodesIntoFile(int fd2, node* root,int t[], int top ) {
    int i;
    if (root->l) {
        t[top] = 0;
        printCodesIntoFile(fd2, root->l, t, top + 1);
    }

    if (root->r) {
        t[top] = 1;
        printCodesIntoFile(fd2, root->r, t, top + 1);
    }

    if (root->l == root->r) {
        code *data = (code*)malloc(sizeof(code));
        //tree = (Tree*)malloc(sizeof(Tree));
        data->p = NULL;
        data->k = root->ch;
        //tree->g = root->character;
        write(fd2, &root->ch, sizeof(char));
        printf("code for '%c' : ", data->k);
        for (i = 0; i < top; i++) {
            data->code_arr[i] = t[i];
            printf("%d ", t[i]);
        }
        printf("\n");
        data->l = top;
        //char len_str[16]; // Buffer for length
        //sprintf(len_str, "%d", top); // Format length as string
        //write(fd2, len_str, strlen(len_str)); // Write length
        write(fd2, &data->l, sizeof(int)); // Write length

        // Convert the binary code to decimal and write it
        data->d = convertBinaryToDecimal(t, top); // Compute decimal value from binary
        //char dec_str[16]; // Buffer for decimal
        //sprintf(dec_str, "%d", data->d); // Format decimal as string
        //write(fd2, dec_str, strlen(dec_str)); // Write decimal value
        write(fd2, &data->d, sizeof(int)); // Write decimal value
        if (k == 0) {
            front = rear = data;
            k++;
        }
        else {
            rear->p = data;
            rear = rear->p;
        }
    }
}

// Function to compress file
void compressFile(int fd1, int fd2, unsigned char a)
{
    char n;
    int h = 0, i;

    // Codes are written into file in bit by bit format
    while (read(fd1, &n, sizeof(char)) != 0) {
        rear = front;
        while (rear->k != n && rear->p != NULL) {
            rear = rear->p;
        }
        if (rear->k == n) {
            for (i = 0; i < rear->l; i++) {
                if (h < 7) {
                    if (rear->code_arr[i] == 1) {
                        a++;
                        a = a << 1;
                        h++;
                    }
                    else if (rear->code_arr[i] == 0) {
                        a = a << 1;
                        h++;
                    }
                }
                else if (h == 7) {
                    if (rear->code_arr[i] == 1) {
                        a++;
                        h = 0;
                    }
                    else {
                        h = 0;
                    }
                    write(fd2, &a, sizeof(char));
                    a = 0;
                }
            }
        }
    }
    for (i = 0; i < 7 - h; i++) {
        a = a << 1;
    }
    write(fd2, &a, sizeof(char));
    //printf("char : '%c'\n", a);
}


void convertDecimalToBinary(int bin[], int decimal, int length) {
    //printf("len = %d\n", length);
    for (int i = length - 1; i >= 0; i--) {
        //printf("I am in convert to binary loop\n");
        bin[i] = decimal % 2;
        decimal /= 2;
    }
}

void ExtractCodesFromFile(int fd1)
{
    read(fd1, &t->g, sizeof(char));
    read(fd1, &t->len, sizeof(int));
    read(fd1, &t->dec, sizeof(int));
}

// Function to rebuild the Huffman tree
void ReBuildHuffmanTree(int fd1, int size) {
    int i = 0, j, k;
    tree = (Tree*)malloc(sizeof(Tree));
    tree_temp = tree;
    tree->f = NULL;
    tree->r = NULL;
    t = (Tree*)malloc(sizeof(Tree));
    t->f = NULL;
    t->r = NULL;
    for (k = 0; k < size; k++) {
        tree_temp = tree;
        ExtractCodesFromFile(fd1);
        //printf("'%c' : %d\n", t->g, t->dec);
        int bin[MAX], bin_con[MAX];
        for (i = 0; i < MAX; i++) {
            bin[i] = bin_con[i] = 0;
        }
        //printf("i converted into binary\n");
        convertDecimalToBinary(bin, t->dec, t->len);
        for (i = 0; i < t->len; i++) {
            bin_con[i] = bin[i];
        }
        
        for (j = 0; j < t->len; j++) {
            if (bin_con[j] == 0) {
                if (tree_temp->f == NULL) {
                    tree_temp->f
                        = (Tree*)malloc(sizeof(Tree));
                }
                tree_temp = tree_temp->f;
            }
            else if (bin_con[j] == 1) {
                if (tree_temp->r == NULL) {
                    tree_temp->r
                        = (Tree*)malloc(sizeof(Tree));
                }
                tree_temp = tree_temp->r;
            }
        }
        tree_temp->g = t->g;
        tree_temp->len = t->len;
        tree_temp->dec = t->dec;
        tree_temp->f = NULL;
        tree_temp->r = NULL;
        tree_temp = tree;
    }
}

int isroot(Tree* node){
    return (node->f == NULL && node->r == NULL);
}

void decompressFile(int fd1, int fd2, int f) {
    int inp[8], i, k = 0;
    unsigned char p;
    read(fd1, &p, sizeof(char));
    convertDecimalToBinary(inp, p, 8);
    tree_temp = tree;
    for (i = 0; i < 8 && k < f; i++) {
        if (!isroot(tree_temp)) {
            if (i != 7) {
                if (inp[i] == 0) {
                    tree_temp = tree_temp->f;
                }
                if (inp[i] == 1) {
                    tree_temp = tree_temp->r;
                }
            }
            else {
                if (inp[i] == 0) {
                    tree_temp = tree_temp->f;
                }
                if (inp[i] == 1) {
                    tree_temp = tree_temp->r;
                }
                if (read(fd1, &p, sizeof(char)) != 0) {
                    convertDecimalToBinary(inp, p, 8);
                    i = -1;
                }
                else {
                    break;
                }
            }
        }
        else {
            k++;
            write(fd2, &tree_temp->g, sizeof(char));
            //printf("'%c'\n", tree_temp->g);
            tree_temp = tree;
            i--;
        }
    }
}

void freeHuffmanTree(Tree* node) {
    if (node) {
        freeHuffmanTree(node->f); // Free left subtree
        freeHuffmanTree(node->r); // Free right subtree
        free(node);               // Free current node
    }
}

void freeCodeList(code *head) {
    code *temp;
    while (head != NULL) {
        temp = head;
        head = head->p; // Move to the next node
        free(temp);     // Free the current node
    }
}

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
        printf("%c", buffer);
        total++;
    }
    printf("\n");

    int count = insertInMeanHeap(heap, freq);

    i = 0;
    while(i < 256) {
        if(freq[i] > 0) {
            printf("%c : %d\n", i, freq[i]);
        }
        i++;
    }

    printf("above built\n");
    node* root = buildHuffmanTree(heap);
    

    int fd2 = open("compressed1.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

    close(fd2);//writing to compressed file is done

    fd2 = open("compressed1.bin", O_RDONLY);
    if (fd2 == -1) {
        perror("Error opening compressed file for reading");
        return 1;
    }
    
    int fd3 = open("Decompressed1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

    close(file);
    close(fd2);
    close(fd3);

    freeHuffmanTree(tree);
    freeCodeList(front);
    freeHeap(heap);
    return 0;
}

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