/*
  Simulação da fila de peças "futuras" do Tetris Stack.
  - Fila circular com capacidade fixa (MAX_SIZE)
  - Peças geradas automaticamente por gerarPeca()
  - Ações: jogar peça (dequeue), inserir peça (enqueue), sair
  - Exibição do estado da fila após cada ação
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_SIZE 5  // capacidade da fila (fixa)

typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador único
} Peca;

typedef struct {
    Peca itens[MAX_SIZE];
    int frente;  // índice do elemento da frente (onde dequeue remove)
    int tras;    // índice da próxima posição livre para inserir (enqueue)
    int tamanho; // número atual de elementos na fila
} Fila;

// Protótipos
void inicializarFila(Fila *f);
int estaVazia(Fila *f);
int estaCheia(Fila *f);
int enqueue(Fila *f, Peca p);
int dequeue(Fila *f, Peca *out);
Peca gerarPeca();
void exibirFila(Fila *f);
void mostrarMenu();

int main() {
    Fila fila;
    inicializarFila(&fila);

    // Inicializa gerador de números aleatórios para gerar tipos aleatórios de peça
    srand((unsigned int) time(NULL));

    // Pré-preencher a fila com MAX_SIZE peças
    for (int i = 0; i < MAX_SIZE; ++i) {
        Peca p = gerarPeca();
        enqueue(&fila, p); // sabemos que há espaço porque fila está sendo preenchida inicialmente
    }

    int opcao;
    do {
        // Exibe estado da fila e menu
        printf("\n--------------------------------------------\n\n");
        printf("Fila de pecas: ");
        exibirFila(&fila);
        printf("\n--------------------------------------------\n\n");
        

        mostrarMenu();
        printf("\nEscolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            // entrada inválida: limpa stdin e repete
            int c;
            while ((c = getchar()) != EOF && c != '\n');
            printf("Entrada invalida. Tente novamente.\n");
            opcao = -1;
            continue;
        }

        if (opcao == 1) {
            // Jogar peça (dequeue)
            Peca removida;
            if (dequeue(&fila, &removida)) {
                printf("\nPeca jogada: [%c %d]\n", removida.nome, removida.id);
            } else {
                printf("\nFila vazia. Nenhuma peca para jogar.\n");
            }
        } else if (opcao == 2) {
            // Inserir nova peça (enqueue) se houver espaço
            if (estaCheia(&fila)) {
                printf("\nFila cheia. Nao e possivel inserir nova peça.\n");
            } else {
                Peca nova = gerarPeca();
                enqueue(&fila, nova);
                printf("\nPeca inserida: [%c %d]\n", nova.nome, nova.id);
            }
        } else if (opcao == 0) {
            printf("\nSaindo...\n");
        } else {
            printf("\nOpção invalida. Use 0, 1 ou 2.\n");
        }

        // limpa buffer de entrada restante (se houver)
        int ch;
        while ((ch = getchar()) != EOF && ch != '\n');

    } while (opcao != 0);

    return 0;
}

/* Inicializa os índices da fila */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
}

/* Verifica se fila está vazia */
int estaVazia(Fila *f) {
    return f->tamanho == 0;
}

/* Verifica se fila está cheia */
int estaCheia(Fila *f) {
    return f->tamanho == MAX_SIZE;
}

/* Enqueue: insere no fim; retorna 1 se sucesso, 0 se falha (fila cheia) */
int enqueue(Fila *f, Peca p) {
    if (estaCheia(f)) return 0;
    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % MAX_SIZE;
    f->tamanho++;
    return 1;
}

/* Dequeue: remove da frente; retorna 1 se sucesso (preenche out), 0 se vazio */
int dequeue(Fila *f, Peca *out) {
    if (estaVazia(f)) return 0;
    if (out != NULL) {
        *out = f->itens[f->frente];
    }
    f->frente = (f->frente + 1) % MAX_SIZE;
    f->tamanho--;
    return 1;
}

/* Gera uma peça com tipo aleatório entre 'I','O','T','L' e id incremental único.
   id é estático para manter contagem entre chamadas. */
Peca gerarPeca(void) {
    static int proximoId = 0;
    Peca p;
    // conjunto de tipos possíveis
    const char tipos[] = {'I', 'O', 'T', 'L'};
    int idx = rand() % (sizeof(tipos) / sizeof(tipos[0]));
    p.nome = tipos[idx];
    p.id = proximoId++;
    return p;
}

/* Exibe de forma legível a fila atual: [NOME id] [NOME id] ... na ordem da frente para o tras.
   Se estiver vazia, mostra mensagem apropriada. */
void exibirFila(Fila *f) {
    if (estaVazia(f)) {
        printf("[fila vazia]\n");
        return;
    }

    // Vamos iterar a partir de frente, percorrendo tamanho elementos
    int idx = f->frente;
    for (int i = 0; i < f->tamanho; ++i) {
        Peca p = f->itens[idx];
        printf("[%c %d] ", p.nome, p.id);
        idx = (idx + 1) % MAX_SIZE;
    }
    printf("\n");
}

/* Mostra o menu de opções ao usuário */
void mostrarMenu() {
    
    printf("\n1 - Jogar peca (dequeue)\n");
    printf("2 - Inserir nova peca (enqueue)\n");
    printf("0 - Sair\n");
    
}