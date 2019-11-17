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

	MPI_Request *reqs;
	MPI_Status sts;




	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);

	start = MPI_Wtime();

	reqs = (MPI_Request*)malloc(sizeof(MPI_Request) * numprocs);
	
	int su = ARRAYNUM / numprocs;
	

	if (rank == 0){
		srand(time(NULL));
		i = 0;
		arr = (int*)malloc(sizeof(int) * ARRAYNUM);
		res = (int*)malloc(sizeof(int) * ARRAYNUM);

		while (i < ARRAYNUM){
			arr[i] = (int)(rand() +1) % 50;			
	//		arr[i] = i;
			i++;
		}
	}
	local_arr = (int*)malloc(sizeof(int) * su);
	local_sum = (int*)malloc(sizeof(int) * su);


	if (rank == 0){
		i = 1;
		while(i < numprocs){
			MPI_Send(arr + i*su, su, MPI_INT, i, 0, MPI_COMM_WORLD);
			i++;
		}
		memcpy(local_arr, arr, su * sizeof(int));
	}
	else{
		MPI_Recv(local_arr, su, MPI_INT, 0, 0, MPI_COMM_WORLD, &sts);
	}



	if (rank == 0){		// first process. only send
		MPI_Send(local_arr, su, MPI_INT, 1, 0, MPI_COMM_WORLD);

		memcpy(local_sum, local_arr, su*sizeof(int));
	}
	else if (rank == numprocs -1){		// last process, only recv
		MPI_Recv(local_sum, su, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &sts);

		i = 0;
		while(i < su){
			local_sum[i] += local_arr[i];
			i++;
		}
	}
	else{

		MPI_Recv(local_sum, su, MPI_INT, rank - 1, 0 ,MPI_COMM_WORLD, &sts);

		i = 0;
		while(i < su){
			local_sum[i] += local_arr[i];
			i++;
		}
		MPI_Send(local_sum, su, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

	}



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

	if (rank == 0){
		i = 1;
		while(i < numprocs){
			MPI_Recv(res + i*su, su, MPI_INT, i, 0, MPI_COMM_WORLD, &sts);
			i++;
		}

		memcpy(res, local_sum, sizeof(int) * su);
	}
	else{
		MPI_Send(local_sum, su, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}



//	printf("process %d on %s out of %d \n", rank, processor_name, numprocs);
	if (rank == 0){
		i = su * numprocs;
		while(i < ARRAYNUM){
			res[i] = res[i-1] + arr[i];
			i++;
		}
/*
		i = 0;
		while (i < ARRAYNUM){
			printf("%d ", res[i]);
			i++;
		}*/
	}

	finish = MPI_Wtime();

	if (rank == 0)
		printf("number of node : %d, time %.6lf secs \n", numprocs, finish - start);


	if(arr) free(arr);
	if(res) free(res);
	if(local_arr) free(local_arr);
	if(local_sum) free(local_sum);
	if(reqs) free(reqs);

	MPI_Finalize();


	return 0;
}
