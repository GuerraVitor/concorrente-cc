//aula 09-09-25 prog concorrente

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Define o tamanho do buffer.
#define TAMANHO_BUFFER 5

// variaveis globais
int vetor[TAMANHO_BUFFER];
int contador = 0; // Quantidade de elementos no buffer
int entrada = 0;  // Proxima posicao para inserir
int saida = 0;    // Proxima posicao para retirar

pthread_mutex_t mutex;
pthread_cond_t insere, retira;

void inserir (int elemento) {
  pthread_mutex_lock(&mutex);
  while(contador == TAMANHO_BUFFER) {
      pthread_cond_wait(&insere, &mutex);
  }
  vetor[entrada] = elemento;
  entrada = (entrada + 1) % TAMANHO_BUFFER;
  contador++;
  pthread_cond_signal(&retira);
  pthread_mutex_unlock(&mutex);
}

int retirar (void) {
  int aux;
  pthread_mutex_lock(&mutex);
  while(contador == 0) {
      pthread_cond_wait(&retira, &mutex);
  }
  aux = vetor[saida];
  saida = (saida + 1) % TAMANHO_BUFFER;
  contador--;
  pthread_cond_signal(&insere);
  pthread_mutex_unlock(&mutex);
  return aux;
}

// parte concorrente
void * p (void *args) {
  // incializado
  // ...
  // produzindo novo elemento
  // ...
  // Aqui voce usaria 'inserir(elemento);'
  // ...
  // produzindo de novo
  // ...
  // Aqui voce usaria 'inserir(elemento);'
  // ...
}
