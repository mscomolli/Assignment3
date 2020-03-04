#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "array.h"
#include "arrayrep.h"
#include "assert.h"
#include "mem.h"
#include "uarray2.h"

#define T UArray2_T


T UArray2_new(int width, int height, int size) {
  	T uarray2;
  	NEW(uarray2);
  	uarray2->width  = width;
  	uarray2->height = height;
  	uarray2->size   = size;

	Array_T *arr;
	NEW(arr);
	*arr = Array_new(height*width, size);

	uarray2->array = arr;
  	return uarray2;
}

void UArray2_free(T *uarray2) {
   	assert(uarray2 && *uarray2);
  	FREE((*uarray2)->array);
	FREE(*uarray2);
}

void *UArray2_at(T uarray2, int i, int j) {
  	assert(uarray2);
  	return Array_get(*(uarray2->array), (j*uarray2->width)+i);
}

int UArray2_height(T uarray2) {
	assert(uarray2);
        return uarray2->height;
}
int UArray2_width(T uarray2) {
        assert(uarray2);
        return uarray2->width;
}
int UArray2_size(T uarray2) {
        assert(uarray2);
        return uarray2->size;
}

void UArray2_map_row_major(T uarray2, void apply(int i, int j, T uarray2, void *elem, void *cl), void *cl) {
	assert(uarray2);
	int *n;
	for(int i=0; i<uarray2->height; i++){
		for(int j=0; j<uarray2->width; j++){
			n = UArray2_at(uarray2, i, j); 
			apply(i, j, uarray2, n, cl);
		}
	}
	return;
}

void UArray2_map_col_major(T uarray2, void apply(int i, int j, T uarray2, void *elem, void *cl), void *cl) {
  	assert(uarray2);
  	int *n;
	Array_T arr = *(uarray2->array);
	for(int i=0; i<uarray2->width; i++){
		for(int j=0; j<uarray2->height; j++){
			n = (int *)(arr->array + (j * uarray2->size * uarray2->width) + (i * uarray2->size));
			apply(i, j, uarray2, n, cl);
		}
	}
	return;
}
