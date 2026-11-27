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

    ll tungparcial[m], tungfinal[m];

    

    return 0;
}