#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pnmrdr.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

typedef A2Methods_Array2 A2;
//Function to rotate image 90 degrees
void apply_90_rotation(int i, int j, A2 m, void *elem, void *cl){
	(void)elem;
	//Takes the data from Pnm_ppm data and creates a new Pnm_ppm
	Pnm_ppm pixelArray = cl;
	printf("\npixel array created\n");
	//Get the location from the original 2D array that you want to change.
	Pnm_rgb original = pixelArray->methods->at(m, i, j);
	printf("\nat function successful\n");
	//Get the location from the pixel map that is 90 degress the opposite way
	Pnm_rgb newPosition = pixelArray->methods->at(pixelArray->pixels, j, (pixelArray->height)-i-1);
	//Put the opposite rgb into the original 2D array.
	*original = *newPosition;

}

//Function to rotate image 180 degress
void apply_180_rotation(int i, int j, A2 m, void *elem, void *cl){
	(void)elem;
	//Takes the data from Pnm_ppm data and creates a new Pnm_ppm
	Pnm_ppm pixelArray = cl;
	//Get the location from the original 2D array that you want to change
	Pnm_rgb original = pixelArray->methods->at(m, i, j);
	//Get the location from the pizel map that is 90 degrees the opposite way
	Pnm_rgb newPosition = pixelArray->methods->at(pixelArray->pixels, (pixelArray->width)-i-1, (pixelArray->height)-j-1);
	//Put the opposite rgb into the original 2D array.
	*original = *newPosition;
	
}

int main(int argc, char *argv[]){

	int rotation = 0;
	A2Methods_T methods = array2_methods_plain; // default to Array2 methods
	assert(methods);
	A2Methods_mapfun *map = methods->map_default; // default to best map
	assert(map);
	#define SET_METHODS(METHODS, MAP, WHAT) do { \
	methods = (METHODS); \
	assert(methods); \
	map = methods->MAP; \
	if (!map) { \
		fprintf(stderr, "%s does not support " WHAT "mapping\n", argv[0]); \
		exit(1); \
	} \
	} while(0)
	int i;
	FILE* file;
	printf("\nFile Defined.");
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-row-major")) {
			SET_METHODS(array2_methods_plain, map_row_major, "row-major");
			
		} 
		else if (!strcmp(argv[i], "-col-major")) {
			SET_METHODS(array2_methods_plain, map_col_major, "column-major");
		} 
		else if (!strcmp(argv[i], "-block-major")) {
			SET_METHODS(array2_methods_blocked, map_block_major, "block-major");
		} 
		else if (!strcmp(argv[i], "-rotate")) {
			assert(i + 1 < argc);
			char *endptr;
			rotation = strtol(argv[++i], &endptr, 10);
			assert(*endptr == '\0'); // parsed all correctly
			assert(rotation == 0 || rotation == 90 || rotation == 180 || rotation == 270);
		} 
		else if (*argv[i] == '-') {
			fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
			exit(1);
		} 
		else if (argc - i > 2) {
			fprintf(stderr, "Usage: %s [-rotate <angle>] [-{row,col,block}-major] [filename]\n", argv[0]);
			exit(1);
		} 
		else {
			break;
		}
	}

	if(argc==2){
		file = fopen(argv[1], "r");
	}
	else{
		file = stdin;
	}

	if(file==NULL){
		fprintf(stderr, "%s: Could not open file %s.\n", argv[0], argv[1]);
	}

	// Reads a file and returns a pixel map containing a 2D array.
	Pnm_ppm data = Pnm_ppmread(file, methods);
	
	//Creates a new instance of A2Methods
	A2 newImage;
	if(rotation == 90){
		//Create a new 2D array
		printf("\nRotating 90\n");	
		newImage = methods->new(data->width, data->height, sizeof(Pnm_rgb) * 2);
		//Map for the new 2D array and calls apply 90
		map(newImage, apply_90_rotation, data);
	}
	else if(rotation == 180){
		//Create a new 2D array
		newImage = methods->new(data->height, data->width, sizeof(Pnm_rgb) * 2);
		//Map for the new 2D array and calls apply 180
		map(newImage, apply_180_rotation, data);
	}
	if(rotation == 90){	
		data->width = data->height;
		data->height = data->width;
		data->pixels = newImage;
	}
	else if(rotation == 180){
		data->pixels = newImage;
	}

	Pnm_ppmwrite(file, data);
	
	methods->free(&newImage);
	
	Pnm_ppmfree(&data);
	fclose(file);

}

	
