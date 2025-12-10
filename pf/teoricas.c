 (a) Seção Crítica
     - Definição: Trecho de código que acessa um recurso compartilhado (ex: variável global).
     - Regra: Apenas uma thread pode executá-lo por vez para garantir a consistência dos dados.
     - Exemplo: `soma++` em uma variável global `soma`.

 (b) Corrida de Dados (Data Race)
     - Definição: Ocorre quando duas ou mais threads acessam a mesma memória sem sincronização,
       e pelo menos um dos acessos é de escrita.
     - Consequência: O resultado da operação se torna imprevisível e não determinístico.

 (c) Violação de Atomicidade
     - Definição: Ocorre quando uma operação que deveria ser indivisível (atômica) é
       interrompida pela execução de outra thread, gerando um estado inconsistente.
     - Exemplo: `soma++` não é atômico. Envolve 3 passos: (1) Ler valor, (2) Incrementar, (3) Escrever valor.
       Uma interrupção entre os passos pode fazer com que um incremento seja perdido.

 (d) Violação de Ordem (Violação de Condição Lógica)
     - Definição: Acontece quando operações de threads diferentes são executadas em uma sequência
       incorreta, quebrando a lógica necessária para o funcionamento do programa.
     - Exemplo: Uma thread B imprime "BYEBYE" *antes* da thread A imprimir "HELLO", quando a ordem
       correta seria o inverso.

 (e) Sincronização por Exclusão Mútua (Locks/Mutex)
     - Objetivo: Garantir que apenas uma thread acesse a seção crítica por vez.
     - Funcionamento:
       1. pthread_mutex_lock(): Antes de entrar na seção crítica. Se o lock estiver ocupado, a thread bloqueia.
       2. (Executa a seção crítica)
       3. pthread_mutex_unlock(): Libera o lock, permitindo que outra thread o adquira.

 (f) Sincronização por Condição (Variáveis de Condição)
     - Objetivo: Permitir que threads esperem (bloqueiem) até que uma condição lógica se torne verdadeira.
       Sempre usada em conjunto com um mutex.
     - Funções:
       - pthread_cond_wait(&cond, &mutex): Libera o mutex e bloqueia a thread na variável `cond`.
         Quando acordada, ela tenta readquirir o mutex antes de continuar.
       - pthread_cond_signal(&cond): Acorda *uma* thread que está esperando em `cond`.
       - pthread_cond_broadcast(&cond): Acorda *todas* as threads que estão esperando em `cond`.

 (g) Necessidade de Comunicação e Sincronização
     - Correção: Evitar erros de concorrência (corridas de dados, violações de atomicidade/ordem)
       para garantir resultados corretos e comportamento determinístico.
     - Coordenação: Permitir que threads cooperem e troquem informações para realizar uma tarefa complexa.
       Ex: Uma thread produtora sinaliza a uma consumidora que um dado está pronto.

 (h) Comunicação por Memória Compartilhada
     - Como funciona: Threads de um mesmo processo compartilham o mesmo espaço de memória (variáveis
       globais, dados no heap). Uma thread escreve em uma posição de memória e outra lê da mesma posição.
     - Vantagem: É a forma mais rápida de comunicação entre threads.
     - Desvantagem: É a principal fonte de problemas de concorrência. Todo acesso compartilhado
       que envolva escrita deve ser protegido por mecanismos de sincronização.

 (i) Multiprocesso
     - Definição: Cada fluxo lógico de execução é um processo escalonado e mantido pelo SO.
     - Criação: Processos podem ser disparados a priori ou criados como filhos de um processo principal.
     - Memória: O espaço de endereçamento é exclusivo de cada processo.
     - Comunicação: Requer mecanismos explícitos de interação interprocessos (IPC).
     - Escalonamento: Feito pelo SO, normalmente de forma preemptiva.

 (j) I/O Multiplexado
     - Definição: A aplicação escalona seus próprios fluxos de execução dentro de um único processo e espaço de endereçamento.
     - Modelo: A aplicação funciona como uma máquina de estados controlada pelo programa principal, baseada em eventos de I/O.

 (k) Multithreading
     - Definição: Fluxos lógicos independentes (threads) dentro de um mesmo processo.
     - Memória: Compartilham o mesmo espaço de endereçamento.
     - Escalonamento: Realizado pelo SO, geralmente de forma preemptiva.

 (l) Gerência Cooperativa com Co-rotinas
     - Definição: Linhas de execução distintas, cada uma com sua própria pilha, variáveis locais e ponteiro de instrução.
     - Compartilhamento: Compartilham variáveis globais, assim como as threads.
     - Diferença Principal: Co-rotinas são colaborativas e não executam simultaneamente. A troca de controle é explícita.
     - Execução: Apenas uma co-rotina executa por vez, e a suspensão é solicitada pela própria co-rotina.

 (m) Futuros ou Computação Assíncrona
     - Definição: Primitiva de alto nível que permite que uma computação seja executada em paralelo e seu resultado
       capturado posteriormente no programa.
     - Implementação: Geralmente utiliza um pool de threads ou de processos.