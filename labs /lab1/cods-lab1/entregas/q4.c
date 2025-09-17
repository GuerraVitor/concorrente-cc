#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *vetor;
int n;

typedef struct {
   int idThread, tamSecao;
} t_Args;

void *incrementa (void *args){
    t_Args *arg = (t_Args *) args;
    int tam = arg->tamSecao;
    int id = arg->idThread;

    for (int i = 0; i < tam; i++){
        vetor[id+i] += 1;
    }
}

int main(int ac, char **av){
    if (ac < 2){
        printf("Erro: informe o tamanho\n");
        return 1;
    }

    n = atoi(av[1]);
    vetor = malloc(sizeof(int) * n);
    if (vetor == NULL){
        printf("Erro: malloc\n");
        return 2;
    }   
}