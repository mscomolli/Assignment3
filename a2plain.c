#include <stdlib.h>

#include <a2plain.h>
#include "uarray2.h"

// define a private version of each function in A2Methods_T that we implement
// Function that returns a new uarray2
static A2Methods_Array2 new(int width, int height, int size) {
	return UArray2_new(width, height, size);
}
// Function that returns a new uarray2
static A2Methods_Array2 new_with_blocksize(int width, int height, int size, int blocksize){
	(void) blocksize;
	return UArray2_new(width, height, size);
}

// Function that call uarray2_free that frees the memory allocated for the uarray.
static void a2free (A2Methods_Array2 *array2p) {
	UArray2_free((UArray2_T *)array2p);
}
//These functions return the members of the uarray2 struct. 
static int width    (A2Methods_Array2 array2) { return UArray2_width (array2); }
static int height   (A2Methods_Array2 array2) { return UArray2_height(array2); }
static int size     (A2Methods_Array2 array2) { return UArray2_size  (array2); }
static int blocksize(A2Methods_Array2 array2) { (void)array2; return 1; }
// This Function return the function call to uarray2_at that returns a pointer to a location in the uarray.
static A2Methods_Object *at(A2Methods_Array2 array2, int i, int j) {
	return UArray2_at(array2, i, j);
}


typedef void applyfun(int i, int j, UArray2_T array2, void *elem, void *cl);

//This function calls the row major function for the uarray2 which goes through the array row by row and call an apply 
//function.
static void map_row_major  (A2Methods_Array2 array2, A2Methods_applyfun apply, void *cl) {
	UArray2_map_row_major(array2, (applyfun*)apply, cl);
}

//This function calls the col major function for the uarrays which goes through the array column by column and calls an apply function.
static void map_col_major  (A2Methods_Array2 array2, A2Methods_applyfun apply, void *cl) {
	UArray2_map_col_major(array2, (applyfun*)apply, cl);
}

// The structs and members for the method suite. 
static struct A2Methods_T array2_methods_plain_struct = {
	new,
	new_with_blocksize,
	a2free,
	width,
	height,
	size,
	blocksize,
	at,
	map_row_major,
	map_col_major,
	NULL,
	map_row_major
	};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T array2_methods_plain = &array2_methods_plain_struct;
