#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

typedef struct {
  int id_thread;
  int n_thread;
} t_args;

void *ft_print_oi (void *arg) {
  //typecast para a sctruct
  t_args *args = (t_args *) arg;

  printf ("sou a thread %d do total de %d", args->id_thread, args->n_thread);

  free(arg);

  pthread_exit(NULL);
}

int main(int ac, void **av) {

  if (ac < 2) {
    printf("--erro: informe o numero de threads: <%s> <n_thread>\n", av[0]);
    return 1;
  }
  int n_threads = atoi(av[1]);

  //vetor para threads
  pthread_t list_threads[n_threads];

  //aloca e cria as threads
  for (int i = 0; i < n_threads; i++) {
    t_args *args = malloc(sizeof(t_args));
    if (args) {
      printf ("--erro: malloc()\n");
      return 2;
    }
    args->id_thread = i;
    args->n_thread = n_threads;

    printf("cria thread %d\n", i);
    if (pthread_create(&list_threads[i], NULL, ft_print_oi, (void *) args)){
      printf("--erro: pthread_create()\n");
        return 3;
    }
  }

  //espera threads terminarem
  for (int i = 0; i < n_threads; i++) {
    if (pthread_join(list_threads[i], NULL)) {
      printf("--erro: pthread_join() da thread %d\n", i);
      return 4;
    }
  }

  printf("thread principal terminou\n");

  return 0;
}
