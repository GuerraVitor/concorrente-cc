/* Disciplina: Programação Concorrente */
/* Prof.: Silvana Rossetto */
/* Lab5: Atividade 4 - Duas threads B esperam duas threads A */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4 // 2 threads A e 2 threads B

/* Variaveis globais */
short int estado = 0; // Conta quantas threads A já passaram
pthread_mutex_t mutex;
pthread_cond_t cond;

/* Thread A */
void *A (void *t) {
  printf("Thread A: Comecei\n");
  
  // Executa uma computação qualquer...
  printf("HELLO\n");

  // Sinaliza que terminou de executar 'HELLO'
  pthread_mutex_lock(&mutex);
  estado++;
  if (estado == 2) {
      printf("Thread A: estado = %d, vai sinalizar todas as threads B\n", estado);
      pthread_cond_broadcast(&cond); // Desperta todas as threads B
  }
  pthread_mutex_unlock(&mutex);

  // Executa outra computação qualquer...
  pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {
  printf("Thread B: Comecei\n");

  // Verifica se pode executar 'BYEBYE'
  pthread_mutex_lock(&mutex);
  while (estado < 2) { 
     printf("Thread B: estado = %d, vai se bloquear para aguardar a condicao\n", estado);
     pthread_cond_wait(&cond, &mutex); // Bloqueia até a condicao ser atendida
     printf("Thread B: sinal recebido e mutex realocado, estado = %d\n", estado);
  }
  pthread_mutex_unlock(&mutex); 
  
  // Executa 'BYEBYE'
  printf("BYEBYE\n");
  
  // Executa outra computação qualquer...
  pthread_exit(NULL);
}


/* Funcao principal */
int main(int argc, char *argv[]) {
  pthread_t threads[NTHREADS];

  /* Inicializa o mutex e a variavel de condicao */
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  /* Cria as threads */
  // As duas primeiras são A, as duas últimas são B. A ordem pode ser invertida.
  pthread_create(&threads[0], NULL, A, NULL);
  pthread_create(&threads[1], NULL, A, NULL);
  pthread_create(&threads[2], NULL, B, NULL);
  pthread_create(&threads[3], NULL, B, NULL);

  /* Espera todas as threads completarem */
  for (int i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  return 0;
}
