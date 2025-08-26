#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int ac, char **av){
	
	if (ac < 3){
		printf("erro de poucos argumentos recebidos\n");
		return 1;
	}

	long N = atoi(av[1]);
	char *nome_arquivo = av[2];

	float *v1 = (float *)malloc(N * sizeof(float));
	float *v2 = (float *)malloc(N * sizeof(float));

	if (v1 == NULL || v2 == NULL) {
		printf("erro ao alocar memoria\n");
		return 2;
	}

	//seed para numeros aleatorios
	srand(time(NULL));
	for (long i = 0; i < N; i++) {
		v1[i] = (float)rand() / RAND_MAX;
		v2[i] = (float)rand() / RAND_MAX;
		
	}

	float prod_interno = 0.0;
	for (long i = 0; i < N; i++) {
		prod_interno += v1[i] * v2[i];
	}

	FILE *arquivo = fopen(nome_arquivo, "wb");
	if (arquivo == NULL) {
		printf("erro ao abrir arquivo para escrita\n");
		return 3;
	}

	fwrite(&N, sizeof(long), 1, arquivo);
	fwrite(v1, sizeof(float), N, arquivo);
	fwrite(v2, sizeof(float), N, arquivo);
	fwrite(&prod_interno, sizeof(float), 1, arquivo);

	fclose(arquivo);
	free(v1);
	free(v2);

	printf("Arquivo de teste gerado: %s\n", nome_arquivo);
	printf("Dimensao N: %ld\n", N);
	printf("Resultado pelo calculo sequencial: %f\n", prod_interno);
	

	return 0;
}
