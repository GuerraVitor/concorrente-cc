// Disciplina: Programacao Concorrente 
// Aluno: Vitor Hugo Guerra 
// DRE: 12360066
// 04/09 - lab5 


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int soma = 0; // variavel compartilhada entre as threads
pthread_mutex_t mutex; // variavel de lock para exclusao mutua
pthread_cond_t cond_executa_tarefa; // variavel de condicao para a thread executora
pthread_cond_t cond_extra; // variavel de condicao para a thread extra
int pronto_para_imprimir = 0; // variavel de estado

// funcao executada pelas threads de incremento
void *ExecutaTarefa (void *arg) {
  long int id = (long int) arg;
  printf("Thread : %ld esta executando...\n", id);

  for (int i=0; i<100000; i++) {
     // -- entrada na SC
     pthread_mutex_lock(&mutex);
     soma++; // incrementa a variavel compartilhada

     // Verifica se o valor de 'soma' é um múltiplo de 1000
     if (soma % 1000 == 0) {
        // Sinaliza a thread extra que há um valor para imprimir
        pronto_para_imprimir = 1;
        pthread_cond_signal(&cond_extra);
        
        // Aguarda a confirmação da thread extra antes de continuar o loop
        while (pronto_para_imprimir == 1) {
            pthread_cond_wait(&cond_executa_tarefa, &mutex);
        }
     }
     
     // -- saida da SC
     pthread_mutex_unlock(&mutex);
  }
  printf("Thread : %ld terminou!\n", id);
  pthread_exit(NULL);
}

// funcao executada pela thread de log
void *extra (void *args) {
  printf("Extra : esta executando...\n");
  for (int i=0; i<100; i++) { // Esperamos 100 múltiplos de 1000
     // -- entrada na SC
     pthread_mutex_lock(&mutex);
     // Espera a thread executora sinalizar que há um valor múltiplo de 1000
     while (pronto_para_imprimir == 0) {
        pthread_cond_wait(&cond_extra, &mutex);
     }
     
     printf("soma = %ld \n", soma);
     
     // Sinaliza a thread executora para que ela possa continuar
     pronto_para_imprimir = 0;
     pthread_cond_signal(&cond_executa_tarefa);
     
     // -- saida da SC
     pthread_mutex_unlock(&mutex);
  }
  printf("Extra : terminou!\n");
  pthread_exit(NULL);
}

// fluxo principal
int main(int argc, char *argv[]) {
   pthread_t *tid; // identificadores das threads no sistema
   int nthreads; // qtde de threads (passada linha de comando)

   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);

   tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
   if(tid==NULL) {
      puts("ERRO--malloc");
      return 2;
   }

   // inicializa o mutex e as variaveis de condicao
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&cond_executa_tarefa, NULL);
   pthread_cond_init(&cond_extra, NULL);

   // cria as threads de incremento
   for(long int t=0; t<nthreads; t++) {
     if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
     }
   }

   // cria thread de log
   if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
   }

   // espera todas as threads terminarem
   for (int t=0; t<nthreads+1; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
     } 
   } 

   // finaliza o mutex e as variaveis de condicao
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond_executa_tarefa);
   pthread_cond_destroy(&cond_extra);
   
   printf("Valor de 'soma' = %ld\n", soma);

   return 0;
}
