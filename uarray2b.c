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

T UArray2b_new(int width, int height, int size, int blocksize){
	assert(height>0 && width>0 && size>0);

	T uarray2b;
	NEW(uarray2b);

	uarray2b->height = height;
	uarray2b->width = width;
	uarray2b->size = size;
	uarray2b->blocksize = blocksize;

	UArray2_T *uarray2;
	NEW(uarray2);
	*uarray2 = UArray2_new((int)ceil((float)height/(float)blocksize), (int)ceil((float)width/(float)blocksize), sizeof(int *));
	
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

T UArray2b_new_64_block(int width, int height, int size){
	assert(height>0 && width>0 && size>0);
	
	T uarray2b;
	NEW(uarray2b);

	uarray2b->height = height;
	uarray2b->width = width;
	uarray2b->size = size;
	int blocksize = sqrt(64/size);
	uarray2b->blocksize = blocksize;

	UArray2_T *uarray2;
	NEW(uarray2);
	*uarray2 = UArray2_new(ceil(height/blocksize), ceil(width/blocksize), blocksize*blocksize*sizeof(void *));
	
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

void UArray2b_free(T *uarray2b){
	assert(uarray2b && *uarray2b);
	for(int i=0; i<(int)ceil((float)(*uarray2b)->height/(float)(*uarray2b)->blocksize); i++){
		for(int j=0; j<(int)ceil((float)(*uarray2b)->width/(float)(*uarray2b)->blocksize); j++){
			Array_T *p = UArray2b_at(*uarray2b, i, j);
			FREE(p);
		}
	}
	UArray2_free((*uarray2b)->array);
	FREE(*uarray2b);

}

int UArray2b_width(T uarray2b){
	assert(uarray2b != NULL);
	return uarray2b->width;
}

int UArray2b_height(T uarray2b){
	assert(uarray2b != NULL);
	return uarray2b->height;
}

int UArray2b_size(T uarray2b){
	assert(uarray2b != NULL);
	return uarray2b->size;
}

int UArray2b_blocksize(T uarray2b){
	assert(uarray2b != NULL);
	return uarray2b->blocksize;
}

void *UArray2b_at(T uarray2b, int i, int j){
	assert(uarray2b);
	assert(i>=0);
	assert(j>=0);

	int k = (i/(uarray2b->blocksize));
	int l = (j/(uarray2b->blocksize));
	Array_T *p;
	p = UArray2_at(*(uarray2b->array), l, k);
	void *d;
	d = ((int *)*p + (((i%uarray2b->blocksize)*uarray2b->blocksize + (j%uarray2b->blocksize))*uarray2b->size));
	return d;
}
void UArray2b_map(T uarray2b, void apply(int i, int j, T uarray2b, void *elem, void *cl), void *cl){
	void *p;
	int bs = uarray2b->blocksize;
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


