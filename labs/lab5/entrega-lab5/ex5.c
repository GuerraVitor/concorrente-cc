/* Disciplina: Programação Concorrente */
/* Prof.: Silvana Rossetto */
/* Lab5: Atividade 5 - Três threads A executam antes de três threads B */
// Aluno: Vitor Hugo Guerra 
// DRE: 12360066

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 6 // 3 threads A e 3 threads B

// Variáveis globais 
short int estado = 0; // Conta quantas threads A já passaram
pthread_mutex_t mutex;
pthread_cond_t cond;

// Thread A 
void *A(void *t) {
  printf("Thread A: Comecei\n");
  printf("HELLO\n");

  // Sinaliza que terminou de executar 'HELLO'
  pthread_mutex_lock(&mutex);
  estado++;
  if (estado == 3) {
    printf("Thread A: estado = %d, vai sinalizar todas as threads B\n", estado);
    pthread_cond_broadcast(&cond); // Desperta todas as threads B
  }
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

// Thread B 
void *B(void *t) {
  printf("Thread B: Comecei\n");

  // Verifica se pode executar 'BYEBYE'
  pthread_mutex_lock(&mutex);
  while (estado < 3) {
    printf("Thread B: estado = %d, vai se bloquear para aguardar a condicao\n", estado);
    pthread_cond_wait(&cond, &mutex); // Bloqueia até a condição ser atendida
    printf("Thread B: sinal recebido e mutex realocado, estado = %d\n", estado);
  }
  pthread_mutex_unlock(&mutex);

  // Executa 'BYEBYE'
  printf("BYEBYE\n");

  pthread_exit(NULL);
}

// Função principal 
int main(int argc, char *argv[]) {
  pthread_t threads[NTHREADS];

  // Inicializa o mutex e a variável de condição
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  // Cria as threads 
  // As três primeiras são A, as três últimas são B. A ordem pode ser invertida.
  pthread_create(&threads[0], NULL, A, NULL);
  pthread_create(&threads[1], NULL, A, NULL);
  pthread_create(&threads[2], NULL, A, NULL);
  pthread_create(&threads[3], NULL, B, NULL);
  pthread_create(&threads[4], NULL, B, NULL);
  pthread_create(&threads[5], NULL, B, NULL);

  // Espera todas as threads completarem 
  for (int i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf("\nFIM\n");

  // Desaloca variáveis e termina
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  return 0;
}