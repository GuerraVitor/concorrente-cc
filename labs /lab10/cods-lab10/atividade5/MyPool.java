/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Criando um pool de threads em Java */

import java.util.LinkedList;

//-------------------------------------------------------------------------------
/**
 * A classe FilaTarefas implementa um pool de threads simples.
 * Ela gerencia uma fila de tarefas (Runnable) e um conjunto de threads trabalhadoras
 * para processar essas tarefas de forma concorrente.
 */
class FilaTarefas {
    private final int nThreads;
    private final MyPoolThreads[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;

    /**
     * Construtor da FilaTarefas.
     * @param nThreads O número de threads trabalhadoras no pool.
     */
    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i=0; i<nThreads; i++) {
            // Adiciona o id para facilitar a depuração
            threads[i] = new MyPoolThreads(i);
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
     * A classe interna MyPoolThreads representa uma thread trabalhadora do pool.
     * Cada thread executa tarefas da fila em um loop contínuo até que o pool seja encerrado.
     */
    private class MyPoolThreads extends Thread {
       private int id;

       /**
        * Construtor da thread trabalhadora.
        * @param id Identificador da thread.
        */
       public MyPoolThreads(int id) {
           this.id = id;
       }
       public void run() {
         Runnable r;
         while (true) {
           synchronized(queue) {
             while (queue.isEmpty() && (!shutdown)) {
               try { queue.wait(); }
               catch (InterruptedException ignored){}
             }
             // Se a fila estiver vazia e o shutdown foi chamado, a thread termina.
             if (queue.isEmpty() && shutdown) {
                 return;
             }
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
       for (int i = 3; i * i <= n; i += 2) {
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
        final String m = "Hello da tarefa " + i;
        Runnable hello = new Hello(m);
        pool.execute(hello);
        Runnable primo = new Primo(i);
        pool.execute(primo);
      }

      //--PASSO 4: esperar pelo termino das threads
      pool.shutdown();
      System.out.println("Terminou");
   }
}
