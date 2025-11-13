typedef struct {
    int id, n;
} t_args;

typedef struct {
    int min, max;
} t_ret;

int *vetor;

void *acha(void *arg){
    //armazeno argumentos de entrada
    t_args *args = (t_args *) arg;
    int id = args->id;
    int n = args->n;
    //cria e alloca retorno
    t_ret *ret = malloc(sizeof(t_ret));
    if (ret == NULL){
        printf("Erro: malloc\n");
        pthread_exit(NULL);     
    }
    ret->min = ret->max = vetor[id];
}