#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAX_AERONAVES (pow(10,7))
#define MAX_POS (pow(10,9))

typedef struct{
    int x;
    int y;
    int ordemAdicionado;
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
    short int hasMenorDist;
    pares paresFirst;
    pares paresLast;
}distancia;


typedef struct {
    int startList;
    int endList;
    int sizeList;
    Avioes* array;
    distancia* distPares;
} lista;
typedef lista* mestre;


void initUncheckedDistPares(mestre mestreUnchecked){
    mestreUnchecked->distPares = malloc(sizeof(distancia));
    mestreUnchecked->distPares->menorDist = 0;
    mestreUnchecked->distPares->hasMenorDist = 0;
    mestreUnchecked->distPares->paresFirst = NULL;
    mestreUnchecked->distPares->paresLast = NULL;
}


void firstHalf(mestre original, mestre firstHalfM){
    firstHalfM->array = original->array;
    firstHalfM->startList = original->startList;
    firstHalfM->endList = original->endList / 2;
    firstHalfM->sizeList = firstHalfM->endList - firstHalfM->startList + 1;
    initUncheckedDistPares(firstHalfM);
    return;
}


void secondHalf(mestre original, mestre secondHalfM){
    secondHalfM->array = original->array;
    secondHalfM->startList = original->endList/2 + 1;
    secondHalfM->endList = original->endList;
    secondHalfM->sizeList = secondHalfM->endList - secondHalfM->startList + 1;
    initUncheckedDistPares(secondHalfM);
    return;
}


void concedeListPontos(mestre passivo, mestre ativo){
    passivo->distPares->hasMenorDist = 1;
    passivo->distPares->menorDist = ativo->distPares->menorDist;
    passivo->distPares->paresFirst = ativo->distPares->paresFirst;
    passivo->distPares->paresLast = ativo->distPares->paresLast;
    return;
}


void mergeConcedeListPontos(mestre passivo, mestre ativo1, mestre ativo2){
    concedeListPontos(passivo, ativo1);
    pares tmp = ativo2->distPares->paresLast;
    passivo->distPares->paresLast = ativo2->distPares->paresFirst;
    passivo->distPares->paresLast = tmp;
}


void compareTemDistMestre(mestre original, mestre first, mestre second){
    if(first->distPares->hasMenorDist && second->distPares->hasMenorDist){
        if(first->distPares->menorDist != second->distPares->menorDist){
            mestre mestreMenorDist = first->distPares->menorDist < second->distPares->menorDist ? first:second;
            concedeListPontos(original, mestreMenorDist);
        }
        if(first->distPares->menorDist == second->distPares->menorDist){
            mergeConcedeListPontos(original, first, second);
        }
    }
    else if(first->distPares->hasMenorDist){
        concedeListPontos(original, first);
    }
    else if(second->distPares->hasMenorDist){
        concedeListPontos(original, second);
    }
    return;
}


long int calcDistAvioes(Avioes aviao1, Avioes aviao2 ){
    return (pow(aviao1->x - aviao2->x,2) + pow(aviao1->y - aviao2->y, 2));
}


void newParesFirstList(distancia* distPares, Avioes aviao1, Avioes aviao2){
    pares tmp = malloc(sizeof(par));
    tmp->aviao1 = aviao1->ordemAdicionado < aviao2->ordemAdicionado ? aviao1 : aviao2;
    tmp->aviao2 = aviao1->ordemAdicionado > aviao2->ordemAdicionado ? aviao1 : aviao2;
    tmp->next = distPares->paresFirst;
    distPares->paresFirst = tmp;
    return;
}


void newLowestDist(mestre mestreOriginal,Avioes aviao1, Avioes aviao2){
    mestreOriginal->distPares->menorDist = calcDistAvioes(aviao1, aviao2);
    mestreOriginal->distPares->paresFirst = NULL;
    newParesFirstList(mestreOriginal->distPares, aviao1, aviao2);
    mestreOriginal->distPares->paresLast = mestreOriginal->distPares->paresFirst;
    return;
}


void newLowestDistWithDist(mestre mestreOriginal,Avioes aviao1, Avioes aviao2, long int* newLowest){
    mestreOriginal->distPares->menorDist = *newLowest;
    mestreOriginal->distPares->paresFirst = NULL;
    newParesFirstList(mestreOriginal->distPares, aviao1, aviao2);
    mestreOriginal->distPares->paresLast = mestreOriginal->distPares->paresFirst;
    return;
}


void distCalcFinder(mestre mestreOriginal, mestre mestreFirst, mestre mestreSecond){
    compareTemDistMestre(mestreOriginal, mestreFirst, mestreSecond);
    short int* doesMestreHaveSmallDist = &mestreOriginal->distPares->hasMenorDist;
    Avioes* arrayAvioes = mestreOriginal->array;
    for(int i = mestreFirst->startList; i == mestreFirst->endList; i++ ){
        for(int j = mestreSecond->startList; j == mestreSecond->endList; j++){
            if(*doesMestreHaveSmallDist == 0){
                mestreOriginal->distPares->hasMenorDist = 1;
                newLowestDist(mestreOriginal, mestreOriginal->array[i], mestreOriginal->array[j]);
            }
            long int DistCalc = calcDistAvioes(arrayAvioes[i], arrayAvioes[j]);
            if(mestreOriginal->distPares->menorDist == DistCalc){
                newParesFirstList(mestreOriginal->distPares, arrayAvioes[i],arrayAvioes[j]);
            }
            if(mestreOriginal->distPares->menorDist > DistCalc){
                newLowestDistWithDist(mestreOriginal, arrayAvioes[i], arrayAvioes[j], &DistCalc);
            }
            
        }
    }
    mestreFirst = NULL;
    mestreSecond = NULL;
    return;
}


void distFinderCount(mestre mestreOriginal){
    if(mestreOriginal->sizeList <= 1){ //Basicamente garantindo flags que agiliza a verificacao do estado do distPares
        return;
    }
    else{
        mestre mestre1 = malloc(sizeof(lista));
        firstHalf(mestreOriginal, mestre1);
        mestre mestre2 = malloc(sizeof(lista));
        secondHalf(mestreOriginal, mestre2);
        distCalcFinder(mestreOriginal, mestre1, mestre2);
        return;
    }
}


void firstDistFinderCount(mestre primeiroMestre, Avioes* ptrAvioesArray, int* ptrNAvioes){
    primeiroMestre = malloc(sizeof(lista));
    primeiroMestre->array = ptrAvioesArray;
    primeiroMestre->endList = *ptrNAvioes - 1;
    primeiroMestre->startList = 0;
    primeiroMestre->sizeList = *ptrNAvioes;
    initUncheckedDistPares(primeiroMestre);
    distFinderCount(primeiroMestre);
    return;

}


void printAvioes(int* ptrNAvioes, Avioes* ptrArrayAvioes){
    for(int i = 0; i <*ptrNAvioes; i++){
        printf("Aviao %d, coordenadas (X = %d,Y = %d)\n",ptrArrayAvioes[i]->ordemAdicionado, ptrArrayAvioes[i]->x, ptrArrayAvioes[i]->y);
    }
}


void encerrarPrograma(int* ptrNAvioes, Avioes* ptrArrayAvioes){
    for(int i = 0; i <*ptrNAvioes; i++){
        ptrArrayAvioes[i] = NULL;
    }
    free(ptrArrayAvioes);
    ptrArrayAvioes = NULL;
    printf("Programa encerrado com exito!\n");
    return;
}


int adicionarAvioes(int* ptrPosicao, Avioes* ptrArrayAvioes){
    int x_coord = 0, y_coord = 0;
    scanf(" %d %d", &x_coord, &y_coord);
    if(x_coord>MAX_POS || x_coord<-MAX_POS || y_coord>MAX_POS || y_coord<-MAX_POS) return 1; // pos. invalida
    Avioes tmp = malloc(sizeof(Aviao));
    tmp->x = x_coord;
    tmp->y = y_coord;
    tmp->ordemAdicionado = *ptrPosicao;
    ptrArrayAvioes[(*ptrPosicao)] = tmp;
    return 0;
}


int main(int argc){
    int n;
    scanf("%d", &n);
    if(n <= 0 || n > MAX_AERONAVES) return -1; // numero invalido

    Avioes* arrayAvioes = malloc(n * sizeof(Avioes));

    for(int i=0; i<n; i++) if(adicionarAvioes(&i, arrayAvioes)) return 1; // adiciona+verifica pos. invalida

    if(argc>2) printAvioes(&n, arrayAvioes);

    encerrarPrograma(&n, arrayAvioes);
    return 0;
}