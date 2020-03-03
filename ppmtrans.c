#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

typedef A2Methods_UArray2 A2;

void apply_90_rotation(int i, int j, A2 m, void *elem, void *cl){
	

}

void apply_180_rotation(int i, int j, A2 m, void *elem, void *cl){

}

int main(int argc, char *argv[]){
	int rotation = 0;

	A2Methods_T = array2_methods_plain;
	A2Methods_mapfun *map = methods->map_default;

	#define SET_METHODS(METHODS, MAP) do {\
		methods = (METHODS); \
		map = methods->MAP; \
		if (!map){ \ 
			exit(1);\
		}\
	} while(0)

	int i;
	
