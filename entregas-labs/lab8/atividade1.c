#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

// Variáveis globais para o buffer e sincronização
int* buffer;
int N_NUMBERS;
int M_BUFFER_SIZE;
int total_primes = 0;
int winner_consumer_id = -1;
int max_primes_found = -1;
int read_index = 0;
int write_index = 0;

// Semáforos
sem_t empty_slots; // Conta o número de espaços vazios no buffer
sem_t full_slots;  // Conta o número de espaços preenchidos no buffer
sem_t mutex;       // Garante a exclusão mútua ao buffer e variáveis compartilhadas

// Estrutura para os dados da thread consumidora
typedef struct {
    int id;
    int primes_found;
} consumer_data;

// Variáveis para a thread consumidora vencedora
consumer_data* consumer_stats;

// Função para verificar se um número é primo
int ehPrimo(long long int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Função da thread produtora
void* producer_thread(void* arg) {
    long long int i;
    int num_consumers = *((int*)arg);
    
    // Parte 1: Produtora insere os primeiros M elementos de uma vez
    // Espera o buffer ficar completamente vazio.
    sem_wait(&mutex); // Protege o acesso ao buffer
    
    // Espera por M slots vazios
    for (i = 0; i < M_BUFFER_SIZE; i++) {
        sem_wait(&empty_slots);
    }

    // Insere todos os M elementos de uma vez.
    for (i = 0; i < M_BUFFER_SIZE; i++) {
        buffer[i] = i + 1;
    }
    
    // Libera os slots completos para as consumidoras.
    sem_post(&mutex);
    for (i = 0; i < M_BUFFER_SIZE; i++) {
        sem_post(&full_slots);
    }
    
    // Parte 2: Continua a produzir os N-M números restantes, um por um.
    for (i = M_BUFFER_SIZE + 1; i <= N_NUMBERS; i++) {
        sem_wait(&empty_slots);
        sem_wait(&mutex);
        
        buffer[write_index] = i;
        write_index = (write_index + 1) % M_BUFFER_SIZE;
        
        sem_post(&mutex);
        sem_post(&full_slots);
    }
    
    // Sinaliza o fim do trabalho para todas as threads consumidoras
    for (i = 0; i < num_consumers; i++) {
        sem_wait(&empty_slots);
        sem_wait(&mutex);
        buffer[write_index] = -1; // Sentinela
        write_index = (write_index + 1) % M_BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&full_slots);
    }

    pthread_exit(NULL);
}

// Função das threads consumidoras
void* consumer_thread(void* arg) {
    consumer_data* my_data = (consumer_data*) arg;
    int number_to_check;

    while (1) {
        sem_wait(&full_slots);
        sem_wait(&mutex);
        
        number_to_check = buffer[read_index];
        read_index = (read_index + 1) % M_BUFFER_SIZE;

        sem_post(&mutex);
        
        if (number_to_check == -1) {
            // Se for o sinal de parada, re-envia para a próxima consumidora
            sem_post(&empty_slots);
            break; // Sai do loop
        }
        
        sem_post(&empty_slots);
        
        // Verifica a primalidade e atualiza as estatísticas
        if (ehPrimo(number_to_check)) {
            my_data->primes_found++;
        }
    }

    // Atualiza as variáveis globais de forma segura
    sem_wait(&mutex);
    total_primes += my_data->primes_found;
    if (my_data->primes_found > max_primes_found) {
        max_primes_found = my_data->primes_found;
        winner_consumer_id = my_data->id;
    } else if (my_data->primes_found == max_primes_found && my_data->id < winner_consumer_id) {
         winner_consumer_id = my_data->id;
    }
    sem_post(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Uso: %s <N_NUMBERS> <M_BUFFER_SIZE> <NUM_CONSUMERS>\n", argv[0]);
        return 1;
    }

    // Lê os argumentos da linha de comando
    N_NUMBERS = atoi(argv[1]);
    M_BUFFER_SIZE = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    // Aloca o buffer e o array de estatísticas das consumidoras
    buffer = (int*)malloc(sizeof(int) * M_BUFFER_SIZE);
    consumer_stats = (consumer_data*)malloc(sizeof(consumer_data) * num_consumers);

    // Inicializa os semáforos
    sem_init(&empty_slots, 0, M_BUFFER_SIZE); // Inicialmente, o buffer está vazio
    sem_init(&full_slots, 0, 0);               // Inicialmente, não há itens no buffer
    sem_init(&mutex, 0, 1);                    // Inicializa o mutex para 1

    // Cria as threads
    pthread_t producer_tid;
    pthread_t* consumer_tids = (pthread_t*)malloc(sizeof(pthread_t) * num_consumers);

    // Cria a thread produtora
    pthread_create(&producer_tid, NULL, producer_thread, (void*)&num_consumers);

    // Cria as threads consumidoras
    for (int i = 0; i < num_consumers; i++) {
        consumer_stats[i].id = i;
        consumer_stats[i].primes_found = 0;
        pthread_create(&consumer_tids[i], NULL, consumer_thread, (void*)&consumer_stats[i]);
    }

    // Aguarda o término das threads
    pthread_join(producer_tid, NULL);
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumer_tids[i], NULL);
    }

    // Exibe os resultados
    printf("Quantidade total de primos encontrados: %d\n", total_primes);
    printf("Thread consumidora vencedora (ID): %d\n", winner_consumer_id);
    printf("Quantidade de primos encontrados pela vencedora: %d\n", max_primes_found);

    // Libera a memória alocada
    free(buffer);
    free(consumer_tids);
    free(consumer_stats);

    // Destrói os semáforos
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    sem_destroy(&mutex);

    return 0;
}