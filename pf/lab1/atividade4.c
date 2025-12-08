#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *vetor;
int n, tam;

typedef struct {
    int t_id, inicio, fim;
} t_args;

void *incr_vetor(void *arg){
    t_args *args = (t_args *)arg;

    for(int i=args->inicio; i<args->fim; i++){
        vetor[i] += 1;
    }
    pthread_exit(NULL);
}

void inicializa_vetor(int tam) {
    vetor = (int *) malloc(sizeof(int) * tam);
    if (vetor == NULL) {
        fprintf(stderr, "ERRO: malloc\n");
        exit(1);
    }
    for (int i = 0; i < tam; i++) {
        vetor[i] = 0;
    }
}

int main(int ac, char **av){
    pthread_t t_id[4];
    t_args args[4];

    if (ac < 2){
        printf("digite o valor de N\n");
        return 1;
    }
    n = atoi(av[1]);
    tam = 4 * n;

    inicializa_vetor(tam);
    
    for(int i =0; i<4; i++){
        args[i].t_id = i;
        args[i].inicio = i*n;
        args[i].fim = (i+1)*n;
        pthread_create(&t_id[i], NULL, incr_vetor, &args[i]);
    }
    
    for(int i =0; i <4; i++){
        pthread_join(t_id[i], NULL);
    }

    for (int i = 0; i < tam; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
    free(vetor);
    return 0;

}