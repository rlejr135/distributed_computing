#include <stdio.h>
#include <stdlib.h>
//#include <mpi.h>
#include <string.h>
#include <time.h>

typedef struct pix{
	int r, g, b;
}pixel;

int dir[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

int main(int argc, char *argv[]){
	FILE *fr = fopen("inout/input.ppm", "r");
	FILE *fw = fopen("inout/output_smooth_seq.ppm", "w");

	int i, j;
	int rank, procnum;
	char format[4];
	int width, height, datsize;

	pixel *img;
	pixel *copy;


	double start, end;



	fscanf(fr, "%s", format);
	fscanf(fr, "%d %d", &width, &height);
	fscanf(fr, "%d", &datsize);

	img = (pixel*)malloc(sizeof(pixel) * height * width);
	copy = (pixel*)malloc(sizeof(pixel) * height * width);

	for (i = 0 ; i < height ; i++){
		for (j = 0 ; j < width ; j++){
			fscanf(fr, "%c%c%c", &(img[i*width + j].r), &(img[i*width + j].g), &(img[i * width + j].b));
			copy[i * width + j] = img[i * width + j];
		}
	}


	///////////////////
	start = clock();

	int tmpvalr,tmpvalg, tmpvalb;
	int k, q;
	for (i = 1 ; i < height - 1 ; i++){
		for (j = 1 ; j < width - 1; j++){
			tmpvalr = tmpvalg = tmpvalb = 0;
			for (k = 0 ; k < 8 ; k++){
				tmpvalr += copy[(i + dir[k][0]) * width + j + dir[k][1]].r;
				tmpvalg += copy[(i + dir[k][0]) * width + j + dir[k][1]].g;
				tmpvalb += copy[(i + dir[k][0]) * width + j + dir[k][1]].b;
			}
			img[i*width+j].r = tmpvalr/8;
			img[i*width+j].g = tmpvalg/8;
			img[i*width+j].b = tmpvalb/8;
		}
	}



	//////////////////
	end = clock();

	fprintf(fw, "%s\n%d %d\n%d", format, width, height, datsize);

	for (i = 0 ; i < height ; i++){
		for (j = 0 ; j < width ; j++){
			fprintf(fw, "%c%c%c", img[i * width + j].r, img[i*width + j].g, img[i* width + j].b);
		}
	}


	printf("smooth seq time : %.6lf\n", (double)(end - start)/CLOCKS_PER_SEC);

	free(img);
	free(copy);

	fclose(fr);
	fclose(fw);
	
	return 0;
}
