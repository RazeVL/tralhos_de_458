/* ESTUDANTES: 198218 Leonardo Paillo da Silva | 193329 Rafael de Oliveira Fernandes */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAX_AERONAVES 10000000
#define MAX_POS 1000000000


typedef struct{
    int x;
    int y;
    int ordemAdicionado;
    int lado;
} Aviao;
typedef Aviao* Avioes;


typedef struct{
    Avioes aviao1;
    Avioes aviao2;
    struct par* next;
}par;
typedef par* pares;


typedef struct{
    long int menorDist;
    short int hasMenorDist; // Variável booleana, 0 = não tem | 1 = tem a menor distância nesse struct
    int numPares; // Quantos pares tem a menor distância
    pares paresFirst;
    pares paresLast;
} distancia;


typedef struct {
    int startList;
    int endList;
    int sizeList;
    Avioes* array;
    Avioes* arrayY;
    distancia* distPares;
} lista;
typedef lista* mestre;


/* As próximas 2 funções comparam as coordenadas de 2 aviões, ordenando de menor para maior */
int compararAvioesX(const void* a, const void* b){
    Avioes aa = *(Avioes*)a;
    Avioes bb = *(Avioes*)b;
    if(aa->x != bb->x) return aa->x - bb->x;
    return aa->y - bb->y;
}
int compararAvioesY(const void* a, const void* b){
    Avioes aa = *(Avioes*)a;
    Avioes bb = *(Avioes*)b;
    if(aa->y != bb->y) return aa->y - bb->y;
    return aa->x - bb->x;
}


void initUncheckedDistPares(mestre unchecked){
    /* Inicializa a struct de distância */
    unchecked->distPares = malloc(sizeof(distancia));
    unchecked->distPares->menorDist = 0;
    unchecked->distPares->hasMenorDist = 0;
    unchecked->distPares->numPares = 0;
    unchecked->distPares->paresFirst = NULL;
    unchecked->distPares->paresLast = NULL;
}


void liberarListaPares(pares first){
    /* Libera lista de pares */
    pares atual = first;
    while(atual != NULL){
        pares prox = atual->next;
        free(atual);
        atual = prox;
    }
}


void firstHalf(mestre original, mestre firstHalfM){
    /* Divide em 2, pega a 1a metade */
    firstHalfM->array = original->array;
    firstHalfM->startList = original->startList;
    firstHalfM->endList = (original->startList + original->endList) / 2;
    firstHalfM->sizeList = firstHalfM->endList - firstHalfM->startList + 1;
    firstHalfM->arrayY = malloc(firstHalfM->sizeList * sizeof(Avioes));
    initUncheckedDistPares(firstHalfM);
    return;
}


void secondHalf(mestre original, mestre secondHalfM){
    /* Divide em 2, pega a 2a metade */
    secondHalfM->array = original->array;
    secondHalfM->startList = (original->startList + original->endList) / 2 + 1  ;
    secondHalfM->endList = original->endList;
    secondHalfM->sizeList = secondHalfM->endList - secondHalfM->startList + 1;
    secondHalfM->arrayY = malloc(secondHalfM->sizeList * sizeof(Avioes));
    initUncheckedDistPares(secondHalfM);
    return;
}


void copiaListPontos(mestre passivo, mestre ativo){
    /* Copia ativo dentro do passivo */
    passivo->distPares->menorDist = ativo->distPares->menorDist;
    passivo->distPares->hasMenorDist = 1;
    passivo->distPares->numPares = ativo->distPares->numPares;
    passivo->distPares->paresFirst = ativo->distPares->paresFirst;
    passivo->distPares->paresLast = ativo->distPares->paresLast;
    return;
}


void mergeCopiaListPontos(mestre passivo, mestre ativo1, mestre ativo2){
    /* Faz merge de ativo1 e ativo2 dentro do passivo */
    copiaListPontos(passivo, ativo1);
    passivo->distPares->paresLast->next = ativo2->distPares->paresFirst;
    passivo->distPares->paresLast = ativo2->distPares->paresLast;
    passivo->distPares->numPares += ativo2->distPares->numPares;
}


void compareTemDistMestre(mestre original, mestre first, mestre second){
    /* Combina os resultados das listas parciais na lista original */
    if(first->distPares->hasMenorDist && second->distPares->hasMenorDist){
        if(first->distPares->menorDist < second->distPares->menorDist){
            copiaListPontos(original, first);
            liberarListaPares(second->distPares->paresFirst);
            second->distPares->paresFirst = NULL;
            second->distPares->paresLast = NULL;
            second->distPares->numPares = 0;
        }
        else if(second->distPares->menorDist < first->distPares->menorDist){
            copiaListPontos(original, second);
            liberarListaPares(first->distPares->paresFirst);
            first->distPares->paresFirst = NULL;
            first->distPares->paresLast = NULL;
            first->distPares->numPares = 0;
        }
        else {
            mergeCopiaListPontos(original, first, second);
        }
    }
    else if(first->distPares->hasMenorDist){
        copiaListPontos(original, first);
    }
    else if(second->distPares->hasMenorDist){
        copiaListPontos(original, second);
    }
    return;
}


long int calcDistAvioes(Avioes a, Avioes b){
    /* d² = x² + y². Teorema de Pitágoras. */
    long int dx = (long int)a->x - b->x;
    long int dy = (long int)a->y - b->y;
    return dx*dx + dy*dy;
}


void newParesFirstList(distancia* distPares, Avioes aviao1, Avioes aviao2){
    pares tmp = malloc(sizeof(par));
    tmp->aviao1 = aviao1->ordemAdicionado < aviao2->ordemAdicionado ? aviao1 : aviao2;
    tmp->aviao2 = aviao1->ordemAdicionado > aviao2->ordemAdicionado ? aviao1 : aviao2;
    tmp->next = distPares->paresFirst;
    distPares->paresFirst = tmp;
    distPares->numPares++;
    return;
}



/* Chama essas funções quando encontrar uma nova menor distância */
void newLowestDist(mestre mestreOriginal,Avioes aviao1, Avioes aviao2){
    liberarListaPares(mestreOriginal->distPares->paresFirst);
    mestreOriginal->distPares->menorDist = calcDistAvioes(aviao1, aviao2);
    mestreOriginal->distPares->paresFirst = NULL;
    mestreOriginal->distPares->numPares = 0;
    newParesFirstList(mestreOriginal->distPares, aviao1, aviao2);
    mestreOriginal->distPares->paresLast = mestreOriginal->distPares->paresFirst;
    return;
}


void newLowestDistWithDist(mestre mestreOriginal,Avioes aviao1, Avioes aviao2, long int* newLowest){
    liberarListaPares(mestreOriginal->distPares->paresFirst);
    mestreOriginal->distPares->menorDist = *newLowest;
    mestreOriginal->distPares->paresFirst = NULL;
    mestreOriginal->distPares->numPares = 0;
    newParesFirstList(mestreOriginal->distPares, aviao1, aviao2);
    mestreOriginal->distPares->paresLast = mestreOriginal->distPares->paresFirst;
    return;
}


void distCalcFinder(mestre original, mestre first, mestre second){
    /* Ordena o arrayY combinado por ordem de y, usando uma faixa central para
    comparar geometricamente com seus vizinhos na coordenada y */
    compareTemDistMestre(original, first, second);
    int k = 0;
    for(int i = 0; i < first->sizeList; i++) original->arrayY[k++] = first->arrayY[i];
    for(int j = 0; j < second->sizeList; j++) original->arrayY[k++] = second->arrayY[j];
    qsort(original->arrayY, original->sizeList, sizeof(Avioes), compararAvioesY);

    long int delta = original->distPares->hasMenorDist ? original->distPares->menorDist : (long int) 4e18; // Assume infinito se nn tiver menor dist

    int xMeio = original->array[first->endList]->x;

    Avioes* faixa = malloc(original->sizeList * sizeof(Avioes));
    int tamFaixa = 0;
    for(int t = 0; t < original->sizeList; t++){
        Avioes p = original->arrayY[t];
        long int dx = (long int)p->x - xMeio;
        if(dx < 0) dx = -dx;
        if(dx < delta) faixa[tamFaixa++] = p;
    }

    for(int i = first->startList; i <= first->endList; i++) original->array[i]->lado = 0;
    for(int j = second->startList; j <= second->endList; j++) original->array[j]->lado = 1;

    for(int a = 0; a < tamFaixa; a++){
        for(int b = a + 1; b < tamFaixa && b <= a + 7; b++){
            if(faixa[a]->lado == faixa[b]->lado) continue;
            long int dy = (long int)faixa[b]->y - faixa[a]->y;
            if(dy >= delta) break;   // Ordenado por Y, para aqui porque não precisaria comparar com todos
            long int DistCalc = calcDistAvioes(faixa[a], faixa[b]);
            if(!original->distPares->hasMenorDist){
                original->distPares->hasMenorDist = 1;
                newLowestDist(original, faixa[a], faixa[b]);
                delta = original->distPares->menorDist;
            }
            else if(original->distPares->menorDist == DistCalc){
                newParesFirstList(original->distPares, faixa[a], faixa[b]);
            }
            else if(original->distPares->menorDist > DistCalc){
                newLowestDistWithDist(original, faixa[a], faixa[b], &DistCalc);
                delta = DistCalc;
            }
        }
    }
    free(faixa);
    return;
}


void distFinderCount(mestre original){
    if(original->sizeList <= 1){ // Garante flags que agilizam a verificacao do estado da distancia
        if(original->sizeList == 1) {
            original->arrayY[0] = original->array[original->startList]; // arrayY=o próprio avião
        }
        return;
    }
    mestre mestre1 = malloc(sizeof(lista));
    firstHalf(original, mestre1);
    mestre mestre2 = malloc(sizeof(lista));
    secondHalf(original, mestre2);
    distFinderCount(mestre1);
    distFinderCount(mestre2);
    distCalcFinder(original, mestre1, mestre2);
    free(mestre1->arrayY);
    free(mestre1->distPares);
    free(mestre1);
    free(mestre2->arrayY);
    free(mestre2->distPares);
    free(mestre2);
    return;
}


mestre firstDistFinderCount(Avioes* ptrAvioesArray, int* ptrNAvioes){
    /* Para iniciar no main */
    mestre m = malloc(sizeof(lista));
    m->array = ptrAvioesArray;
    m->endList = *ptrNAvioes - 1;
    m->startList = 0;
    m->sizeList = *ptrNAvioes;
    m->arrayY = malloc(m->sizeList * sizeof(Avioes)); // Começa como cópia do array X normal, ordena no mergesort
    for(int i = 0; i < m->sizeList; i++) m->arrayY[i] = ptrAvioesArray[i];
    initUncheckedDistPares(m);
    distFinderCount(m);
    return m;
}


int compararPares(pares a, pares b){
    /* Verifica qual par foi adicionado primeiro, usando o aviao2 como critério de desempate;
    Positivo se o par a vem antes, negativo se o par b vem antes */
    if(a->aviao1->ordemAdicionado != b->aviao1->ordemAdicionado)
        return a->aviao1->ordemAdicionado - b->aviao1->ordemAdicionado;
    return a->aviao2->ordemAdicionado - b->aviao2->ordemAdicionado;
}


void mergePares(pares* v, pares* aux, int esq, int meio, int dir){
    int i = esq, j = meio + 1, k = esq;
    while(i <= meio && j <= dir){
        if(compararPares(v[i], v[j]) <= 0) {
            aux[k++] = v[i++];
        }
        else {
            aux[k++] = v[j++];
        }
    }
    while(i <= meio) {
        aux[k++] = v[i++];
    }
    while(j <= dir) {
        aux[k++] = v[j++];
    }
    for(i = esq; i <= dir; i++) {
        v[i] = aux[i];
    }
}


void mergeSortPares(pares* v, pares* aux, int esq, int dir){
    /* MergeSort top-down utilizando a função mergePares recursivamente */
    if(esq >= dir) return;
    int meio = (esq + dir) / 2;
    mergeSortPares(v, aux, esq, meio);
    mergeSortPares(v, aux, meio + 1, dir);
    mergePares(v, aux, esq, meio, dir);
}


pares* listaParaArrayOrdenado(distancia* d){
    /* Transforma a lista em array para fazer a verificação em X e em Y garantido O(n log² n)*/
    int m = d->numPares;
    if(m == 0) return NULL;
    pares* arr = malloc(m * sizeof(pares));
    pares* aux = malloc(m * sizeof(pares));
    pares atual = d->paresFirst;
    for(int i = 0; i < m; i++){
        arr[i] = atual;
        atual = atual->next;
    }
    mergeSortPares(arr, aux, 0, m - 1);
    free(aux);
    return arr;
}


void imprimirParesMenorDist(mestre m){
    pares* arr = listaParaArrayOrdenado(m->distPares);
    int mQuant = m->distPares->numPares;
    for(int i = 0; i < mQuant; i++){
        printf("(%d,%d)\n", arr[i]->aviao1->ordemAdicionado, arr[i]->aviao2->ordemAdicionado);
    }
    if(arr) {
        free(arr);
    }
    liberarListaPares(m->distPares->paresFirst);
    m->distPares->paresFirst = NULL;
    m->distPares->paresLast = NULL;
    m->distPares->numPares = 0;
}


void encerrarPrograma(int* ptrNAvioes, Avioes* ptrArrayAvioes){
    /* Libera o array e encerra o programa sem vazar nada */
    for(int i = 0; i <*ptrNAvioes; i++){
        free(ptrArrayAvioes[i]);
        ptrArrayAvioes[i] = NULL;
    }
    free(ptrArrayAvioes);
    ptrArrayAvioes = NULL;
    return;
}


int adicionarAvioes(int* ptrPosicao, Avioes* ptrArrayAvioes){
    /* Lê as entradas */
    int x_coord = 0, y_coord = 0;
    scanf("%d %d", &x_coord, &y_coord);
    if(x_coord>MAX_POS || x_coord<-MAX_POS || y_coord>MAX_POS || y_coord<-MAX_POS) return 1; // pos. invalida
    Avioes tmp = malloc(sizeof(Aviao));
    tmp->x = x_coord;
    tmp->y = y_coord;
    tmp->ordemAdicionado = *ptrPosicao;
    ptrArrayAvioes[(*ptrPosicao)] = tmp;
    return 0;
}


int main(int argc, char* argv[]){
    int n;
    scanf("%d", &n);
    if(n <= 0 || n > MAX_AERONAVES) return -1; // Verifica n invalido

    Avioes* arrayAvioes = malloc(n * sizeof(Avioes));
    for(int i=0; i<n; i++) if(adicionarAvioes(&i, arrayAvioes)) return 1; // Adiciona+verifica pos. invalida

    if(n >= 2){
        qsort(arrayAvioes, n, sizeof(Avioes), compararAvioesX);
        mestre resultado = firstDistFinderCount(arrayAvioes, &n);
        imprimirParesMenorDist(resultado);
        free(resultado->arrayY);
        free(resultado->distPares);
        free(resultado);
    }

    encerrarPrograma(&n, arrayAvioes);
    return 0;
}
