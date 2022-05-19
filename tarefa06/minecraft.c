#include <stdio.h>
#include <stdlib.h>
#include "minecraft.h"

int main() {
    int m, n, seed, **altitudes, qtdBlocos =0, qtdDiamante=0, qtdOuro=0, qtdFerro=0, i, j;
    Bloco ***mundo;
    float tempoPorBloco;

    scanf("%d %d", &m, &n);
    scanf("%d", &seed);
    scanf("%f", &tempoPorBloco);

    altitudes = calcularAltitudes(m, n, seed);
    mundo = criarMundo(m, n, altitudes, seed);
    explorarMundo(mundo, m, n, altitudes, tempoPorBloco, &qtdDiamante, &qtdOuro, &qtdFerro, &qtdBlocos);
    
    printf("Total de Blocos: %d\n",  qtdBlocos);
    printf("Tempo total: %.2fs\n",  tempoPorBloco*qtdBlocos);
    printf ("Diamantes: %d\n", qtdDiamante);
    printf("Ouros: %d\n",  qtdOuro);
    printf("Ferros: %d\n",  qtdFerro);

    for (i=0; i < m; i++)
        free (altitudes[i]) ;
    free (altitudes);

    for (i=0; i < m; i++) {
        for (j=0;j<n;j++)
            free (mundo[i][j]) ;
        free (mundo[i]);
    }
    free(mundo);

    return 0;
}