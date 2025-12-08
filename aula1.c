#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_hello (void *arg){
	printf("Oi!\n")
	pthread_Exit(NULL);
}

int main()
{
	pthread_t t_id;

	if (pthread_create(&t_id, NULL, print_hello, NULL)){
		printf("erro_1\n")
	}

}
