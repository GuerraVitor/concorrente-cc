sem_t mutex;    // inicia com 1 na main
sem_t cond;     // inicia com 0 na main
int bloq;       // inicia com 0 na main

void barreira(int nt){
    sem_wait(&mutex);
    bloq++;
    if (bloq < nt){
        sem_post(&mutex);
        sem_wait(&cond);
        bloq--;
        if (bloq == 0){
            sem_pos(&cond);
        }
        sem_post(&mutex);
    }
    else {
        bloq--;
        sem_post(&cond);
    }
}

