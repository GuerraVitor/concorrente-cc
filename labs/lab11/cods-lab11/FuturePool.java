/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Exemplo de uso de futures */
/* -------------------------------------------------------------------*/

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import java.util.ArrayList;
import java.util.List;


//classe runnable
class PrimoCallable implements Callable<Integer> {
	private final long numero;

	public PrimoCallable(long numero) {
		this.numero = numero;
	}

	private boolean ehPrimo(long n) {
		if (n <= 1) return false;
		if (n == 2) return true;
		if (n % 2 == 0) return false;
		for (long i = 3; i * i <= n; i += 2) {
			if (n % i == 0) return false;
		}
		return true;
	}

	@Override
	public Integer call() {
		return ehPrimo(numero) ? 1 : 0;
	}
}

//classe do método main
public class FuturePool {
	private static final int N = 100_000; // Intervalo para verificar primos: 1 a N
	private static final int NTHREADS = 10;

	public static void main(String[] args) {
		//cria um pool de threads (NTHREADS)
		ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
		//cria uma lista para armazenar referencias de chamadas assincronas (Futures)
		List<Future<Integer>> list = new ArrayList<>();

		// Submete uma tarefa para cada número no intervalo de 1 a N
		for (int i = 1; i <= N; i++) {
			Callable<Integer> worker = new PrimoCallable(i);
			Future<Integer> future = executor.submit(worker);
			list.add(future);
		}

		// Recupera os resultados e faz o somatório final
		int totalPrimos = 0;
		for (Future<Integer> future : list) {
			try {
				totalPrimos += future.get(); // get() bloqueia até a computação terminar
			} catch (InterruptedException | ExecutionException e) {
				e.printStackTrace();
			}
		}

		System.out.println("Quantidade de números primos entre 1 e " + N + ": " + totalPrimos);
		executor.shutdown();
	}
}
