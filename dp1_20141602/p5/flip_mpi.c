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

	int numw;

	pixel *img;
	pixel *tmp;


	double start, finish;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	fr = fopen("inout/input.ppm","r");

	if (rank == 0)	fw = fopen("inout/output_flip_par.ppm", "w");

	fscanf(fr, "%s", format);
	fscanf(fr, "%d %d", &width, &height);
	fscanf(fr, "%d", &datsize);

	numw = height / numprocs;

	MPI_Type_contiguous(3, MPI_INT, &ppixel);
	MPI_Type_commit(&ppixel);


	tmp = (pixel*)malloc(sizeof(pixel) * numw*width);


	if (rank == 0){

		img = (pixel*)malloc(sizeof(pixel) * height*width);

		for (i = 0 ; i < height ; i++){
			for (j = 0 ; j < width ; j++){
				fscanf(fr, "%c%c%c", &(img[i*width+j].r), &(img[i*width+j].g), &(img[i*width+j].b));
			}
		}
	}


	//////////////////	
	start = MPI_Wtime();

	MPI_Scatter(img, numw*width, ppixel, tmp, numw*width, ppixel, 0, MPI_COMM_WORLD);



	int itmp;
	int swap;
	
	for (i = 0 ; i < numw ; i++){
		for (j = 0 ; j < width/2 ; j++){

			swap = width - j - 1;
			itmp = tmp[i*width+j].r;
			tmp[i*width+j].r = tmp[i*width+swap].r;
			tmp[i*width+swap].r = itmp;

			itmp = tmp[i*width+j].g;
			tmp[i*width+j].g = tmp[i*width+swap].g;
			tmp[i*width+swap].g = itmp;
			
			itmp = tmp[i*width+j].b;
			tmp[i*width+j].b = tmp[i*width+swap].b;
			tmp[i*width+swap].b = itmp;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(tmp, numw*width, ppixel, img, numw*width, ppixel, 0, MPI_COMM_WORLD);


	if (rank == 0){
		for (i = numw*(numprocs) ; i < height ; i++){
			for (j = 0 ; j < width/2 ; j++){
				swap = width - j - 1;
				itmp = img[i*width+j].r;
				img[i*width+j].r = img[i*width+swap].r;
				img[i*width+swap].r = itmp;

				itmp = img[i*width+j].g;
				img[i*width+j].g = img[i*width+swap].g;
				img[i*width+swap].g = itmp;
			
				itmp = img[i*width+j].b;
				img[i*width+j].b = img[i*width+swap].b;
				img[i*width+swap].b = itmp;
			}
	}


	}


	/////////////////////
	finish = MPI_Wtime();


	fclose(fr);

	if (rank == 0){

		fprintf(fw, "%s\n%d %d\n%d", format, width, height, datsize);

		for (i = 0 ; i < height ; i++){
			for (j = 0 ; j < width ; j++){
				fprintf(fw, "%c%c%c", img[i*width+j].r, img[i*width+j].g, img[i*width+j].b);
			}
		}
	}
	if (img)free(img);

//	 fclose(fr);
//	if (fw) fclose(fw);

	if (rank == 0){
		printf("num of procs : %d, flip par time : %.6lf\n", numprocs, finish-start);
	}

	MPI_Type_free(&ppixel);
	MPI_Finalize();

	return 0;
}
