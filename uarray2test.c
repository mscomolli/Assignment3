#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "mem.h"
#include "uarray2.h"
#include "uarray2b.h"
#include "array.h"
#include "assert.h"
#include "pnmrdr.h"

void fill_uarray2b(int i, int j, UArray2b_T uarray2b, void *p, void *cl){
	(void)cl;
	
	*(int*)p = (i*uarray2b->blocksize + j);
	
}

void apply_print(int i, int j, UArray2b_T uarray2b, void *elem, void *cl){
	(void)cl;
	(void)uarray2b;
	printf("%d,%d\t%p:\t%d\n",i,j, elem, *(int *)elem);
}

int main(int argc, char *argv[]){
	(void)argc;
	(void)argv;
	//assert(argc<=2);
	//FILE *stream;
	//if (argc==2){
	//	stream = fopen(argv[1],"r");
	//}
	//else{
	//	stream = stdin;
	//}
	
	//if (stream==NULL){
	//	fprintf(stderr, "%s: Could not open file %s for reading!\n", argv[0], argv[1]);
//		exit(1);
	//}

	//Pnmrdr_T *pnm;
	//NEW(pnm);
//	*pnm = Pnmrdr_new(stream);
	//Pnmrdr_mapdata data = Pnmrdr_data(*pnm);
	

	UArray2b_T *uarray2b;
	NEW(uarray2b);
	*uarray2b = UArray2b_new(10, 10, sizeof(int), 2);
	UArray2b_map(*uarray2b, fill_uarray2b, NULL);

	UArray2b_map(*uarray2b, apply_print, NULL);

	return(0);
}	
