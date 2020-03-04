#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <math.h>
#include "mem.h"
#include "assert.h"
#include "array.h" 
#include "uarray2.h"
#include "uarray2b.h"

#define T UArray2b_T


//Create Uarray2b struct
struct T{
	int height;
	int width;
	int size;
	int blocksize;
	UArray2_T *array;
};

//Function to create a new UArray2b
T UArray2b_new(int width, int height, int size, int blocksize){
	assert(height>0 && width>0 && size>0);
	
	//Creates and allocates space for new uarray2b
	T uarray2b;
	NEW(uarray2b);
	printf("\nUarray2b created\n");

	//Initailize members for the uarray2b struct
	uarray2b->height = height;
	uarray2b->width = width;
	uarray2b->size = size;
	uarray2b->blocksize = blocksize;

	//Creat and allocates space for new uarray2
	UArray2_T *uarray2;
	NEW(uarray2);
	*uarray2 = UArray2_new((int)ceil((float)height/(float)blocksize), (int)ceil((float)width/(float)blocksize), sizeof(void *));
	printf("\nUarray2 created\n");
	//Nested for loops that access each index in uarray2 and saves a pointer to an array.
	for(int i=0; i<((int)ceil((float)height/(float)blocksize)); i++){
		for(int j=0; j<((int)ceil((float)width/(float)blocksize)); j++){
			Array_T *arrP = UArray2_at(*uarray2, i, j);
			void *p;
			p = Array_new((blocksize*blocksize), size);
			*arrP = p;
			printf("\n%d arrays created\n", i);

			printf("%p\n", p);
		}
	}
	uarray2b->array = uarray2;

	return uarray2b;
}

T UArray2b_new_64K_block(int width, int height, int size){
	assert(height>0 && width>0 && size>0);
	
	//Creates and allocates space for new uarray2b
	T uarray2b;
	NEW(uarray2b);

	//Intialized member for uarray2b struct
	uarray2b->height = height;
	uarray2b->width = width;
	uarray2b->size = size;
	int blocksize = sqrt(64/size);
	uarray2b->blocksize = blocksize;
	
	//Creates and allocates space for new uarray2 
	UArray2_T *uarray2;
	NEW(uarray2);
	*uarray2 = UArray2_new(ceil(height/blocksize), ceil(width/blocksize), blocksize*blocksize*sizeof(void *));
	
	//Nested for loops that save a pointer to a new array at each index of the uarray2
	for(int i=0; i<((int)ceil((float)height/(float)blocksize)); i++){
		for(int j=0; j<((int)ceil((float)width/(float)blocksize)); j++){
			Array_T *arrP = UArray2_at(*uarray2, i, j);
			void *p;
			p = Array_new((blocksize*blocksize), size);
			*arrP = p;
			printf("%p\n", p);
												
		}
	}
	uarray2b->array = uarray2;

	return uarray2b;
}

//return uarray2b width
int UArray2b_width(T uarray2b){
	assert(uarray2b != NULL);
	return uarray2b->width;
}

//return uarray2b height
int UArray2b_height(T uarray2b){
	assert(uarray2b != NULL);
	return uarray2b->height;
}

//return uarray2b size
int UArray2b_size(T uarray2b){
	assert(uarray2b != NULL);
	return uarray2b->size;
}

//return uarray2b blocksize
int UArray2b_blocksize(T uarray2b){
	assert(uarray2b != NULL);
	return uarray2b->blocksize;
}

//Function to return a pointer to an index 
void *UArray2b_at(T uarray2b, int i, int j){
	assert(uarray2b);
	assert(i>=0);
	assert(j>=0);
	//k and l are the block at which the index is located
	int k = (i/(uarray2b->blocksize));
	int l = (j/(uarray2b->blocksize));

	int c = i % uarray2b->blocksize;
	int r = j % uarray2b->blocksize;
	//Creates an array pointer
	Array_T **p = (Array_T **)UArray2_at(*uarray2b->array, k, l);
	Array_T *d = *p;
	return Array_get(*d, ((r * uarray2b->blocksize) + c));
}

//Function frees memory allocated for the Uarray2b, uarray2 and all array saved in the uarray2
void UArray2b_free(T *uarray2b){
	assert(uarray2b && *uarray2b);
	//nested for loop to access each index in the uarray2
	for(int i=0; i<(int)ceil((float)(*uarray2b)->height/(float)(*uarray2b)->blocksize); i++){
		for(int j=0; j<(int)ceil((float)(*uarray2b)->width/(float)(*uarray2b)->blocksize); j++){
			//p saves the address of the array to be freed
			Array_T *p = UArray2b_at(*uarray2b, i, j);
			Array_free(p);
		}
	}
	//free memory allocated for uarray2
	UArray2_free((*uarray2b)->array);
	FREE(*uarray2b);

}

//Function to call the apply function that visits every member of each block before going to the next block
void UArray2b_map(T uarray2b, void apply(int i, int j, T uarray2b, void *elem, void *cl), void *cl){
	void *p;
	int bs = uarray2b->blocksize;
	//nested for loop to go from block to block and index to index
	for(int i=0; i<(int)ceil((uarray2b->width)/bs); i++){
		for(int j=0; j<(int)ceil((uarray2b->height)/bs); j++){
			for(int t=0; t<bs; t++){
				for(int l=0; l<bs; l++){
					p = UArray2b_at(uarray2b,(i*bs) + t , (j*bs)+l);
					apply(i, j, uarray2b, p, cl);
				}

			}
		}
	}
}

