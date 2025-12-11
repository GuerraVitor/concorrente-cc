sem_t mutex;    // inicia com 1 na main
sem_t vazios;   // inicia com N na main
sem_t cheio;    // inicia com 0 na main

int buffer[N];  //iniciado na main

void Insere(int item){
    static int in = 0;
    sem_wait(&vazios);
    sem_wait(&mutex);
    buffer[in] = item;
    in++;
    if (in == N){
        in = 0;
        sem_post(&cheio);
    }
    sem_post(&mutex);
}

void Retira(int itens[]){
    sem_wait(&cheio);
    for (i = 0; i < N; i++){
        itens[i] = buffer[i];
        sem_post(&vazio);
    }
}