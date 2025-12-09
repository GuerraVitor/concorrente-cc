1  - variaveis de escopo global
2  - Struct de args e retornos 
3  - função executada pelas threads
   - na main:
4  - criar tid e tn
5  - alocar tipo pthread_t para tid
6  - pthread_mutex_init(&mutex, NULL)
7  - criar threads
8  - espera threads terminarem
9  - pthread_mutex_destroy(&mutex)
10 - free para t_id



int pthread create (pthread t *thread_id, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
	// retorna 0 no caso de sucesso
	// start_routine: recebe um argumento void* e retornar um void*

void pthread exit (void *retval);
	// retval: retorno que a thread disponibiliza para qualquer outra thread (usando pthread_join)

int pthread join (pthread_t thread_id, void **retval);
	// retorna 0 no caso de sucesso
	// thread_id: id da thread que se espera
	// retval: recebe o valor da thread que terminou

// 5 - alocar tipo pthread_t para tid
tid = (pthread_t*) malloc(sizeof(pthread_t)*(tn+1));
if(tid==NULL) {puts("ERRO--malloc"); return 1;}

// 7 - criar threads
for(long int i=0; t<tn; i++) {
	if (pthread_create(&tid[i], NULL, ExecutaTarefa, (void *)i)) {
	printf("--ERRO\n"); exit(-1);
	}
}

// 8 - espera threads terminarem
for (int i=0; i<nt+1; i++) {
	if (pthread_join(tid[i], NULL)) {
		printf("--ERRO\n"); exit(-1); 
	} 
} 

//cria variavel de Sincronização por exclusão mutua
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_lock(&mutex);
	//aqui estaria a seção crítica do codigo
	pthread_mutex_unlock(&mutex);
	
//cria variavel de sincronização por condição
	pthread_cond_t cond
	pthread_cond_init (&cond, NULL)
	pthread_cond_destroy (cond)
	pthread_cond_wait(&cond, &mutex)
		//bloqueia a thread na condição (cond)
		//chamada com mutex locado, desloca mutex quando finaliza
	pthread_cond_signal(&cond)
		//desbloqueia uma thread esperando pela condição (cond)
	
	
	
//exemplo de função executada pela thread
void *print_hello (void *arg){
	t_args args = *(t_args*) arg;
	printf("thread %d de %d\n", args.t_id, args.t_n);
	free(arg);
	pthread_exit(NULL);
}
//exemplo de função executada pela thread com retorno
void *PrintHello (void *arg) {
  t_ret *ret;
  t_args *args = (t_args*) arg;

  ret = malloc(sizeof(t_ret));
  if (ret==NULL) {
     printf("--ERRO: malloc() thread\n");
     pthread_exit(NULL);
  }
  ret->t_id = args->t_id;
  ret->aux = args->t_id * 2;
  free(arg);

  pthread_exit((void*) ret);
}
