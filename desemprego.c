/* ESTUDANTES: 198218 Leonardo Paillo da Silva | 193329 Rafael de Oliveira Fernandes */

#include <stdio.h>
#include <stdlib.h>

// Lista ligada
typedef struct listyay {
    short int value;
    struct listyay* next;
} list;

// Troca dois elementos
void swap(list* primeiro, list* segundo){
    list tmp = *primeiro;
    *primeiro = *(segundo);
    *segundo = tmp;
    return;
}

// Imprime os valores de uma lista ligada
void printList(list* lista){
    if (!lista) return;
    printList(lista->next);
    printf("%d", lista->value);
}

// Coloca um número no array no formato da lista
void addNumArr(list* arrayPos, int value){
    if (value == 0) {
        list* tmp = malloc(sizeof(tmp));
        tmp->value = 0;
        tmp->next = NULL;
        arrayPos->next = tmp;
        return;
    }
    while(value>0){
        list* tmp = malloc(sizeof(list));
        tmp->value = value%10;
        tmp->next = arrayPos->next;
        arrayPos->next = tmp;
        value/=10;
    }
    //*arrayPos = *(arrayPos->next);
}

// Adiciona cartas do input à lista
void getCartasValue(list** array, int* cartas){
    int tmp = 0;
    for(int i = 0; i < *cartas; i++){
        scanf("%d",&tmp);
        addNumArr(&(*array)[i], tmp);
        //printf("Numero %d adicionado com sucesso\n", tmp); // para testes
    }
}

// Recebe o número de cartas (input: linha 1) e aloca elas (input: linha 2)
void innit(list** array, int* cartas){
    scanf("%d", cartas);
    *array = malloc(*cartas * sizeof(list));
    getCartasValue(array, cartas);
    return;
}

// Imprime as cartas na ordem correta
void printCarta(list** array, int* cartas){
    for(int i = 0; i< *cartas; i++){
        printList(&(*array)[i]);
    }
    printf("\n");
}

// Retorna 0 se a > b, retorna 1 se b > a
int compare(list* a, list* b){
    if(a->value > b->value) return 0;
    if(a->value < b->value) return 1; 
    if(a->value == b->value){
        if(a->next == NULL && b->next == NULL) return 0;
        if(a->next == NULL){
            if(b->next->value == 0) return 0;
            else return 1;
        }
        if(b->next == NULL){
            if(a->next->value == 0) return 1;
            else return 0;
        }
        return compare(a->next, b->next);
    }
}

// Função auxiliar do mergeSort
void merge(list* a, list* b) {
    list* tmp = malloc(sizeof(list));
    while (a->next != NULL && b->next != NULL) {
        if (compare(&a, &b) == 1) {
            tmp->value = a->value;
            a = a->next;
        } else {
            tmp->value = b->value;
            b = b->next;
        }
    }
}


list* mergeSort(list* original) {
    if (original->next->next != NULL) {
        list* tmp1 = malloc(sizeof(list));
        list* tmp2 = malloc(sizeof(list));
        int counter = 0;
        while (original->next != NULL) {
            original = original->next;
            counter++;
        }
        counter /= 2;
        while (original->next != NULL) {
            if (counter >= 0) {
                tmp1->value = original->value;
                tmp1->next = original->next;
            }
            else {
                tmp2->value = original->value;
                tmp2->next = original->next;
            }
            original = original->next;
            counter--;
        }
        mergeSort(&tmp1);
        mergeSort(&tmp2);
        merge(&tmp1, &tmp2);
    }
    return original;
}

// Função principal do programa
int main(){
    int nCartas;
    list* arrayCartas = NULL;
    innit(&arrayCartas, &nCartas);
    if(compare(&(arrayCartas)[0], &(arrayCartas)[1])){
        swap(&(arrayCartas)[0], &(arrayCartas)[1]);
    }
    printCarta(&arrayCartas, &nCartas);

    return 0;
}
