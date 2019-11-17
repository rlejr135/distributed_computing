#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

typedef struct pix{
	int r, g, b;
}pixel;


int main(int argc, char *argv[]){
	FILE *fr;
	FILE *fw;

	int i, j;
	int rank, numprocs;
	char format[4];
	int width, height, datsize;

	MPI_Datatype ppixel;

	pixel *img, *tmp;

	double start, finish;

	int numw;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	fr = fopen("inout/input.ppm", "r");
	if (rank == 0)fw = fopen("inout/output_gray_par.pgm", "w");


	fscanf(fr, "%s", format);
	fscanf(fr, "%d %d", &width, &height);
	fscanf(fr, "%d", &datsize);




	numw = height/ numprocs;

	MPI_Type_contiguous(3, MPI_INT, &ppixel);
	MPI_Type_commit(&ppixel);


	
	tmp = (pixel*)malloc(sizeof(pixel) * height * width);

	if (rank == 0){
		img = (pixel*)malloc(sizeof(pixel) * height * width);

		for (i = 0 ; i < height ; i++){
			for (j = 0 ; j < width ; j++){
				fscanf(fr, "%c%c%c", &(img[i*width+j].r), &(img[i*width+j].g), &(img[i*width+j].b));
			}
		}
	}



	////////////////////
	start = MPI_Wtime();


	MPI_Scatter(img, numw*width, ppixel, tmp, numw*width, ppixel, 0, MPI_COMM_WORLD);

	int restmp;	
	for (i = 0 ; i < numw ; i++){
		for (j = 0 ; j < width ; j++){
			restmp = (tmp[i*width+j].r + tmp[i*width + j].g + tmp[i*width+j].b)/3;
			tmp[i*width+j].r = restmp;
		}
	}


	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(tmp, numw*width, ppixel, img, numw*width, ppixel, 0, MPI_COMM_WORLD);
	

	if (rank == 0){

		for (i = numw*numprocs ; i < height ; i++){
			for (j = 0 ; j < width ; j++){
				restmp = (img[i*width+j].r + img[i*width + j].g + img[i*width+j].b)/3;
				img[i*width+j].r = restmp;
			}
		}
	
	}

	/////////////////////
	finish = MPI_Wtime();


	if(rank == 0){

		fprintf(fw, "P5\n%d %d\n%d\n",  width, height, datsize);

		for (i = 0 ; i < height ; i++){
			for (j = 0 ; j < width ; j++){
				fprintf(fw, "%c", img[i*width+j].r);
			}
		}
	}	

	if (img) free(img);

	fclose(fr);
	if (rank == 0)fclose(fw);


	if (rank == 0){
		printf("num of procs : %d, gray par time : %.6lf\n", numprocs, finish-start);

	}



	MPI_Type_free(&ppixel);
	MPI_Finalize();
	
	return 0;
}
