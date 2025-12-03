/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Criando um pool de threads em Java */

import java.util.LinkedList;

//-------------------------------------------------------------------------------
	/**
	* A classe FilaTarefas implementa um pool de threads customizado.
	* Ela gerencia um conjunto de threads trabalhadoras e uma fila de tarefas (Runnables).
	* As tarefas são executadas pelas threads disponíveis no pool.
	*/
class FilaTarefas {
	/** Número de threads no pool. */
	private final int nThreads;
	/** Array de threads trabalhadoras. */
	private final MyPoolThreads[] threads;
	/** Fila para armazenar as tarefas Runnable a serem executadas. */
	private final LinkedList<Runnable> queue;
	/** Flag para indicar se o pool de threads foi encerrado. */
	private boolean shutdown;

	/**
	* Construtor da FilaTarefas.
	* @param nThreads O número de threads a serem criadas no pool.
	*/
	public FilaTarefas(int nThreads) {
		this.shutdown = false;
		this.nThreads = nThreads;
		queue = new LinkedList<Runnable>();
		threads = new MyPoolThreads[nThreads];
		for (int i=0; i<nThreads; i++) {
			threads[i] = new MyPoolThreads("PoolThread-" + (i+1));
			threads[i].start();
		}
	}

	public void execute(Runnable r) {
		synchronized(queue) {
			if (this.shutdown) return;
			queue.addLast(r);
			queue.notify();
		}
	}

	/**
	* Sinaliza para o pool de threads encerrar. Novas tarefas não são mais aceitas.
	* As threads existentes terminam após completarem suas tarefas atuais e a fila de tarefas esvaziar.
	*/
	public void shutdown() {
		synchronized(queue) {
			this.shutdown=true;
			queue.notifyAll();
		}
		for (int i=0; i<nThreads; i++) {
			try { threads[i].join(); } catch (InterruptedException e) { return; }
		}
	}

	/**
	* Classe interna que representa uma thread trabalhadora do pool.
	* Cada thread executa tarefas da fila em um loop contínuo até que o pool seja encerrado.
	*/
	private class MyPoolThreads extends Thread {

		public MyPoolThreads(String name) {
			super(name);
		}

		public void run() {
			Runnable r;
			while (true) {
				synchronized(queue) {
					while (queue.isEmpty() && (!shutdown)) {
						try { queue.wait(); }
						catch (InterruptedException ignored){}
					}
					if (queue.isEmpty() && shutdown) return;
					r = (Runnable) queue.removeFirst();
				}
				try { r.run(); }
				catch (RuntimeException e) {}
			}
		}
	}
}
//-------------------------------------------------------------------------------

//--PASSO 1: cria uma classe que implementa a interface Runnable
class Hello implements Runnable {
	String msg;
	public Hello(String m) { msg = m; }

	//--metodo executado pela thread
	public void run() {
		System.out.println(msg);
	}
}

class Primo implements Runnable {
	private long numero;

	public Primo(long n) {
		this.numero = n;
	}

	// Função para determinar se um número é primo
	private boolean ehPrimo(long n) {
		if (n <= 1) return false;
		if (n == 2) return true;
		if (n % 2 == 0) return false;
		for (int i = 3; i < Math.sqrt(n) + 1; i += 2) {
			if (n % i == 0) return false;
		}
		return true;
	}

	public void run() {
		if (ehPrimo(this.numero)) {
			System.out.println("O número " + this.numero + " é primo.");
		} else {
			System.out.println("O número " + this.numero + " não é primo.");
		}
	}
}

//Classe da aplicação (método main)
class MyPool {
	private static final int NTHREADS = 10;

	public static void main (String[] args) {
		//--PASSO 2: cria o pool de threads
		FilaTarefas pool = new FilaTarefas(NTHREADS);

		//--PASSO 3: dispara a execução dos objetos runnable usando o pool de threads
		for (int i = 0; i < 25; i++) {
		//final String m = "Hello da tarefa " + i;
		//Runnable hello = new Hello(m);
		//pool.execute(hello);
			Runnable primo = new Primo(i);
			pool.execute(primo);
		}

		//--PASSO 4: esperar pelo termino das threads
		pool.shutdown();
		System.out.println("Terminou");
	}
}
