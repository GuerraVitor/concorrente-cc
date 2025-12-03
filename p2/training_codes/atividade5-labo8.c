#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N=5

sem_t mutexProd, mutexCons, lockProd, lockCons;
int buffer[M];

sem_init(&mutexProd);

void	*produtor(int threadid){

}
