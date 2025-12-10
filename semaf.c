#include <semaphore.h>
#include <pthread.h>

#define N 10 // Tamanho do buffer (exemplo)

// --- Variaveis Globais ---
int Buffer[N];
int in = 0; // Índice de inserção para os produtores

sem_t mutex;      // Exclusão mútua para produtores manipularem 'in'
sem_t slotCheio;  // Sinaliza ao consumidor que o buffer está CHEIO (Batch pronto)
sem_t slotVazio;  // Conta quantos espaços livres existem para os produtores

// Inicialização (deve ser feita na main)
// sem_init(&mutex, 0, 1);
// sem_init(&slotCheio, 0, 0); // Começa bloqueado (0)
// sem_init(&slotVazio, 0, N); // Começa com N vagas

void Insere (int item) {
    sem_wait(&slotVazio);       // 1. O produtor precisa de uma vaga vazia
    sem_wait(&mutex);           // 2. Acesso exclusivo para escrever no buffer e mexer no 'in'
    
    Buffer[in] = item;
    in++;

    if (in == N) {              // 3. Verifica se completou o lote (buffer cheio)     
        in = 0;                 // Reinicia o índice para o próximo ciclo
        sem_post(&slotCheio);   // acordamos o consumidor quando encheu tudo
    }

    sem_post(&mutex);           // 4. Libera o mutex para outros produtores (se houverem) continuarem enchendo
}

void Retira (int itens[]) {
    sem_wait(&slotCheio); // 1. O consumidor espera o sinal de que o buffer está COMPLETO

    for(int i = 0; i < N; i++) { // 2. Copia o lote inteiro (Consome tudo)
        itens[i] = Buffer[i];
    }
    
    for(int i = 0; i < N; i++) {
        sem_post(&slotVazio);
    }
}