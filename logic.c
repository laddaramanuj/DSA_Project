#include "logic.h"

int k = 0;
code *front = NULL, *rear = NULL;

//only changes %c to %d , (unsugned char ) typecasting
//to print intensity as int
int insertInMeanHeap(MinHeap* h, int* freq) {
    int i = 0, count = 0;
    while(i < 256) {
        if(freq[i] > 0) {
            insertMinHeap(h, newNode(i, freq[i]));
            count++;
        }
        i++;
    }
    i = 0;
    while(i < count) {
        printf("%d) %d : %d\n", i, (unsigned char)h->array[i]->ch, h->array[i]->freq);
        i++;
    }
    printf("capacity : %d, size :  %d\n", h->capacity, h->size);
    return count;
}

//no chage
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

void freeHuffmanTree(Tree* node) {
    if(node) {
        freeHuffmanTree(node->f); // Free left subtree
        freeHuffmanTree(node->r); // Free right subtree
        free(node);               // Free current node
    }
}

int convertBinaryToDecimal(int arr[], int n) {
    int decimal = 0;
    for (int i = 0; i < n; i++) {
        decimal = (decimal << 1) | arr[i];
    }
    return decimal;
}

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
        printf("code for '%d' : ", (unsigned char)data->k);
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


void freeCodeList(code *head) {
    code *temp;
    while (head != NULL) {
        temp = head;
        head = head->p; // Move to the next node
        free(temp);     // Free the current node
    }
}

void writeHeader(int fd2, int file, int count) {
    write(fd2, &count, 2);
    int i = count;
    char ch;
    while(i--) {
        int bytesRead = read(file, &ch, 1);
        if(bytesRead == -1) {
            return; 
        }else if (bytesRead == 0) {
            printf("end of file\n");
            return;
        }
        if(write(fd2, &ch, 1) == -1) {
            return; 
        }
    }
}

// Function to compress file
void compressFile(int fd1, int fd2, unsigned char a) {
    char n;
    int h = 0, i;
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
}

void convertDecimalToBinary(int bin[], int decimal, int length) {
    //printf("len = %d\n", length);
    for (int i = length - 1; i >= 0; i--) {
        //printf("I am in convert to binary loop\n");
        bin[i] = decimal % 2;
        decimal /= 2;
    }
}

void ExtractCodesFromFile(int fd1) {
    read(fd1, &t->g, sizeof(char));
    read(fd1, &t->len, sizeof(int));
    read(fd1, &t->dec, sizeof(int));
}

// Function to rebuild the Huffman tree
void ReBuildHuffmanTree(int fd1, int size) {
    int i = 0, j, k;
    int l = 0;
    tree = (Tree*)malloc(sizeof(Tree));
    tree_temp = tree;
    tree->f = NULL;
    tree->r = NULL;
    t = (Tree*)malloc(sizeof(Tree));
    t->f = NULL;
    t->r = NULL;
    for(k = 0; k < size; k++) {
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
                    if(l++ == 0) {
                        if(tree_temp->f->f == NULL) {
                            printf("How is it possible!!\n");
                            printf("char = '%c' &  len = %d\n", tree_temp->f->g, tree_temp->f->len);
                        }
                            
                    }
                    
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
        if(!isroot(tree_temp)) {
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

void writingHeader(int fd3, int fd2) {
    int count;
    char ch;
    read(fd2, &count, 2);
    printf("count = %d\n", count);
    while(count--) {
        int bytesRead = read(fd2, &ch, 1);
        if(bytesRead == -1) {
            return; 
        }else if (bytesRead == 0) {
            printf("end of file\n");
            return;
        }
        if(write(fd3, &ch, 1) == -1) {
            return; 
        }
    }
}

void decompressFileGreyScale(int fd1, int fd2, int f) {
    int inp[8], i, k = 0;
    unsigned char p;
    read(fd1, &p, sizeof(char));
    convertDecimalToBinary(inp, p, 8);
    tree_temp = tree;
    for (i = 0; i < 8 && k < f; i++) {
        if(!isroot(tree_temp)) {
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
            write(fd2, &tree_temp->g, sizeof(char));
            write(fd2, &tree_temp->g, sizeof(char));
            //printf("'%c'\n", tree_temp->g);
            tree_temp = tree;
            i--;
        }
    }
}


void freeImage(unsigned char** image, int height) {
    if (!(image == NULL)) {
        for (int i = 0; i < height; i++) {
            free(image[i]); // Free each row
        }
        free(image); // Free the array of pointers
        image = NULL;
    }
}

void freeData(unsigned char** image, int height, MinHeap *heap) {
    freeHuffmanTree(tree);//tree is diclared globally
    freeHeap(heap);
    freeCodeList(front);//front is diclared globally
    freeImage(image, height);
}

void closeAllFiles(int file, int fd2, int fd3) {
    close(file);
    close(fd2);
    close(fd3);
}
    