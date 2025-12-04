/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e sincronização com semáforos */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS  2


int s = 0;
sem_t em;

void *ExecutaTarefa (void *threadid) {
	int i;
	int *tid = (int*) threadid;
	printf("Thread : %d esta executando...\n", *tid);
	for (i=0; i<1000000; i++) {
		sem_wait(&em);
		s++;
		sem_post(&em);
	}
	printf("Thread : %d terminou!\n", *tid);
	free(threadid);
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	pthread_t tid[NTHREADS];
	int t, *id;


	sem_init(&em, 0, 1);

	for(t=0; t<NTHREADS; t++) {
		if ((id = malloc(sizeof(int))) == NULL) {
			pthread_exit(NULL);
			return 1;}
		*id=t;
		if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)id)) {
			printf("--ERRO: pthread_create()\n");
			exit(-1);}
		}
	for (t=0; t<NTHREADS; t++) {
		if (pthread_join(tid[t], NULL)) {
			printf("--ERRO: pthread_join() \n");
			exit(-1);}
	}
	printf("Valor de s = %d\n", s);
	pthread_exit(NULL);
}
