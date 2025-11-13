#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *ft_print_hello(void *arg) {

  long int id_thread = (long int) arg;

  printf("Oi mundo da thread: %ld\n", id_thread);

  pthread_exit(NULL);
}

int main(int ac, char **av) {
  
  int n_threads;

  if (ac < 2) {
    printf("erro: informe a qtde de threads <%s> <n_threads", av[0]);
    return 1;
  }

  n_threads = atoi(av[1]);

  pthread_t list_id_threads[n_threads];

  for (long int i = 0; i < n_threads; i++) {
    printf("--cria thread %ld\n", i);  
    //cria a thread
    if(pthread_create(&list_id_threads[i], NULL, ft_print_hello, (void*) i)) {
      printf("-erro: pthread_create()\n");
      return 2;
    }
  }

  //espera termino das threads para encerrar a main
  for (int i = 0; i < n_threads; i++) {
    if (pthread_join(list_id_threads[i], NULL)) {
      printf("--erro: pthread_join da thread %d\n", i);
    }
  }

  printf ("--thread principal terminou\n");

  return 0;

}
