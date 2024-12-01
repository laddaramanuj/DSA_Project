#include "logic.h"
//run command -> 
//gcc image.c minHeap.c logic.c
int main() {
	MinHeap *heap = createMinHeap(256);
	int i,j,r,c;
	char filename[] = "lena_black_white.bmp";
	int offset,hbytes,width,height;
	long bmpsize=0,bmpdataoff=0,bpp=0;
	unsigned char **image;
    char id[2];

	//Reading the BMP File
    int file = open(filename, O_RDONLY);
    //FILE *image_file;
	//image_file = fopen(filename,"rb");
    if (file == -1) {
        perror("Error opening file");
        return 1;
    }else {
		printf("Processing BMP Header...\n\n");
		//1 - 7 -> header
		//8 -> data

		//1 -> 0 id
		offset = 0;
        lseek(file, offset, SEEK_SET);
		//fread(&id[0],1,1,image_file);
        read(file, &id[0], 1);
		printf("%c",id[0]);
		read(file, &id[1], 1);
		printf("%c\n",id[1]);
		
		//2 -> 2 size of file
		//printf("\n\nGetting size of BMP File...\n\n");
        read(file, &bmpsize, 4);
		printf("Size of the BMP File:: %ld bytes\n\n",bmpsize);

		//3 -> 10 offset for pixel array
		//printf("Getting offset where the pixel arrray starts...\n\n");
		offset = 10;
		lseek(file, offset, SEEK_SET);
        read(file, &bmpdataoff, 4);
		printf("Bitmap data offset:: %ld\n\n",bmpdataoff);

		//4 ->  14 num of bytes in header
		//printf("DIB Header\n\n");
        read(file, &hbytes, 4);
		printf("Number of bytes in header:: %d\n\n",hbytes);

		//5 -> 18 -> width of image
		//fread(&width,4,1,image_file);
        read(file, &width, 4);
		printf("Width of Image: %d\n",width);

		//6 -> 22 heightof image
        read(file, &height, 4);
		printf("Height of image: %d\n\n",height);
        if(height < 0)
            height = (-1)*height;
		
		//7 -> 28 Number of bits per pixel
		//fseek(image_file,2,SEEK_CUR);
        lseek(file, 2, SEEK_CUR);
        read(file, &bpp, 2);
		printf("Number of bits per pixel: %ld\n\n",bpp);

		//8 -> now we will read 
		//printf("Setting offset to start of pixel data...\n\n");
		//fseek(image_file,bmpdataoff,SEEK_SET);
        lseek(file, bmpdataoff, SEEK_SET);

		printf("Creating Image array...\n\n");
		image = (unsigned char **)malloc(height*sizeof(unsigned char *));
		for(i=0;i<height;i++) {
			image[i] = (unsigned char *)malloc(width*sizeof(unsigned char));
		}
		//int image[height][width];
		int numbytes = (bmpsize - bmpdataoff)/3;
		printf("Number of bytes: %d \n\n",numbytes);
		//printf("Reading the BMP File into Image Array...\n\n");
		for(r=0;r<height;r++) {
			for(c=0;c<width;c++) {
                unsigned char ch;
				//fread(&ch,1,1,image_file);
				//printf("%d ", ch);
				//fread(&ch,1,1,image_file);
				//printf("%d ", ch);
				//fread(&ch,1,1,image_file);
                read(file, &ch, 1);
				//printf("%d \n", ch);
				image[r][c] = ch;
			}
		}
		printf("Image array allocated...\n\n");
	}
    
	

	//Finding the probability of occurence
	int hist[256] = {0};
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			hist[(unsigned char)image[i][j]]+=1;
	
    //i -> intensity
    //hist[i] -> frequency of that i 

	int count = insertInMeanHeap(heap, hist);

	node* root = buildHuffmanTree(heap);

	int fd2 = open("compressedImage.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd2 == -1) {
        perror("Error opening output file");
        return 1;
    }

	printf("count : %d\n", count);
    write(fd2, &count, sizeof(int));
    int t[16];
    printCodesIntoFile(fd2, root, t, 0);

    lseek(file, 0, SEEK_SET);
    writeHeader(fd2, file, bmpdataoff);//from file write bmpdataoff byte to fd2
    
    unsigned char a = 0;
    compressFile(file, fd2, a);
	
	lseek(fd2, 0, SEEK_SET);
    fd2 = open("compressedImage.bin", O_RDONLY);
    if (fd2 == -1) {
        perror("Error opening compressed file for reading");
        return 1;
    }
    
    int fd3 = open("DecompressedImage.bmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd3 == -1) {
        perror("Error opening output file");
        return 1;
    }
    
    int n1;
    read(fd2, &n1, sizeof(int));
    ReBuildHuffmanTree(fd2, n1);
    printf("i have built tree\n");
    int total = bmpsize - hbytes;
    writingHeader(fd3, fd2);
    decompressFile(fd2, fd3, total);
    printf("i have decompressed\n");

    
	closeAllFiles(file, fd2, fd3);
    freeData(image, height, heap);
	
	return 0;
}

