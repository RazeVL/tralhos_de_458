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
    Aviao* aviao1;
    Aviao* aviao2;
    struct paresList* next;

}paresList;

typedef struct{
    long int menorDist;
    short int hasMenorDist;
    paresList* paresFirst;
    paresList* paresLast;

}distPares;


typedef struct {
    int startList;
    int endList;
    int sizeList;
    Avioes* array;
    distPares* distParesMestre;
} mestre;


void initializeNonCheckedDistPares(mestre* mestreNaoVerificado){
    mestreNaoVerificado->distParesMestre = malloc(sizeof(distPares));
    mestreNaoVerificado->distParesMestre->hasMenorDist = 0;
    mestreNaoVerificado->distParesMestre->hasMenorDist = 0;
    mestreNaoVerificado->distParesMestre->paresFirst = NULL;
    mestreNaoVerificado->distParesMestre->paresLast = NULL;
    
}

void firstHalf(mestre* originalMestre, mestre* firstHalfMestre){
    firstHalfMestre->array = originalMestre->array;
    firstHalfMestre->startList = originalMestre->startList;
    firstHalfMestre->endList = originalMestre->endList / 2;
    firstHalfMestre->sizeList = firstHalfMestre->endList - firstHalfMestre->startList + 1;
    initializeNonCheckedDistPares(firstHalfMestre);
    return;
}

void secondHalf(mestre* originalMestre, mestre* secondHalfMestre){
    secondHalfMestre->array = originalMestre->array;
    secondHalfMestre->startList = originalMestre->endList/2 + 1;
    secondHalfMestre->endList = originalMestre->endList;
    secondHalfMestre->sizeList = secondHalfMestre->endList - secondHalfMestre->startList + 1;
    initializeNonCheckedDistPares(secondHalfMestre);
    return;
}

void concedeListPontos(mestre* passivo, mestre* ativo){
    passivo ->distParesMestre ->hasMenorDist = 1;
    passivo ->distParesMestre ->menorDist = ativo->distParesMestre->menorDist;
    passivo ->distParesMestre ->paresFirst = ativo->distParesMestre->paresFirst;
    passivo->distParesMestre -> paresLast = ativo->distParesMestre->paresLast;
    return;
}

void mergeConcedeListPontos(mestre* passivo, mestre* ativo1, mestre* ativo2){
    concedeListPontos(passivo, ativo1);
    paresList* tmp = ativo2->distParesMestre->paresLast;
    passivo->distParesMestre->paresLast = ativo2->distParesMestre->paresFirst;
    passivo->distParesMestre->paresLast = tmp;
}

void compareTemDistMestre(mestre* mestreOriginal, mestre* mestreFirst, mestre* mestreSecond){
    if(mestreFirst->distParesMestre->hasMenorDist && mestreSecond->distParesMestre->hasMenorDist){
        if(mestreFirst->distParesMestre->menorDist != mestreSecond->distParesMestre->menorDist){
            mestre* mestreMenorDist = mestreFirst->distParesMestre->menorDist < mestreSecond->distParesMestre->menorDist ? mestreFirst : mestreSecond;
            concedeListPontos(mestreOriginal, mestreMenorDist);
        }
        if(mestreFirst->distParesMestre->menorDist == mestreSecond->distParesMestre->menorDist){
            mergeConcedeListPontos(mestreOriginal, mestreFirst, mestreSecond);
        }
    }
    else if(mestreFirst->distParesMestre->hasMenorDist){
        concedeListPontos(mestreOriginal, mestreFirst);
    }
    else if(mestreSecond->distParesMestre->hasMenorDist){
        concedeListPontos(mestreOriginal, mestreSecond);
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
    if(mestreOriginal->sizeList <= 1){ //Basicamente garantindo flags que agiliza a verificacao do estado do distPares
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
    initializeNonCheckedDistPares(primeiroMestre);
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
    int x_cord = 0, y_cord = 0;
    scanf(" %d %d", &x_cord, &y_cord);
    if(x_cord > MAX_POS || x_cord < -MAX_POS || y_cord > MAX_POS || y_cord < -MAX_POS){
        printf("Posicao invalida, Aviao %d em posicao invalida (X = %d, Y = %d )\n",*ptrPosicao, x_cord, y_cord);
        return 1;
    }
    Aviao* tmp = malloc(sizeof(Aviao));
    if(tmp == NULL){
        printf("Falha na criacao do Aviao %d.\n", *ptrPosicao);
        return 1;
    }
    tmp->x = x_cord;
    tmp->y = y_cord;
    tmp->ordemAdicionado = *ptrPosicao;
    ptrArrayAvioes[(*ptrPosicao)] = tmp;
    return 0;
}

int main(int argc){
    int NAvioesRegistrar;
    scanf(" %d", &NAvioesRegistrar);

    if(NAvioesRegistrar < 0 || NAvioesRegistrar > MAX_AERONAVES){
        printf("Numero de Avioes nao valido\n");
        return -1;
    }
    
    printf("Numero de avioes Validos\n");

    if(NAvioesRegistrar){
        Avioes* arrayAvioes = malloc(NAvioesRegistrar * sizeof(Aviao*));

        if(arrayAvioes == NULL){
            printf("Falha em alocacao de memoria de arrayAvioes\n");
            return 1;
        }

        for(int i = 0; i < NAvioesRegistrar; i++){
            if(adicionarAvioes(&i, arrayAvioes)){
                printf("Saindo do ForLoop.\nEncerrando Codigo\n");
                return 1;
            }
        }
        
        printf("\nAvioes criados e alocados com sucesso\n");

        if(argc >2){
            printAvioes(&NAvioesRegistrar, arrayAvioes);
        }







        printf("-------\nEncerrando o programa...\n-------\n");

        encerrarPrograma(&NAvioesRegistrar, arrayAvioes);



        
    }
    else{
        printf("Nada a fazer, encerrando o codigo com sucesso!\n");
    }

    return 0;


}