#include <stdio.h>
#include <pthread.h>

void * mensagem(void * arg){
	printf("minha thread\n");
	pthread_exit(NULL);
}

int main(void){
	pthread_t tid;
	printf("thread principal\n");
	if(pthread_create(&tid, NULL, mensagem, NULL))
		printf("erro ao criar thread nova\n");
	pthread_exit(NULL);
	return(0);
}
