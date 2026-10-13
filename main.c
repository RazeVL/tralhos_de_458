/* ESTUDANTES: 198218 Leonardo Paillo da Silva | 193329 Rafael de Oliveira Fernandes */

#include <stdio.h>
#include <stdlib.h>

// Função auxiliar para trocar dois elementos
void swap(int *a, int *b) {
    int placeholder = *b;
    *b = *a;
    *a = placeholder;
}

// 1. INSERTION SORT (Usado para subarrays pequenos)
void insertionSort(int arr[], int left, int right) {
    if (left > right) return;
    // Coloca o elemento na posição correta em relação aos anteriores (já ordenados)
    for (int i = left; i > 0 && arr[i-1] > arr[i]; i--) swap(&arr[i-1], &arr[i]);
    insertionSort(arr, left+1, right);
}

// 2. HEAPSORT E FUNÇÕES DE HEAP (Usado quando a recursão fica muito profunda)
void heapify(int arr[], int n, int i, int offset) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[offset + l] > arr[offset + largest])
        largest = l;
    if (r < n && arr[offset + r] > arr[offset + largest])
        largest = r;

    if (largest != i) {
        swap(&arr[offset + i], &arr[offset + largest]);
        heapify(arr, n, largest, offset);
    }
}

void heapSort(int arr[], int left, int right) {
    int n = right - left + 1;
    // Constrói o max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, left);
    // Extrai os elementos do heap um por um
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[left], &arr[left + i]);
        heapify(arr, i, 0, left);
    }
}

// 3. QUICKSORT PARTITION (Motor principal usando Mediana de Três)
void medianOfThreeSwap(int arr[], int a, int b, int c) {
    if ((arr[a] <= arr[b] && arr[b] <= arr[c]) || (arr[c] <= arr[b] && arr[b] <= arr[a])) {
        swap(&arr[a], &arr[b]);
    } else if ((arr[a] <= arr[c] && arr[c] <= arr[b]) || (arr[b] <= arr[c] && arr[c] <= arr[a])) {
        swap(&arr[a], &arr[c]);
    }
}

int partition(int arr[], int left, int right) {
    int pivo = arr[left];
    int j = left+1;
    for (int i = left+1; i <= right; i++) {
        if (arr[i] < pivo) {
            swap(&arr[j], &arr[i]);
            j++;
        }
    }   
    swap(&arr[left], &arr[j-1]);
    return j-1;
}

// FUNÇÃO RECURSIVA DO INTROSORT
void introSortUtil(int arr[], int left, int right, int depthLimit) {
    int size = right - left + 1;

    // Condição 1: Subarray pequeno -> Insertion Sort
    if (size < 16) {
        insertionSort(arr, left, right);
        return;
    }

    // Condição 2: Profundidade limite atingida -> HeapSort
    if (depthLimit == 0) {
        heapSort(arr, left, right);
        return;
    }

    // Condição 3: Caso normal -> Quicksort
    //introSortUtil(arr, left, ▒▒▒▒▒▒▒- 1); // vvvvvv
    //introSort?? right, depthLimit - 1); // deixei essas 2 linhas comentadas pq já tá parcialmente escrito
}

// FUNÇÃO PRINCIPAL DE CHAMADA
void introSort(int arr[], int n) {
    if (n < 2) return;

    // Calcula o limite de profundidade como 2 * log2(n)
    int depthLimit = 0;
    int temp = n;
    while (temp > 1) {
        depthLimit++;
        temp >>= 1;
    }
    depthLimit *= 2;

    introSortUtil(arr, 0, n - 1, depthLimit);
}

int* gerador(int seed, long long n) {
    int *arr = (int *)malloc(n * sizeof(int));

    // Inicializa a semente de aleatoriedade usando o relógio do sistema
    srand(seed);

    for (long long i = 0; i < n; i++) {
        // Combina duas chamadas de rand() para gerar números de até ~1 bilhão
        // Isso evita que números se repitam excessivamente em sistemas onde RAND_MAX é pequeno
        int numero_aleatorio = (rand() << 15) ^ rand();
        arr[i] = numero_aleatorio;
    }

    return arr;
}

// TESTE DO ALGORITMO
int main() {
    int seed;
    scanf("%d", &seed);
    long long n = 10000000;
    int *arr = gerador(seed, n);

    introSort(arr, n);

    printf("Soma de todos os elementos do array ordenado via Introsort, mod 1000000007: \n");
    int result = 0;
    for (int i = 0; i < n; i++) {
        result = (result + (arr[i] % 1000000007)) % 1000000007;  // Apenas para evitar overflow em casos de teste muito grandes
    }

    printf("%d ", result);
    printf("\n");

    free(arr);
    return 0;
}