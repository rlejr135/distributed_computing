#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

typedef struct pix{
	int r, g, b;
}pixel;

int dir[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

int main(int argc, char *argv[]){
	FILE *fr;
	FILE *fw;

	int i, j;
	int rank, numprocs;
	char format[4];
	int width, height, datsize;

	MPI_Datatype ppixel;

	pixel *img, *tmp;
	pixel *copy, *allcopy;
	
	int numw;

	double start, finish;
	
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Type_contiguous(3, MPI_INT, &ppixel);
	MPI_Type_commit(&ppixel);



	fr = fopen("inout/input.ppm", "r");
	if (rank == 0 )fw = fopen("inout/output_smooth_par.ppm", "w");
	fscanf(fr, "%s", format);
	fscanf(fr, "%d %d", &width, &height);
	fscanf(fr, "%d", &datsize);


	numw = height/numprocs;
	tmp = (pixel*)malloc(sizeof(pixel) * numw * width);

	if (rank == 0){

		img = (pixel*)malloc(sizeof(pixel) * height * width);
		allcopy = (pixel*)malloc(sizeof(pixel) * height * width);

		for (i = 0 ; i < height ; i++){
			for (j = 0 ; j < width ; j++){
				fscanf(fr, "%c%c%c", &(img[i*width + j].r), &(img[i*width + j].g), &(img[i * width + j].b));
				allcopy[i*width + j] = img[i*width + j];
			}
		}
	}

	//////////////////////
	start = MPI_Wtime();

	MPI_Scatter(img, numw*width, ppixel, tmp, numw*width, ppixel, 0, MPI_COMM_WORLD);

	copy = (pixel*)malloc(sizeof(pixel) * numw * width);


	int tmpvalr,tmpvalg, tmpvalb;
	int k, q, ii;

	for (i = 0 ; i < numw ; i++){
		for (j = 0 ; j < width ; j++){
			copy[i*width+j] = tmp[i*width+j];
		}
	}


	for (i = 1 ; i < numw - 1 ; i++){
		for (j = 1 ; j < width - 1; j++){
			tmpvalr = tmpvalg = tmpvalb = 0;
			for (k = 0 ; k < 8 ; k++){
				tmpvalr += copy[(i + dir[k][0]) * width + j + dir[k][1]].r;
				tmpvalg += copy[(i + dir[k][0]) * width + j + dir[k][1]].g;
				tmpvalb += copy[(i + dir[k][0]) * width + j + dir[k][1]].b;
			}
			tmp[i*width+j].r = tmpvalr/8;
			tmp[i*width+j].g = tmpvalg/8;
			tmp[i*width+j].b = tmpvalb/8;
		}
	}



	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(tmp, numw*width, ppixel, img, numw*width, ppixel, 0, MPI_COMM_WORLD);


	if (rank == 0){
		for (i = numw ; i < height - 1 ; i+= numw){
		for (ii = i-1 ; ii <= i+1 ; ii ++){
			for (j = 1 ; j < width - 1; j++){
				tmpvalr = tmpvalg = tmpvalb = 0;
				for (k = 0 ; k < 8 ; k++){
					tmpvalr += allcopy[(ii + dir[k][0]) * width + j + dir[k][1]].r;
					tmpvalg += allcopy[(ii + dir[k][0]) * width + j + dir[k][1]].g;
					tmpvalb += allcopy[(ii + dir[k][0]) * width + j + dir[k][1]].b;
				//	printf("[%d %d %d]\n", tmpvalr, tmpvalg, tmpvalb);
				}
	//		printf("\n[%d]\n%d %d %d\n", i, img[i*width+j].r, img[i*width+j].g, img[i*width+j].b);
				img[ii*width+j].r = tmpvalr/8;
				img[ii*width+j].g = tmpvalg/8;
				img[ii*width+j].b = tmpvalb/8;
		//	printf("[%d]\n%d %d %d\n\n", i, img[i*width+j].r, img[i*width+j].g, img[i*width+j].b);
			}
		}
		}
	}


	///////////////////////
	finish = MPI_Wtime();

	if (rank == 0){
		fprintf(fw, "%s\n%d %d\n%d", format, width, height, datsize);

		for (i = 0 ; i < height ; i++){
			for (j = 0 ; j < width ; j++){
				fprintf(fw, "%c%c%c", img[i * width + j].r, img[i*width + j].g, img[i* width + j].b);
			}
		}

	}
	if (img) free(img);
	if (copy) free(copy);
	if (allcopy) free(allcopy);


	//fclose(fr);
	//fclose(fw);


	if (rank == 0){
		printf("num of procs : %d, smooth par time : %.6lf\n", numprocs, finish-start);
	}


	MPI_Type_free(&ppixel);
	MPI_Finalize();

	return 0;
}
