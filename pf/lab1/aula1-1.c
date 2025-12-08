#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_hello (void *arg){
	long int t_id = (long int) arg;
	printf("Oi da thread %ld!\n", t_id);
	pthread_exit(NULL);
}

int main(int ac, char **av)
{
	if(ac<2) {
    	printf("--ERRO: informe a qtde de threads <%s> <nthreads>\n", av[0]);
    	return 1;
	}

	long int t_n = atoi(av[1]);
	pthread_t t_id[t_n];

	for(long int i = 0; i < t_n; i++){
		if (pthread_create(&t_id[i-1], NULL, print_hello, (void*) i)){
			printf("-- erro_1\n");
			return 1;
		}
	}
	printf("-- fim da thread principal\n");
	pthread_exit(NULL);
	return 0;
}