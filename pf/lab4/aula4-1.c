#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int soma = 0;
//pthread_mutex_t mutex;


void *incrementa(void *arg){
    for(int j = 0; j < 100000; j++){
        //pthread_mutex_lock(&mutex);
        soma++;
        //pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(int ac, char **av){
    int t_n;
    pthread_t *t_id;
    
    if (ac < 2) {printf("Digite o numero de threads\n"); return 1;}
    
    t_n = atoi(av[1]);

    t_id = (pthread_t*) malloc(sizeof(pthread_t) * t_n);
    if (t_id == NULL) {puts("erro--maloc"); return 2;}
    
    //pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < t_n; i++){
        if(pthread_create(&t_id[i], NULL, incrementa, NULL)){
            printf("erro--create\n"); return 3;}
    }

    for(int i = 0; i < t_n; i++){
    if(pthread_join(t_id[i], NULL)){
        printf("erro--join\n"); return 4;}
    }

    //pthread_mutex_destroy(&mutex);

    printf("soma: %ld\n", soma);

    free(t_id);

    return 0;
}