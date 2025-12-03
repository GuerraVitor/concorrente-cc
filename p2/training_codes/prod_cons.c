sem_t slotCheio, slotVazio;
sem_t mutexProd, mutexCons;
int Buffer[B];
...
sem_init(&mutexCons, 0, 1);
sem_init(&mutexProd, 0, 1);
sem_init(&slotCheio, 0, 1);
sem_init(&slotVazio, 0, B);

void Insere(int item) {
	static int in = 0;
	sem_wait(&slotVazio);
	sem_wait(&mutexProd);
	Buffer[in] = item;
	in = (in + 1) % B;
	sem_post(&mutexProd);
	sem_post(&slotCheio)
}

int Retira(void){
	int item;
	static int out = 0;
	sem_wait(&slotCheio);
	sem_wait(&mutexCons);
	out = (out + 1) % B;
	sem_post(&slotCheio);
	sem_post(&slotVazio);
	return item;
}
