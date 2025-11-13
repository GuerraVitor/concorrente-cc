//vetor carregado na main
int *vetor;

//argumentos de entrada das threads
typedef struct {
    int id;
    int qtde; //qtde de elemento que cada thread vai olhar
} t_args;

typedef struct {
    int min, max;
} t_ret;

void *procura(void *args){
    //pegando argumentos de entrada
    t_args *arg = (t_args *) args;
    int ini = arg->id * arg->qtde;
    int fim = ini + arg->qtde;
    free(args);

    //aloca a inicia var de retorno
    t_ret *ret = malloc(sizeof(t_ret));
    if (ret == NULL)
        return 1;
    ret->min = ret->max = vetor[ini];

    //calcula min e max
    for (int i = ini; i < fim; i++){
        if (vetor[i] < ret->min)
            ret->min = vetor[i];
        else if (vetor[i] > ret->max)
            ret->max = vetor[i]; 
    }
    pthread_exit((void *) ret);
}