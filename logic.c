#include "logic.h"

/**
 * content of compressed file :
 *      1)count of 'total' char in input file/image 
 *          -> need when decompression is needed(otherwise extra added zero in last byte will give some char)
 *      2)count of 'unique' char in file/image and 'char', 'len, decimal value of binary code'
 *          -> needed for rebuilt-hauffman-tree for decompression
 *      3)if it is image, then 'header' 
 *          -> gives info about width, height, bits per pixel , etc which need not be changed
 *      4)hauffman code for each char
 */

int k = 0;
code *front = NULL, *rear = NULL;

//only changes %c to %d , (unsugned char ) typecasting
//to print intensity as int
//it return there are how many unique characters
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
        //printf("%d) %d : %d\n", i, (unsigned char)h->array[i]->ch, h->array[i]->freq);
        i++;
    }
    //capacity means how many char it can occupy which is 256 here
    //size means how many index are filled...or unique char in snother words
    printf("capacity : %d, size :  %d\n", h->capacity, h->size);
    return count;
}


// Function to build Huffman Tree
/**
 * it extracts 2 nodes with minimum frequncy among available
 * and add new now with freq equal to sum of that 2, and its left and right points to that 2 nodes
 * returns root node
 * 
 * time complexity ->
 *      For n−1 iterations, each involving an extract and insert operation
 *      O(nlogn)
 *      n is the number of the unique characters in the input
 */
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
    return extractMin(h);
}

//function to free hauffman tree
void freeHuffmanTree(Tree* node) {
    if(node) {
        freeHuffmanTree(node->f); // Free left subtree
        freeHuffmanTree(node->r); // Free right subtree
        free(node);               // Free current node
    }
}


//it return decimal equivalent of given binary
//time complexity -> O(n)
int convertBinaryToDecimal(int arr[], int n) {
    int decimal = 0;
    for (int i = 0; i < n; i++) {
        //multiply decimal by two and perform bitwise or with arr[i]
        decimal = (decimal << 1) | arr[i];
    }
    return decimal;
}

/**
 * Function to print codes into file recursively
 * during decompression we need to rebuilt hauffman tree 
 * so store each 'char', 'len' of its code, 'decimal' equivalent of code
 * 
 * time complexity ->
 *      O(n⋅logn)
 *      where n is total unique char 
 *      For each leaf node, the time complexity is O(logn) due to the binary-to-decimal conversion.
 */
void printCodesIntoFile(FILE* fd2, node* root,int t[], int top ) {
    //top works as len of binary code travesed
    //'t' holds binary code

    if (root->l) {
        t[top] = 0;
        printCodesIntoFile(fd2, root->l, t, top + 1);
    }

    if (root->r) {
        t[top] = 1;
        printCodesIntoFile(fd2, root->r, t, top + 1);
    }

    /**
     * if it is leaf node then add at the end of the linked-list of code , 
     * which contains data like : char, len, decimal, next ptr
     * and also write 'char, len, decimal' in compressed file
     */
    if (root->l == root->r) {
        code *data = (code*)malloc(sizeof(code));
        data->p = NULL;
        data->k = root->ch;
        fwrite(&root->ch, sizeof(char), 1, fd2);
        for (int i = 0; i < top; i++) {
            data->code_arr[i] = t[i];
        }
        data->l = top;
        fwrite(&data->l, sizeof(int), 1, fd2);
        
        data->d = convertBinaryToDecimal(t, top); 
        
        fwrite(&data->d, sizeof(int), 1, fd2);

        //if it is first node to be added update front and rear
        //'k' i global variable
        if(k == 0) {
            front = rear = data;
            k++;
        }
        else {
            rear->p = data;
            rear = rear->p;
        }
    }
}

//function to free linked-list of codes
void freeCodeList(code *head) {
    code *temp;
    while (head != NULL) {
        temp = head;
        head = head->p; // Move to the next node
        free(temp);     // Free the current node
    }
}

/**
 * in image there is header followed by pixel values
 * so write header as it is in compressed file , don't compress it
 * first write how many char are there in header as 2 byte
 */
void writeHeader(FILE* fd2, FILE* file, int count) {
    
    fwrite(&count, 2, 1, fd2);
    int i = count;
    char ch;
    while(i--) {
        int bytesRead = fread(&ch, 1, 1, file);
        if(bytesRead == -1) {
            return; 
        }else if (bytesRead == 0) {
            printf("end of file\n");
            return;
        }
        if(fwrite(&ch, 1, 1, fd2) != 1) {
            return; 
        }
    }
}

/**
 * read char from input file, search that char in linked-list of codes
 * if it founds then take 'required' bits from that code to become '1 byte' and write that byte to compressed file
 * and still left with some bit of code then make 1 byte of it , 
 * if bits are over from code of last read char , read new char and continue 
 * at the end when all char are read, but some bits are remaining from last read char code, fill '0' to become 1 byte and write in file
 * free the linked-list of codes
 * 
 * time complexity ->
 *      O(N⋅M)
 *      for 'N' iterations (reading char from file), & in each traversing linked-list of 'M' nodes
 */
void compressFile(FILE* fd1, FILE* fd2, unsigned char a) {
    char n;
    int h = 0, i;
    while(fread(&n, sizeof(char), 1, fd1) != 0) {
        rear = front;
        while(rear->k != n && rear->p != NULL) {
            rear = rear->p;
        }
        if(rear->k == n) {
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
                    fwrite(&a, sizeof(char), 1, fd2);
                    a = 0;
                }
            }
        }
    }
    for (i = 0; i < 7 - h; i++) {
        a = a << 1;
    }
    fwrite(&a, sizeof(char), 1, fd2);
    freeCodeList(front);//front is diclared globally
}


//functioon to convert decimal to binary of 'required' length
//time complexity -> O(length)
void convertDecimalToBinary(int bin[], int decimal, int length) {
    //printf("len = %d\n", length);
    for (int i = length - 1; i >= 0; i--) {
        bin[i] = decimal % 2;
        decimal /= 2;
    }
}

//function to read char, len, decimal value from compressed file
void ExtractCodesFromFile(FILE* fd1) {
    fread(&t->g, sizeof(char), 1, fd1); 
    fread(&t->len, sizeof(int), 1, fd1); 
    fread(&t->dec, sizeof(int), 1, fd1);
}

/**
 * Function to rebuild the Huffman tree  for 'size' number of unique char
 * remeber : we have stored char, len, decimal value in compressed file
 * extract that data from compressed file, get binary code of 'required len' from decimal value and len
 * traverse that code , if 'dead end found' then add node and continue traversing
 * when bin_con code is complete, that is leaf node and store char, len, dec int it
 * and continue for other
 * 
 * time complexity->
 *      O(C)
 *      where 'c' is the 'sum of len 'of binary codes of all unique char
 */
void ReBuildHuffmanTree(FILE* fd1, int size) {
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
        int bin[MAX], bin_con[MAX];
        for (i = 0; i < MAX; i++) {
            bin[i] = bin_con[i] = 0;
        }
        //printf("i converted into binary\n");
        convertDecimalToBinary(bin, t->dec, t->len);
        for (i = 0; i < t->len; i++) {
            bin_con[i] = bin[i];
            //printf("%d", bin_con[i]);
        }
        //printf("\n");
        
        for (j = 0; j < t->len; j++) {
            if (bin_con[j] == 0) {
                if (tree_temp->f == NULL) {
                    tree_temp->f
                        = (Tree*)malloc(sizeof(Tree));
                    tree_temp->f->f = NULL;
                    tree_temp->f->r = NULL;
                }
                tree_temp = tree_temp->f;
            }
            else if (bin_con[j] == 1) {
                if (tree_temp->r == NULL) {
                    tree_temp->r
                        = (Tree*)malloc(sizeof(Tree));
                    tree_temp->r->f = NULL;
                    tree_temp->r->r = NULL;
                }
                tree_temp = tree_temp->r;
            }
        }
        tree_temp->g = t->g;
        tree_temp->len = t->len;
        tree_temp->dec = t->dec;
        tree_temp = tree;
    }
}

//to check if give node is leaf-node or not
int isLeaf(Tree* node){
    return (node->f == NULL && node->r == NULL);
}

/**
 * read char from compressed file , convert it to binary code
 * traverse that binary code in hauffman tree if leaf-node fond write 'char in that leaf-node' in decompressed file
 * if some bits are still available(from last read byte's code) or even after traversing entire 8bit code leaf node not found then read another byte
 * and continue traversinng 
 * here, f -> total char in input file
 * 
 * time complexity ->
 *      O(M⋅logN)
 *      M is the total number of characters  in the compressed file 
 *      logN represents the height of the Huffman tree or len of that char's code
 * 
 *      OR O(N) where N is the number of the bits in the encoded message
 */
void decompressFile(FILE* fd1, FILE* fd2, int f) {
    int inp[8], i, k = 0;
    unsigned char p;
    fread(&p, sizeof(char), 1, fd1);
    convertDecimalToBinary(inp, p, 8);
    tree_temp = tree;
    for (i = 0; i < 8 && k < f; i++) {
        if(!isLeaf(tree_temp)) {
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
                if (fread(&p, sizeof(char), 1, fd1) != 0) {
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
            fwrite(&tree_temp->g, sizeof(char), 1, fd2);
            tree_temp = tree;
            i--;
        }
    }
}

/**
 * remember for image, we have written header in compressed file
 * read that from it and write as it is in decompressed file
 */
void writingHeader(FILE* fd3, FILE* fd2) {
    int count;
    char ch;
    int bytesRead = fread(&count, sizeof(short), 1, fd2);
    if (bytesRead != 1) {
        printf("Error reading count or unexpected EOF\n");
        return;
    }
    printf("count : %d\n", count);
    while(count--) {
        int bytesRead = fread(&ch, sizeof(char), 1, fd2);
        if (bytesRead != 1) {
            printf("Error reading count or unexpected EOF\n");
            return;
        }else if (bytesRead == 0) {
            printf("end of file\n");
            return;
        }
        if (fwrite(&ch, sizeof(char), 1, fd3) != 1) {
            printf("Error writing to fd3\n");
            return;
        }
    }
}

