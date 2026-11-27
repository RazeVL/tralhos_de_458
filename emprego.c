/* ESTUDANTES: 198218 Leonardo Paillo da Silva | 193329 Rafael de Oliveira Fernandes */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TETO 1000000000000000000LL

typedef long long ll;

int main() {
    ll n, m, res = TETO;
    scanf("%lld %lld", &n, &m);
    ll c[n][m]; //tabela de custos
    
    for (int i=0; i<=n; i++) for (int j=0; j<=m; j++) scanf("%lld", &c[i][j]);

    if (n == 1) { // so 1 casa: menor custo = menor valor da tabela
        for (int tung = 1; tung <= m; tung++) if (c[0][tung] < res) res = c[0][tung];
        printf("%lld\n", res);
        return 0;
    }

    ll tunganterior[m], tungatual[m];
    for (int cor1 = 0; cor1 <= m; cor1++) { // testa todas as possibilidades da cor da casa 1
        
        for (int j = 1; j <= m; j++) { // deixa o tunganterior inicializado
            if (j == cor1) tunganterior[j] = c[0][j];
            else tunganterior[j] = TETO;
        }

        for (int i = 1; i <= n; i++) {
            for (int j = 0; j <= m; j++) {
                ll minp = TETO; // menor parcial

                for (int k = 0; k <= m; k++) if (k != j && tunganterior[k] < minp) minp = tunganterior[k];

                if (minp == TETO) tungatual[j] = minp;
                else tungatual[j] = minp + c[i][j];
            }

            for (int j = 0; j <= m; j++) tunganterior[j] = tungatual[j];
        }
    }

    if (res == TETO) printf("-1\n");
    else printf("%lld\n", res);

    return 0;
}