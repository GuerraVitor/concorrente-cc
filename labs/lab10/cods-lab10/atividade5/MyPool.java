/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Criando um pool de threads em Java */

import java.util.LinkedList;

//-------------------------------------------------------------------------------
//!!! Documentar essa classe !!!
class FilaTarefas {
	//numero de threads na pool
	private final int nThreads;
	//array das threads
	private final MyPoolThreads[] threads;
	//fila de tarefas a serem executadas
	private final LinkedList<Runnable> queue;
	//flag para checar desligamento do pool
	private boolean shutdown;

	//construtor das threadas
	public FilaTarefas(int nThreads) {
		//variaveis da fila
		this.shutdown = false;
		this.nThreads = nThreads;
		//cria fila de tarefas
		queue = new LinkedList<Runnable>();
		//cria array de threads
		threads = new MyPoolThreads[nThreads];
		// cria e inicia as threads
		for (int i=0; i<nThreads; i++) {
			threads[i] = new MyPoolThreads();
			threads[i].start();
		}
	}

	//coloca tarefa da fila
	public void execute(Runnable r) {
		//garante que 1 thread modifique a fila por vez
		synchronized(queue) {
			//verifica se o pool ainda aceita tarefas
			if (this.shutdown) return;
			//adciona tarefa no ultimo lugar da fila
			queue.addLast(r);
			//avisa uma thread que estava em espera
			queue.notify();
		}
	}

	//desliga a pool e encerra threads
	public void shutdown() {
		//garante atomicidade da fila
		synchronized(queue) {
			//sinaliza com a flag que as threads devem para
			this.shutdown=true;
			//acorda todas as threads em espera
			queue.notifyAll();
		}
		//espera encerramento de todas as threads
		for (int i=0; i<nThreads; i++) {
			try { threads[i].join(); } catch (InterruptedException e) { return; }
		}
	}

	private class MyPoolThreads extends Thread {
		public void run() {
			Runnable r;
			while (true) {
				synchronized(queue) {
					while (queue.isEmpty() && (!shutdown)) {
						try { queue.wait(); }
						catch (InterruptedException ignored){}
					}
					if (queue.isEmpty())
						return;
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
	//...completar implementacao, recebe um numero inteiro positivo e imprime se esse numero eh primo ou nao
	public void run() {}
}

//Classe da aplicação (método main)
class MyPool {
	private static final int NTHREADS = 10;

	public static void main (String[] args) {
		//--PASSO 2: cria o pool de threads
		FilaTarefas pool = new FilaTarefas(NTHREADS);

		//--PASSO 3: dispara a execução dos objetos runnable usando o pool de threads
		for (int i = 0; i < 25; i++) {
		final String m = "Hello da tarefa " + i;
		Runnable hello = new Hello(m);
		pool.execute(hello);
		//Runnable primo = new Primo(i);
		//pool.execute(primo);
		}

		//--PASSO 4: esperar pelo termino das threads
		pool.shutdown();
		System.out.println("Terminou");
	}
}
