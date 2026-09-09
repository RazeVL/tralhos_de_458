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
} par;


typedef par* pares;


typedef struct{
    /* Distancias entre pares de coordenadas */
    long int menorDist;
    short int hasMenorDist; // 0 = false, 1 = true
    pares paresFirst;
    pares paresLast;
} dists;


typedef struct {
    int startList;
    int endList;
    int sizeList;
    Avioes* array;
    dists* distsMestre;
} mestre;


void initializeUncheckedDists(mestre* mestreUnchecked){
    mestreUnchecked->distsMestre = malloc(sizeof(dists));
    mestreUnchecked->distsMestre->menorDist = 0;
    mestreUnchecked->distsMestre->hasMenorDist = 0;
    mestreUnchecked->distsMestre->paresFirst = NULL;
    mestreUnchecked->distsMestre->paresLast = NULL;
}


void firstHalf(mestre* originalMestre, mestre* firstHalfMestre){
    firstHalfMestre->array = originalMestre->array;
    firstHalfMestre->startList = originalMestre->startList;
    firstHalfMestre->endList = originalMestre->endList / 2;
    firstHalfMestre->sizeList = firstHalfMestre->endList - firstHalfMestre->startList + 1;
    initializeUncheckedDists(firstHalfMestre);
    return;
}


void secondHalf(mestre* originalMestre, mestre* secondHalfMestre){
    secondHalfMestre->array = originalMestre->array;
    secondHalfMestre->startList = originalMestre->endList/2 + 1;
    secondHalfMestre->endList = originalMestre->endList;
    secondHalfMestre->sizeList = secondHalfMestre->endList - secondHalfMestre->startList + 1;
    initializeUncheckedDists(secondHalfMestre);
    return;
}


void concedeListPts(mestre* passivo, mestre* ativo){
    passivo->distsMestre->hasMenorDist = 1;
    passivo->distsMestre->menorDist = ativo->distsMestre->menorDist;
    passivo->distsMestre->paresFirst = ativo->distsMestre->paresFirst;
    passivo->distsMestre-> paresLast = ativo->distsMestre->paresLast;
    return;
}


void mergeConcedeListPts(mestre* passivo, mestre* ativo1, mestre* ativo2){
    concedeListPts(passivo, ativo1);
    pares tmp = ativo2->distsMestre->paresLast;
    passivo->distsMestre->paresLast = ativo2->distsMestre->paresFirst;
    passivo->distsMestre->paresLast = tmp;
}


void compareTemDistMestre(mestre* mestreOriginal, mestre* mestreFirst, mestre* mestreSecond){
    if(mestreFirst->distsMestre->hasMenorDist && mestreSecond->distsMestre->hasMenorDist){
        if(mestreFirst->distsMestre->menorDist != mestreSecond->distsMestre->menorDist){
            mestre* mestreMenorDist = mestreFirst->distsMestre->menorDist < mestreSecond->distsMestre->menorDist ? mestreFirst : mestreSecond;
            concedeListPts(mestreOriginal, mestreMenorDist);
        }
        if(mestreFirst->distsMestre->menorDist == mestreSecond->distsMestre->menorDist){
            mergeConcedeListPts(mestreOriginal, mestreFirst, mestreSecond);
        }
    }
    else if(mestreFirst->distsMestre->hasMenorDist){
        concedeListPts(mestreOriginal, mestreFirst);
    }
    else if(mestreSecond->distsMestre->hasMenorDist){
        concedeListPts(mestreOriginal, mestreSecond);
    }
    return;
}


void distCalcFinder(mestre* mestreOriginal, mestre* mestreFirst, mestre* mestreSecond){
    compareTemDistMestre(mestreOriginal, mestreFirst, mestreSecond);
    for(int i = mestreFirst->startList; i == mestreFirst->endList; i++ ){
        for(int j = mestreSecond->startList; j == mestreSecond->endList; j++){
            
        }
    }

    mestreFirst = NULL;
    mestreSecond = NULL;
    return;
}


void distFinderCount(mestre* mestreOriginal){
    if(mestreOriginal->sizeList<=1){ //Basicamente garantindo flags que agiliza a verificacao do estado do distpares
        return;
    }
    else{
        mestre* mestre1 = malloc(sizeof(mestre));
        firstHalf(mestreOriginal, mestre1);
        mestre* mestre2 = malloc(sizeof(mestre));
        secondHalf(mestreOriginal, mestre2);
        distCalcFinder(mestreOriginal, mestre1, mestre2);

        return;
    }
}


void firstDistFinderCount(mestre* primeiroMestre, Avioes* ptrAvioesArray, int* ptrNAvioes){
    primeiroMestre = malloc(sizeof(mestre));
    primeiroMestre->array = ptrAvioesArray;
    primeiroMestre->endList = *ptrNAvioes - 1;
    primeiroMestre->startList = 0;
    primeiroMestre->sizeList = *ptrAvioesArray + 1;
    initializeUncheckedDists(primeiroMestre);
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
    printf("Programa encerrado com êxito!\n");
    return;
}


int adicionarAvioes(int* ptrPosicao, Avioes* ptrArrayAvioes){
    int x_cord = 0, y_cord = 0;
    scanf(" %d %d", &x_cord, &y_cord);
    if(x_cord > MAX_POS || x_cord < -MAX_POS || y_cord > MAX_POS || y_cord < -MAX_POS){
        printf("Posicao invalida, Aviao %d em posicao invalida (X = %d, Y = %d )\n",*ptrPosicao, x_cord, y_cord);
        return 1;
    }
    Aviao* tmp = malloc(sizeof(Aviao));
    if(tmp == NULL){
        printf("Falha na criação do Avião %d.\n", *ptrPosicao);
        return 1;
    }
    tmp->x = x_cord;
    tmp->y = y_cord;
    tmp->ordemAdicionado = *ptrPosicao;
    ptrArrayAvioes[(*ptrPosicao)] = tmp;
    return 0;
}


int main(int argc){
    int n;
    scanf("%d", &n);

    if(n<=0 || n>MAX_AERONAVES){
        printf("Numero de aviões inválido.\n"); // Assumimos n=0 inválido pois 0<=i<n
        return -1;
    }

    Avioes* arrayAvioes = malloc(n * sizeof(Aviao*));

    if(arrayAvioes == NULL){
        printf("Falha em alocacao de memoria de arrayAvioes\n");
        return 1;
    }

    for(int i = 0; i < n; i++){
        if(adicionarAvioes(&i, arrayAvioes)){
            printf("Saindo do loop.\nEncerrando código.\n");
            return 1;
        }
    }
    
    printf("\nAviões criados e alocados com sucesso.\n");

    if(argc >2){
        printAvioes(&n, arrayAvioes);
    }

    printf("-------\nEncerrando o programa...\n-------\n");
    encerrarPrograma(&n, arrayAvioes);

    return 0;
}
