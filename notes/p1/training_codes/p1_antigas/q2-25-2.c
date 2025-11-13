//na main serão criadas as threads e alocados os argumentos

//estrutura para argumentos de entrada
typedef struct {
    int id;
    int qtde; //tamanho do pedaço do vetor somado por thread
} t_args;

//vetor para salvar
float *vetor;

void *soma(void *args){ 
    //salvando args passados 
    t_args *arg = (t_args *) args;

    float local = 0; //inicia var local
    float *ret = malloc(sizeof(float));
    //verifica alloc....
    
    int ini = arg->qtde * arg->id;
    int fim = ini + arg->qtde;

    for (int i = ini; i < fim, i++){
        local += vetor[i];
    }
    *ret = local;
    pthread_exit((void *)ret);
}