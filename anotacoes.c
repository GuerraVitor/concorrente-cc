//funções da biblioteca pthread:

int pthread create (pthread t *thread_id, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
	// retorna 0 no caso de sucesso
	// start_routine: recebe um argumento void* e retornar um void*

void pthread exit (void *retval);
	// retval: retorno que a thread disponibiliza para qualquer outra thread (usando pthread_join)

int pthread join (pthread_t thread_id, void **retval);
	// retorna 0 no caso de sucesso
	// thread_id: id da thread que se espera
	// retval: recebe o valor da thread que terminou
