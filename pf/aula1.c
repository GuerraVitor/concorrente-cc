#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_hello (void *arg){
	printf("Oi!\n");
	pthread_exit(NULL);
}

int main()
{
	pthread_t t_id[1];

	if (pthread_create(&t_id[1], NULL, print_hello, NULL)){
		printf("-- erro_1\n");
		return 1;
	}

	pthread_exit(NULL);
	printf("-- fim da thread principal\n");
	return 0;
}