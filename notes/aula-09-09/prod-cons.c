//aula 09-09-25 prog concorrente

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//variaveis globais
int vetor[1];
short int estado = 0; //0:vazio 1:cheio
pthread_mutex_t mutex;
pthread_cond_t insere, retira;

void inserir (int elemento) {
  pthread_mutex_lock(&mutex);
  while(estado)
      pthread_cond_wait(&insere, &mutex);
  vetor[0] = elemento;
  estado = 1;
  pthread_cond_signal(&retira);
  pthread_mutex_unlock(&mutex);

  int Retirar (void) {
    pthread_mutex_lock(&mutex);
    while(!estado)
      pthread_cond_wait(&retira, &mutex);
  }
  aux = vetor[0];
  estado = 0;
  pthread_cond_signal(&insere);
  pthread_mutex_unlock(&mutex);
  return aux;
}

//parte concorrente
void * p (void args) {
  //incilializado
  //...
  //produzindo novo elemento
  //...
  Inserir(elemento);
  //...
  //produzindo de novo
  //...
  Inserir(elemento);
  //...
}

