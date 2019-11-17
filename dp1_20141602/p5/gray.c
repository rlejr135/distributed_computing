#include <stdio.h>
#include <stdlib.h>
//#include <mpi.h>
#include <string.h>
#include <time.h>

typedef struct pix{
	int r, g, b;
}pixel;


int main(int argc, char *argv[]){
	FILE *fr = fopen("inout/input.ppm", "r");
	FILE *fw = fopen("inout/output_gray_seq.pgm", "w");

	int i, j;
	int rank, procnum;
	char format[4];
	int width, height, datsize;

	pixel *img;

	double start, end;

	fscanf(fr, "%s", format);
	fscanf(fr, "%d %d", &width, &height);
	fscanf(fr, "%d", &datsize);

	img = (pixel*)malloc(sizeof(pixel) * height * width);

	for (i = 0 ; i < height ; i++){

		for (j = 0 ; j < width ; j++){
			fscanf(fr, "%c%c%c", &(img[i*width+j].r), &(img[i*width+j].g), &(img[i*width+j].b));
		}
	}


	////////////////////
	start = clock();

	for (i = 0 ; i < height ; i++){
		for (j = 0 ; j < width ; j++){
			img[i*width + j].r = (img[i*width+j].r+ img[i*width+j].g+ img[i*width+j].b)/3;
		}
	}

	//////////////////////
	end = clock();


	fprintf(fw, "P5\n%d %d\n%d\n", width, height, datsize);

	for (i = 0 ; i < height ; i++){
		for (j = 0 ; j < width ; j++){
			fprintf(fw, "%c", img[i*width+j].r);
		}
	}

	printf("gray seq : %.6lf\n", (double)(end - start)/CLOCKS_PER_SEC);

	free(img);

	fclose(fr);
	fclose(fw);
	
	return 0;
}
