#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

typedef struct {
	long inicio;
	long fim;
	float *v1;
	float *v2;
	float resultado;
} Job_th;

void *calcula(void *arg) {

	Job_th *job = (Job_th *)arg;
	job->resultado = 0.0;

	for (long i = job->inicio; i < job->fim; i++) {
		job->resultado += job->v1[i] * job->v2[i];
	}

	pthread_exit(NULL);
}

int main(int ac, char **av) {
	
	if (ac < 3) {
		printf("numero de argumentos insuficientes\n");
		return 1;
	}

	int num_threads = atoi(av[1]);
	char *nome_arquivo = av[2];

	long N;
	float *v1, *v2;
	float resultado_seq;

	FILE *arquivo = fopen(nome_arquivo, "rb");
	if (arquivo == NULL) {
		printf("Erro ao abrir arquivo para leitura\n");
		return 2;
	}
	
	fread(&N, sizeof(long), 1, arquivo);
	
	v1 = (float *)malloc(N * sizeof(float));
	v2 = (float *)malloc(N * sizeof(float));
	if (v1 == NULL || v2 == NULL) {
		printf("Erro ao alocar memoria para os vetores\n");
		return 3;
	}

	fread(v1, sizeof(float), N, arquivo);
	fread(v2, sizeof(float), N, arquivo);
	fread(&resultado_seq, sizeof(float), 1, arquivo);
	
	fclose(arquivo);

	pthread_t threads[num_threads];
	Job_th jobs[num_threads];

	struct timeval t_inicio, t_fim;
	gettimeofday(&t_inicio, NULL);

	long tam_bloco = N / num_threads;

	for (int i = 0; i < num_threads; i++) {
		jobs[i].v1 = v1;
		jobs[i].v2 = v2;
		jobs[i].inicio = i * tam_bloco;
		jobs[i].fim = (i == num_threads - 1) ? N : (i + 1) * tam_bloco;

		pthread_create(&threads[i], NULL, calcula, (void *)&jobs[i]);
	}

	//espera threads terminare e soma os resultados
	float resultado_conc = 0.0;
	for (int i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
		resultado_conc += jobs[i].resultado;
	}

	gettimeofday(&t_fim, NULL);

	//calcula o tempo
	double tempo_exec = (double)(t_fim.tv_sec - t_inicio.tv_sec) + (double)(t_fim.tv_sec - t_inicio.tv_sec) / 1000000.0;

	float erro = fabsf((resultado_seq - resultado_conc) / resultado_seq);

	printf("N: %ld, Threads: %d\n", N, num_threads);
	printf("Resultado Sequencial: %f\n", resultado_seq);
	printf("Resultado Concorrente: %f\n", resultado_conc);
	printf("Erro relativo: %f\n", erro);
	printf("Tempo de execucao: %f s\n", tempo_exec);

	free(v1);
	free(v2);

	return 0;
}
