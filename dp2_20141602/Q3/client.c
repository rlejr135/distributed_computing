#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <pthread.h>
#include <semaphore.h>

#include <time.h>



#define DATANUM 100000

void * background(void *aux);

int port;
int thread_num;
char filename[255];


int working_thread = 0;
int total_data = 0;
double total_time = 0;

int CONNECTNUM = 0;
int howmany = 0;

pthread_mutex_t tnum_lock, data_lock;
	


int main(int argc, char *argv[]){


	int i;
	pthread_t t_id;
	int finish_flag = 0;
	
	struct timespec startTS, endTS;
	const long long NANOS = 1000000000LL;
	double tmp_time = 0;

    
	if (argc != 5)
    {
        printf("Usage : ./client [port] [num of thread] [read filename] [loop count]\n");
        exit(0);
    }

	port = atoi(argv[1]);
	thread_num = atoi(argv[2]);
	strcpy(filename, argv[3]);
	CONNECTNUM = atoi(argv[4]);


	howmany = CONNECTNUM/thread_num;

	printf("get [%s]'s data... \n", filename);

	pthread_mutex_init(&tnum_lock, NULL);
	pthread_mutex_init(&data_lock, NULL);


	/*
	   make thread here
	 */


	i = 0;
	for (i = 0 ; i < thread_num ; i++){
		pthread_mutex_lock(&tnum_lock);
		working_thread++;
		pthread_mutex_unlock(&tnum_lock);
		pthread_create(&t_id, NULL, background, NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &startTS);

	while(1){

		pthread_mutex_lock(&tnum_lock);

		if (working_thread == 0){
			finish_flag = 1;
		}
		pthread_mutex_unlock(&tnum_lock);

		if (finish_flag) break;
	}
	clock_gettime(CLOCK_MONOTONIC, &endTS);

	tmp_time = (endTS.tv_sec - startTS.tv_sec) + (double)(endTS.tv_nsec - startTS.tv_nsec)/NANOS;

	total_time += tmp_time;


	printf("total time : %f\n", total_time);
	printf("total data : %d\n", total_data);


	return 0;



	/*
	 	check 
	 */


	return 0;
}



void* background(void *aux){

    int client_len;
    int client_sockfd;

    char buf_in[2550];
    char buf_get[100000];

    struct sockaddr_in clientaddr;

	int get_data_num = 0;

	int i;

	for (i = 0 ; i < howmany ; i++){
		client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		clientaddr.sin_family = AF_INET;
		clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		clientaddr.sin_port = htons(port);

		client_len = sizeof(clientaddr);

		if (connect(client_sockfd, (struct sockaddr *)&clientaddr, client_len) < 0)
		{
			perror("Connect error: ");
			exit(0);
		}


		sprintf(buf_in, "GET /%s HTTP/1.0 \r\n\r\n", filename);

		write(client_sockfd, buf_in, strlen(buf_in));

		get_data_num  = read(client_sockfd, buf_get, DATANUM);

		close(client_sockfd);
		pthread_mutex_lock(&data_lock);
		total_data += get_data_num;
		pthread_mutex_unlock(&data_lock);
	}


	pthread_mutex_lock(&tnum_lock);
	working_thread--;
	pthread_mutex_unlock(&tnum_lock);
	

	pthread_exit(0);

	return NULL;
}
