#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define ARRAYNUM 10000 

int main(int argc, char*argv[]){
	int numprocs, rank, namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int *arr = NULL, *res = NULL, *local_arr = NULL, *local_sum = NULL;
	int i;


	double start, finish;

	start = MPI_Wtime();


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);


	int su = ARRAYNUM / numprocs;
	

	if (rank == 0){
		srand(time(NULL));
		i = 0;
		arr = (int*)malloc(sizeof(int) * ARRAYNUM);
		res = (int*)malloc(sizeof(int) * ARRAYNUM);

		while (i < ARRAYNUM){
			arr[i] = (int)(rand() + 1) % 50;			
			i++;
		}
	}
	local_arr = (int*)malloc(sizeof(int) * su);
	local_sum = (int*)malloc(sizeof(int) * su);

	MPI_Scatter(arr, su, MPI_INT, local_arr, su, MPI_INT, 0, MPI_COMM_WORLD);
	
	
	MPI_Scan(local_arr, local_sum, su, MPI_INT, MPI_SUM, MPI_COMM_WORLD);


	i = 0;
	int ptmp = 0;
	while(i < su){
		ptmp += local_sum[i++];
	}
	i = 0;
	while (i < su){
		local_sum[i] = ptmp;
		int j = i+1;
		while (j < su){
			local_sum[i] -= local_arr[j++];
		}
		i++;
	}


	
	MPI_Barrier(MPI_COMM_WORLD);	
	MPI_Gather(local_sum, su, MPI_INT, res, su, MPI_INT, 0, MPI_COMM_WORLD);




//	printf("process %d on %s out of %d \n", rank, processor_name, numprocs);
	if (rank == 0){
		i = su * numprocs;
		while(i < ARRAYNUM){
			res[i] = res[i-1] + arr[i];
			i++;
		}
	}

	
	finish = MPI_Wtime();

	if (rank == 0)
		printf("number of procs : %d, time : %.6lf secs\n", numprocs, finish-start);

	if(arr) free(arr);
	if(res) free(res);
	if(local_arr) free(local_arr);
	if(local_sum) free(local_sum);

	MPI_Finalize();


	return 0;
}
