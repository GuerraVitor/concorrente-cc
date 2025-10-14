#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "prime_counter.h"

int count_primes_sequentially(int n) {
    if (n < 2) {
        return 0;
    }

    // Aloca memória para o crivo.
    char* is_prime = (char*)malloc(n + 1);
    if (is_prime == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o crivo sequencial.\n");
        return -1; // Indica erro
    }

    // Inicializa todos os números como primos (1 = true)
    memset(is_prime, 1, n + 1);

    is_prime[0] = 0; // 0 não é primo
    is_prime[1] = 0; // 1 não é primo

    // Implementação do Crivo de Eratóstenes
    for (int p = 2; p * p <= n; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p)
                is_prime[i] = 0;
        }
    }

    int count = 0;
    for (int p = 2; p <= n; p++)
        if (is_prime[p])
            count++;

    free(is_prime);
    return count;
}
