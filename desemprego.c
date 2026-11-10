/* ESTUDANTES: 198218 Leonardo Paillo da Silva | 193329 Rafael de Oliveira Fernandes */

#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

// Lista ligada
typedef struct listyay {
    short int value;
    struct listyay* next;
} list;

// Transforma lista em long long pra comparar
ll listToLL(list* l){
    ll value = 0;
    for (;l; l = l->next) value = value * 10 + l->value;
    return value;
}

// Retorna o tamanho de uma lista
int lenList(list* l){ int c = 0; for (;l; l = l->next) c++; return c; }

// Imprime os valores de uma lista ligada
void printList(list* l){ for (;l; l = l->next) printf("%d", l->value); }

// Coloca um número no array no formato da lista
list* addNumArr(int value){
    list* tmp = NULL;
    if (value == 0) {
        tmp = malloc(sizeof(list));
        tmp->value = 0;
        tmp->next = NULL;
    }
    while(value>0){
        list* tung = malloc(sizeof(list));
        tung->value = value%10;
        tung->next = tmp;
        tmp = tung;
        value/=10;
    }
    return tmp;
}

// Adiciona cartas do input à lista
void getCartasValue(list*** array, int* cartas){
    int tmp = 0;
    for(int i = 0; i < *cartas; i++){
        scanf("%d",&tmp);
        (*array)[i] = addNumArr(tmp);
    }
}

// Recebe o número de cartas (input: linha 1) e aloca elas (input: linha 2)
void innit(list*** array, int* cartas){
    scanf("%d", cartas);
    *array = malloc(*cartas * sizeof(list*));
    getCartasValue(array, cartas);
    return;
}

// Imprime as cartas na ordem correta
void printCarta(list*** array, int* cartas){
    for(int i = 0; i< *cartas; i++){
        printList((*array)[i]);
    }
    printf("\n");
}

// Retorna 0 se a tem preferencia sobre b, 1 caso contrario
int compare(list* a, list* b){
    ll aLL = listToLL(a), bLL = listToLL(b);
    int tamA = lenList(a), tamB = lenList(b);
    ll potenciaA = 1, potenciaB = 1;
    for (int i = 0; i < tamB; i++) potenciaA *= 10;
    for (int i = 0; i < tamA; i++) potenciaB *= 10;
    ll acomp = aLL * potenciaA + bLL, bcomp = bLL * potenciaB + aLL;
    if (acomp > bcomp) return 1;
    if (bcomp > acomp) return -1;
    return 0;
}

// Compara usando o QSort nativo
int cmpQ(const void* pa, const void* pb){
    list* a = *(list* const*)pa;
    list* b = *(list* const*)pb;
    return -1 * compare(a, b);
}

// Função principal do programa
int main(){
    int nCartas;
    list** arrayCartas = NULL;
    innit(&arrayCartas, &nCartas);
    qsort(arrayCartas, nCartas, sizeof(list*), cmpQ);
    printCarta(&arrayCartas, &nCartas);
    return 0;
}
