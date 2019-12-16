#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_SIZE 1024
#define MAX_PTHREADS 1024
pthread_mutex_t lock;

int ind=1;
int r_num[MAX_SIZE];
int arr[MAX_PTHREADS];

//sem_t mutexlock;
sem_t r_mutex[MAX_SIZE];
sem_t w_mutex;
sem_t x_mutex[MAX_SIZE];
sem_t deq_mutex, enq_mutex;



void *reading(void *args)
{
	int pos = *(int*)args;
	sem_wait(&r_mutex[pos]);
	//int *myid=(int *)id;
	if(r_num[pos] == 0)
	{
		sem_wait(&x_mutex[pos]);
		if(pos < 0 || pos >= ind)
		{
			printf("index is out of bounds.\n");
			sem_post(&x_mutex[pos]);
			sem_post(&r_mutex[pos]);
		//	return ;
		}

		else
		{
			r_num[pos]++;
			sem_post(&r_mutex[pos]);

			int val = arr[pos];
			sleep(2);
			printf("The value of the element read from index %d is %d.\n",pos, val);

			sem_wait(&r_mutex[pos]);
			r_num[pos]--;
			if(r_num[pos] == 0)
			{
				sem_post(&x_mutex[pos]);
			}
			sem_post(&r_mutex[pos]);
		}
	}




}


void *writing(void* args)
{
	//int r_arg = args;
	int w_args= *(int*)args;
//	int *myid=(int *)id;
//	int num;
//	pthread_mutex_lock(&lock); 
	sem_wait(&w_mutex);
	sem_wait(&enq_mutex);
	sem_wait(&x_mutex[ind]);
		//sleep(2);
		//sq[R++] = w_args->val;
	//	printf("Writer %d: Element having value %d has been enqueued at index %d.\n", w_args->no, sq[pos], pos);
		
	arr[ind]=w_args;
	ind++;
	//printf("%d\n", ind);

	sleep(2);
	sem_post(&x_mutex[ind-1]);

	sem_post(&enq_mutex);
	sem_post(&w_mutex);
//	printf("%d\n writer with id Enter number to be added in the queue ",id );
	//scanf("%d\n,%d\n", num);
	//	arr[ind]=num;

//   pthread_mutex_unlock(&lock);



}
/* num of readers
num of writers*/
int main()
{

	pthread_t treadarr[MAX_SIZE];
	pthread_t twritearr[MAX_SIZE];
	int w_args[MAX_SIZE];
	int r_args[MAX_SIZE];
	int num_read, num_write;

	arr[0]=1;
	
	for(int i = 0; i < MAX_SIZE; i++)
	{
		r_num[i] = 0;
		sem_init(&r_mutex[i], 0, 1);
		sem_init(&x_mutex[i], 0, 1);
	}
	sem_init(&w_mutex, 0, 1);
	sem_init(&enq_mutex, 0, 1);
//	sem_init(&mutexlock,0,1);

	while(1)
	{
		printf("Enter num of readers" );
		scanf("%d", &num_read);
		if(num_read<=0 || num_read>MAX_PTHREADS)
		{
			printf("Limit exceeded");

		}
		else
		{
			break;
		}

	}

	while(1)
	{
		printf("Enter num of writers" );
		scanf("%d", &num_write);
		if(num_write<=0 || num_write>MAX_PTHREADS)
		{
			printf("Limit exceeded");

		}
		else
		{
			break;
		}

	}
	for(int i = 0; i < num_read; i++)
	{
			while(1)
			{
				printf("Enter the index of the element to be read by reader %d: ", i + 1);
				scanf("%d", &r_args[i]);
				if(r_args[i] < 0 || r_args[i] >= MAX_SIZE)
					printf("ind must be an integer between 0 and %d, try again.\n", MAX_SIZE - 1);
				else
					break;
			}
		
	}

	for(int i = 0; i < num_write; i++)
	{
		//w_args[i].no = i + 1;			
			printf("Enter the element to be written, by writer %d: ", i + 1);
			scanf("%d", &w_args[i]);
		//	if(w_args[i]< 0 || w_args[i]>= MAX_SIZE)
		//		printf("ind must be an integer between 0 and %d, try again.\n", MAX_SIZE - 1);
		//	else
		//		break;
		
		
	}
	for(int i=0; i<num_write; i++)
	{
		pthread_create(&twritearr[i],NULL, writing,(void*)&w_args[i]);
		//pthread_join(i,NULL);
	}


	for(int i=0; i<num_read; i++)
	{
		pthread_create(&treadarr[i],NULL, reading,(void*)&r_args[i]);
		//pthread_join(i,NULL);
	}
		
	for(int i = 0; i < num_read; i++)

		pthread_join(treadarr[i], NULL);

	for(int i = 0; i < num_write; i++)

		pthread_join(twritearr[i], NULL);
	return 1;

}
