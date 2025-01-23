#include <stdio.h>     // Biblioteca para manipulação de entradas e saídas padrão (printf, scanf, etc)
#include <stdlib.h>    // Biblioteca para funções de alocação dinâmica de memória (malloc, free)

// Definindo a estrutura FilaDePrioridade para representar a fila de prioridade
typedef struct {
    int *heap;      // Ponteiro para o vetor (array) que armazenará os elementos do heap
    int size;       // A quantidade atual de elementos na fila
    int capacity;   // Capacidade máxima da fila
} FilaDePrioridade;

// Função para inicializar a fila de prioridade
FilaDePrioridade* inicializa_fila(int capacidade) {
    // Aloca memória para a fila de prioridade
    FilaDePrioridade* fila = (FilaDePrioridade*) malloc(sizeof(FilaDePrioridade));
    
    // Aloca memória para o vetor que representará o heap
    fila->heap = (int*) malloc((capacidade + 1) * sizeof(int));  // +1 para que o índice 0 seja ignorado
    fila->size = 0;  // Inicializa o tamanho da fila como 0
    fila->capacity = capacidade;  // Define a capacidade máxima da fila
    return fila;  // Retorna o ponteiro para a fila inicializada
}

// Função para trocar dois elementos de lugar em memória
void troca(int* a, int* b) {
    int temp = *a;  // Armazena o valor de *a temporariamente
    *a = *b;        // Coloca o valor de *b em *a
    *b = temp;      // Coloca o valor temporário em *b
}

// Função de "subida" (heapify-up) para manter a propriedade do max-heap
void heapify_up(FilaDePrioridade* fila, int i) {
    // Enquanto o índice i não for a raiz e o valor do nó pai for menor que o valor do nó atual
    while (i > 1 && fila->heap[i / 2] < fila->heap[i]) {
        // Troca o nó atual com o nó pai
        troca(&fila->heap[i / 2], &fila->heap[i]);
        i = i / 2;  // Atualiza i para o índice do nó pai
    }
}

// Função de "descida" (heapify-down) para manter a propriedade do max-heap
void heapify_down(FilaDePrioridade* fila, int i) {
    int maior = i;  // Inicializa o maior com o índice do nó atual
    int esquerda = 2 * i;  // Índice do filho à esquerda
    int direita = 2 * i + 1;  // Índice do filho à direita

    // Verifica se o filho à esquerda existe e é maior que o nó atual
    if (esquerda <= fila->size && fila->heap[esquerda] > fila->heap[maior])
        maior = esquerda;  // Atualiza maior se o filho à esquerda for maior

    // Verifica se o filho à direita existe e é maior que o maior encontrado até agora
    if (direita <= fila->size && fila->heap[direita] > fila->heap[maior])
        maior = direita;  // Atualiza maior se o filho à direita for maior

    // Se o maior não for o nó atual, troca o nó com o maior e faz a descida recursivamente
    if (maior != i) {
        troca(&fila->heap[i], &fila->heap[maior]);
        heapify_down(fila, maior);  // Recursivamente corrige a subárvore
    }
}

// Função para inserir um novo elemento na fila de prioridade
void inserir(FilaDePrioridade* fila, int chave) {
    // Verifica se a fila está cheia
    if (fila->size >= fila->capacity) {
        printf("Fila cheia\n");
        return;
    }
    fila->size++;  // Aumenta o tamanho da fila
    fila->heap[fila->size] = chave;  // Coloca o novo elemento no final da fila
    heapify_up(fila, fila->size);  // Ajusta o heap para manter a propriedade do max-heap
}

// Função para remover o elemento de maior prioridade (a raiz do heap)
int remover_max(FilaDePrioridade* fila) {
    // Verifica se a fila está vazia
    if (fila->size == 0) {
        printf("Fila vazia\n");
        return -1;  // Retorna valor de erro
    }

    int max = fila->heap[1];  // O elemento de maior prioridade (raiz do heap)
    fila->heap[1] = fila->heap[fila->size];  // Coloca o último elemento na raiz
    fila->size--;  // Diminui o tamanho da fila
    heapify_down(fila, 1);  // Ajusta o heap para manter a propriedade do max-heap
    return max;  // Retorna o elemento de maior prioridade removido
}

// Função para construir o heap a partir de um vetor de inteiros
void construir_heap(FilaDePrioridade* fila, int* vetor, int n) {
    fila->size = n;  // Define o tamanho da fila
    for (int i = 1; i <= n; i++) {
        fila->heap[i] = vetor[i-1];  // Copia os valores do vetor para o heap
    }
    // Constrói o heap a partir dos elementos não-folha (do meio para a raiz)
    for (int i = n / 2; i >= 1; i--) {
        heapify_down(fila, i);  // Ajusta cada subárvore
    }
}

// Função para ordenar um vetor usando heapsort
void heapsort(int* vetor, int n) {
    FilaDePrioridade fila;  // Cria a fila de prioridade
    fila.heap = vetor;  // O vetor será o heap
    fila.size = n;  // Define o tamanho inicial
    fila.capacity = n;  // Define a capacidade do heap

    // Constrói o heap a partir do vetor
    for (int i = n / 2; i >= 1; i--) {
        heapify_down(&fila, i);  // Ajusta o heap
    }

    // Realiza a ordenação trocando a raiz (máximo) com o último elemento
    for (int i = n; i > 1; i--) {
        troca(&fila.heap[1], &fila.heap[i]);  // Troca o primeiro e o último elemento
        fila.size--;  // Diminui o tamanho do heap
        heapify_down(&fila, 1);  // Ajusta o heap novamente
    }
}

// Função para verificar se um vetor é um heap de máximo
int verifica_heap(int* heap, int n) {
    // Verifica se a condição de max-heap é violada em algum nó
    for (int i = 1; i <= n / 2; i++) {
        int esquerda = 2 * i;  // Índice do filho à esquerda
        int direita = 2 * i + 1;  // Índice do filho à direita
        // Verifica se o filho à esquerda é maior que o nó atual
        if (esquerda <= n && heap[i] < heap[esquerda]) return 0;
        // Verifica se o filho à direita é maior que o nó atual
        if (direita <= n && heap[i] < heap[direita]) return 0;
    }
    return 1;  // Retorna 1 se a condição de max-heap não for violada
}

// Função recursiva para encontrar a chave mínima (valor mais baixo) no heap
int chave_minima(int* heap, int n) {
    if (n == 1) return heap[1];  // Se houver apenas um elemento, ele é a chave mínima

    int min = heap[n];  // Inicializa o valor mínimo com o último elemento
    // Percorre o vetor para verificar se há um valor menor
    for (int i = n - 1; i >= 1; i--) {
        if (heap[i] < min) min = heap[i];  // Atualiza o valor mínimo se encontrar um valor menor
    }
    return min;  // Retorna a chave mínima
}

// Função para remover um elemento arbitrário da posição i no heap
void remove_heap(FilaDePrioridade* fila, int i) {
    // Verifica se o índice fornecido é válido
    if (i < 1 || i > fila->size) {
        printf("Índice inválido\n");
        return;
    }

    fila->heap[i] = fila->heap[fila->size];  // Substitui o elemento a ser removido pelo último elemento
    fila->size--;  // Diminui o tamanho da fila
    heapify_down(fila, i);  // Ajusta o heap para manter a propriedade de max-heap
}

int main() {
    // Inicializa a fila de prioridade com capacidade para 10 elementos
    FilaDePrioridade* fila = inicializa_fila(10);

    // Insere elementos na fila
    inserir(fila, 15);
    inserir(fila, 10);
    inserir(fila, 20);
    inserir(fila, 30);
    inserir(fila, 5);

    // Remove e imprime o elemento de maior prioridade
    printf("Removido: %d\n", remover_max(fila));  // Deve remover 30
    printf("Removido: %d\n", remover_max(fila));  // Deve remover 20

    // Constrói um heap a partir de um vetor de inteiros
    int vetor[] = { 5, 10, 15, 20, 30 };
    construir_heap(fila, vetor, 5);

    // Exibe o conteúdo do heap após a construção
    printf("Heap após construção: ");
    for (int i = 1; i <= fila->size; i++) {
        printf("%d ", fila->heap[i]);  // Imprime cada elemento do heap
    }
    printf("\n");

    // Verifica se o vetor representa um heap de máximo e imprime o resultado
    printf("Verifica se é um heap de máximo: %d\n", verifica_heap(fila->heap, fila->size));

    // Encontra e imprime a chave mínima no heap
    printf("Chave mínima: %d\n", chave_minima(fila->heap, fila->size));

    return 0;  // Fim do programa
}
