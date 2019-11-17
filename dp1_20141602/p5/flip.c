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
	FILE *fw = fopen("inout/output_flip_seq.ppm", "w");

	int i, j;
	int rank, procnum;
	char format[4];
	int width, height, datsize;


	time_t start, end;

	

	pixel *img;

	fscanf(fr, "%s", format);
	fscanf(fr, "%d %d", &width, &height);
	fscanf(fr, "%d", &datsize);

	img = (pixel*)malloc(sizeof(pixel) * height * width);

	for (i = 0 ; i < height ; i++){
		for (j = 0 ; j < width ; j++){
			fscanf(fr, "%c%c%c", &(img[i*width+j].r), &(img[i*width+j].g), &(img[i*width+j].b));
		}
	}



	//////////////////
	start = clock();


	int tmp;
	int swap;
	
	for (i = 0 ; i < height ; i++){
		for (j = 0 ; j < width/2 ; j++){
			swap = width - j - 1;

			tmp = img[i*width+j].r;
			img[i*width+j].r = img[i*width+swap].r;
			img[i*width+swap].r = tmp;

			tmp = img[i*width+j].g;
			img[i*width+j].g = img[i*width+swap].g;
			img[i*width+swap].g = tmp;
			
			tmp = img[i*width+j].b;
			img[i*width+j].b = img[i*width+swap].b;
			img[i*width+swap].b = tmp;
		}
	}


	/////////////////
	end = clock();


	fprintf(fw, "%s\n%d %d\n%d", format, width, height, datsize);

	for (i = 0 ; i < height ; i++){
		for (j = 0 ; j < width ; j++){
			fprintf(fw, "%c%c%c", img[i*width+j].r, img[i*width+j].g, img[i*width+j].b);
		}
	}

	printf("flip seq time : %.6lf\n", (double)(end - start)/CLOCKS_PER_SEC);


	free(img);

	fclose(fr);
	fclose(fw);
	
	return 0;
}
