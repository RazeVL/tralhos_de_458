/* ESTUDANTES: 198218 Leonardo Paillo da Silva | 193329 Rafael de Oliveira Fernandes */

#include <stdio.h>
#include <stdlib.h>

typedef struct listyay {
    short int value;
    struct listyay * next;
} list;

void swap(list* prim, list* segundo){
    list tmp = *prim;
    *prim = *(segundo);
    *segundo = tmp;
    return;


}

void printList(list* lista){
    int num = 0;
    list* tmp = lista;
    while(tmp){
        num *= 10;
        num += tmp->value;
        tmp = tmp->next;
    }
    printf("%d", num);



}

void  addNumArr(list* arrayPos, int value){
    
    while(value>0){
        list* tmp = malloc(sizeof(list));
        tmp->value = value%10;
        tmp->next = arrayPos->next;
        arrayPos->next = tmp;
        value/=10;
    }

    *arrayPos = *(arrayPos->next);

}


void getCartasValue(list** array, int* catras){
    int tmp = 0;
    for(int i = 0; i< *catras; i++){
        scanf(" %d",&tmp);
        addNumArr(&(*array)[i], tmp);
        printf("Numero %d adicionado com sucesso\n", tmp);
    }
}


void innit(list** array, int* cartas){
    scanf("%d", cartas);
    *array = malloc(*cartas * sizeof(list));
    if(array == NULL){
        printf("Erro inicializando o array.\n");
    }
    getCartasValue(array, cartas);
    return;
}

void printCarta(list** array, int* catras){
    for(int i = 0; i< *catras; i++){
        printList(&(*array)[i]);
    }
    printf("\n");
}

//retorna 0 se a > b, retorna 1 se b > a
int compare(list* a, list* b){
    if(a->value > b->value){
        return 0;
    }
    if(a->value < b->value){
        return 1;
    } 
    if(a->value == b->value){
        if(a->next == NULL && b->next == NULL){
            return 0;
        }
        if(a->next == NULL){
            if(b->next->value == 0){
                return 0;
            }
            else {return 1;}
        }
        if(b->next == NULL){
            if(a->next->value == 0){
                return 1;
            }
            else return 0;

        }
        return compare(a->next, b->next);
    } 
    

}



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