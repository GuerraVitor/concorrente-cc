#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int soma = 0;
pthread_mutex_t mutex;


void *incrementa(void *arg){
    for(int j = 0; j < 100000; j++){
        pthread_mutex_lock(&mutex);
        soma++;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(int ac, char **av){
    int t_n;
    if (ac < 2){
        printf("Digite o numero de threads\n");
        return 1;
    }
    t_n = atoi(av[1]);
    

    for(int i = 0; i< t_n; i++){
        if()
    }
    pthread_t t_id[t_n];
    pthread_mutex_init(&mutex, NULL);


}