/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Exemplo de uso de um pool de threads oferecido por Java */
/* -------------------------------------------------------------------*/

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicLong;

//classe runnable
class Worker implements Runnable {
	private final long steps;

	//construtor
	Worker(long numSteps) {
	this.steps = numSteps;
	}

	//método para execução
	public void run() {
		for (long i=1; i<this.steps; i++) {
			AnotherHelloPool.incrementaVariavelCompartilhada();
		}
	}
}

//classe do método main
public class AnotherHelloPool {
	private static final int NTHREADS = 10;
	private static final int WORKERS = 50;

	// Variável compartilhada que será incrementada pelas threads
	private static AtomicLong variavelCompartilhada = new AtomicLong(0);

	public static void main(String[] args) {
	//cria um pool de threads (NTHREADS)
		ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);

		//dispara a execução dos workers
		for (int i = 1; i < WORKERS; i++) {
			Runnable worker = new Worker(i);
			executor.execute(worker);
		}
		//termina a execução das threads no pool (não permite que o executor aceite novos objetos)
		executor.shutdown();
		//espera todas as threads terminarem
		while (!executor.isTerminated()) {}
		System.out.println("Terminou");
		System.out.println("Valor final da variável compartilhada: " + variavelCompartilhada.get());
	}

	/**
	* Incrementa a variável compartilhada de forma segura (thread-safe).
	*/
	public static void incrementaVariavelCompartilhada() {
		variavelCompartilhada.incrementAndGet();
	}
}
