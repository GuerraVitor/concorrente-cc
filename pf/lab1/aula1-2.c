#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int t_id, t_n;
} t_args;

void *print_hello (void *arg){
	t_args args = *(t_args*) arg;
	printf("sou a threada %d de %d threads\n", args.t_id, args.t_n);
	free(arg);
	pthread_exit(NULL);
}

int main(int ac, char **av)
{
	t_args *args;
	int t_n;

	if(ac<2) {
    	printf("--ERRO: informe a qtde de threads <%s> <nthreads>\n", av[0]);
    	return 1;
	}
	t_n = atoi(av[1]);

	pthread_t t_id[t_n];

	for(int i=1; i<t_n; i++) {
		printf("alocando e preenchendo argumentos da thread _%d\n", i);
		args = malloc(sizeof(t_args));
		if (args == NULL){
			printf("--ERRO: malloc()\n");
      		return 2;
		}
		args->t_id = i;
		args->t_n = t_n;
		if (pthread_create(&t_id[i-1], NULL, print_hello, (void*) args)){
			printf("-- erro na thread %d\n", i);
		}
	}

	printf("-- fim da thread principal\n");
	pthread_exit(NULL);
	return 0;
}