//barreira com semaforos
sem_t mutex; //iniciado na main com valor 1
sem_t cond; //iniciado na main com valor 0
void barreira(int numThreads) {
    sem_wait(&mutex);
    bloqueadas++;

    if (bloqueadas < numThreads) {
        sem_post(&mutex);
        sem_wait(&cond);

        bloqueadas--;
        if (bloqueadas==0) 
            sem_post(&mutex);
        else 
            sem_post(&cond); 
    } 

    else {
        bloqueadas--;
        sem_post(&cond);
    }
}

// consumidor, ele deve consumir o buffer inteiro e espera o buffer ficar completamente cheio
sem_t mutex; //inicia com 1 sinal
sem_t slotCheio; //inicia com 0 sinais
sem_t slotVazio; //inicia com N sinais

int buffer[N];

void insere(int item){
    static int in = 0;
    sem_wait(&slotVazio);
    sem_wait(&mutex);
    buffer[in] = item;
    in++;
    if(in==N){
        in = 0;
        sem_post(&slotCheio);
    }
    sem_post(&mutex)
}

void retira(int itens[]){
    sem_wait(&slotCheio);
    for(int i=0; i<N; i++){
        itens[i] = buffer [i];
        sem_post(%slotVazio);
    }
}

// Prod-Cons em java 
class BufferLimitado {
    private int[] buffer;
    private int N, count = 0, in = 0, out = 0;

    public BufferLimitado(int tamanho) {
        this.N = tamanho;
        this.buffer = new int[N];
    }

    // PRODUTOR
    public synchronized void Insere(int elemento) {
        try {
            // "Enquanto" a condição de bloqueio for verdade, durma.
            // USAR WHILE É OBRIGATÓRIO (não use IF)
            while (count == N) {
                wait(); // Libera o lock e dorme
            }
            
            // Área crítica
            buffer[in] = elemento;
            in = (in + 1) % N;
            count++;
            
            // Avisa consumidores que tem dado novo
            notifyAll(); 
            
        } catch (InterruptedException e) { }
    }

    // CONSUMIDOR
    public synchronized int Remove() {
        try {
            // Enquanto vazio, espera
            while (count == 0) {
                wait();
            }
            
            int elemento = buffer[out];
            out = (out + 1) % N;
            count--;
            
            // Avisa produtores que tem espaço livre
            notifyAll();
            
            return elemento;
        } catch (InterruptedException e) { return -1; }
    }
}

//barreira em Java
class Barreira {
    private int numThreads;
    private int cont = 0;

    public Barreira(int n) {
        this.numThreads = n;
    }

    public synchronized void chegada() {
        cont++;
        // Se ainda não chegaram todos
        if (cont < numThreads) {
            try {
                wait(); // Dorme
            } catch (InterruptedException e) {}
        } else {
            // Sou a última thread!
            cont = 0;    // Reseta para reutilizar (cuidado aqui, ver nota abaixo*)
            notifyAll(); // Libera todo mundo
        }
    }
}