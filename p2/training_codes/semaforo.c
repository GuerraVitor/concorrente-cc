#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t em;
int s = 0;

void *tarefa(){

	sem_wait(&em);
	s++;
	sem_post(&em);
	pthread_exit(NULL);
}

int main(){
	int t;
	int n = 10;

	sem_init(&em, 0, 1);// Inicializa o semáforo para exclusão mútua
	pthread_t tid[n];

	for(t=0; t<n; t++) {// Cria as threads
		if (pthread_create(&tid[t], NULL, tarefa, NULL)) {
			printf("--ERRO: pthread_create()\n");
			exit(-1);}
	}

	for (t = 0; t < n; t++) {// Espera todas as threads terminarem
		pthread_join(tid[t], NULL);
	}

	printf("Valor de s = %d\n", s);
	pthread_exit(NULL);
}
