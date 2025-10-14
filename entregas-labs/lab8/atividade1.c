// Compile com: gcc -o atividade1 atividade1.c prime_counter.c -lpthread -lm

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include "prime_counter.h"

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
    int num_consumers = *((int*)arg);
    int current_number = 1;

    while (current_number <= N_NUMBERS) {
        // Espera o buffer ficar completamente vazio
        for (int i = 0; i < M_BUFFER_SIZE; i++) {
            sem_wait(&empty_slots);
        }

        // Preenche o buffer com um lote de números
        sem_wait(&mutex); // Protege o acesso ao buffer para escrita
        int items_to_produce = 0;
        while (items_to_produce < M_BUFFER_SIZE && current_number <= N_NUMBERS) {
            buffer[write_index] = current_number;
            write_index = (write_index + 1) % M_BUFFER_SIZE;
            current_number++;
            items_to_produce++;
        }
        sem_post(&mutex);

        // Libera os slots preenchidos para os consumidores
        for (int i = 0; i < items_to_produce; i++) {
            sem_post(&full_slots);
        }
    }

    // Sinaliza o fim do trabalho para todas as threads consumidoras
    for (int i = 0; i < num_consumers; i++) {
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
            // Se for o sinal de parada, a thread deve terminar. O produtor já enviou um -1 para cada consumidor.
            break; // Sai do loop
        }
        
        sem_post(&empty_slots); // Sinaliza que um slot foi esvaziado
        
        // Verifica a primalidade e atualiza as estatísticas
        if (ehPrimo(number_to_check)) {
            my_data->primes_found++;
        }
    }

    // Atualiza as variáveis globais
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
    if (buffer == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o buffer.\n");
        return 2;
    }
    consumer_stats = (consumer_data*)malloc(sizeof(consumer_data) * num_consumers);
    if (consumer_stats == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para as estatísticas dos consumidores.\n");
        free(buffer);
        return 3;
    }

    // Inicializa os semáforos
    sem_init(&empty_slots, 0, M_BUFFER_SIZE); // Inicialmente, o buffer está vazio
    sem_init(&full_slots, 0, 0);               // Inicialmente, não há itens no buffer
    sem_init(&mutex, 0, 1);                    // Inicializa o mutex para 1

    // Cria as threads
    pthread_t producer_tid;
    pthread_t* consumer_tids = (pthread_t*)malloc(sizeof(pthread_t) * num_consumers);
    if (consumer_tids == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para os IDs das threads consumidoras.\n");
        free(buffer);
        free(consumer_stats);
        return 4;
    }

    // Cria a thread produtora
    if (pthread_create(&producer_tid, NULL, producer_thread, (void*)&num_consumers)) {
        fprintf(stderr, "Erro: Falha ao criar a thread produtora.\n");
        free(buffer);
        free(consumer_stats);
        free(consumer_tids);
        return 5;
    }

    // Cria as threads consumidoras
    for (int i = 0; i < num_consumers; i++) {
        consumer_stats[i].id = i;
        consumer_stats[i].primes_found = 0;
        if (pthread_create(&consumer_tids[i], NULL, consumer_thread, (void*)&consumer_stats[i])) {
            fprintf(stderr, "Erro: Falha ao criar a thread consumidora %d.\n", i);
            free(buffer);
            free(consumer_stats);
            free(consumer_tids);
            return 6;
        }
    }

    // Aguarda o término das threads
    pthread_join(producer_tid, NULL);
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumer_tids[i], NULL);
    }

    // --- Verificação Sequencial ---
    printf("\n--- Verificação ---\n");
    int sequential_prime_count = count_primes_sequentially(N_NUMBERS);

    // Exibe os resultados
    printf("Quantidade total de primos encontrados: %d\n", total_primes);
    printf("Contagem concorrente de primos: %d\n", total_primes);
    printf("Contagem sequencial de primos:  %d\n", sequential_prime_count);
    if (total_primes == sequential_prime_count) {
        printf("Resultado: As contagens são IGUAIS.\n");
    } else {
        printf("Resultado: As contagens são DIFERENTES.\n");
    }
    printf("\n--- Detalhes da Execução Concorrente ---\n");
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