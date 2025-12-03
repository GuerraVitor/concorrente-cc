#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t em;
int s = 0;

void *tarefa(void *id){

	sem_wait(&em)
	s++;
	sem_post(&em);

}
