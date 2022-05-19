typedef struct {
    int tipo;
} Bloco;

/**
 * Cria uma matriz de altitudes representando
 * o relevo de um mundo.
 **/
int **calcularAltitudes(int m, int n, int seed) {
    int i, j, **altitudes;

    altitudes = malloc (m*sizeof(int*));

    for (i=0;i<m;i+=1)
        altitudes[i] = malloc(n*sizeof(int));

    for (i=0;i<m;i+=1){
        for (j=0;j<n;j+=1) {
            altitudes[i][j] = (seed*(202 + i + j) + 12345 + i + j) % 256;
        }
    }
    return altitudes; 
}

/**
 * Cria um novo mundo representado por uma matriz
 * de blocos alocada dinamicamente.
 **/
Bloco ***criarMundo(int m, int n, int **altitudes, int seed){
    int i, j, k;
    Bloco ***mundo;

    mundo = malloc (m*sizeof(Bloco**));

    for (i=0;i<m;i+=1)
        mundo[i] = malloc(n*sizeof(Bloco*));

    for (i=0;i<m;i+=1){
        for (j=0;j<n;j+=1) {
            mundo[i][j] = malloc(256*sizeof(Bloco));
        }
    }

    for (i=0;i<m;i+=1){
        for (j=0;j<n;j+=1) {
            for (k=0;k<256;k+=1) {
                if (k>altitudes[i][j]) {
                    mundo[i][j][k].tipo = 21;
                }
                else {
                    mundo[i][j][k].tipo = (seed * (202 + i + j + k) + i + j + k) % 33;
                }
                
            }
        }
    }
    return mundo;
}

/**
 * Explora o mapa de um mundo e devolve o tempo
 * necessário para minerar todo ele. Além disso,
 * devolve a quantidade de alguns minérios de
 * interesse.
 **/
double explorarMundo( Bloco ***mundo, int m, int n, int **altitudes, double tempoPorBloco,
    int *qtdDiamante, int *qtdOuro, int *qtdFerro, int *qtdBlocos) {
    int i, j, k;
    float tempo_mineracao;

        for (i=0;i<m;i+=1){
            for (j=0;j<n;j+=1) {
                for (k=0;k<256;k+=1){
                    if (mundo[i][j][k].tipo == 0) {
                        (*qtdDiamante) += 1;
                        (*qtdBlocos)+= 1;
                    }
                    else if (1 <=mundo[i][j][k].tipo && mundo[i][j][k].tipo<=2) {
                        (*qtdOuro)+=1;
                        (*qtdBlocos)+=1; 
                    }          
                    else if (3 <=mundo[i][j][k].tipo && mundo[i][j][k].tipo<= 5){
                        (*qtdFerro)+=1;
                        (*qtdBlocos)+=1; 
                    }
                    else if (6 <=mundo[i][j][k].tipo && mundo[i][j][k].tipo<= 11)
                        (*qtdBlocos)+=1;
                    else if (12 <=mundo[i][j][k].tipo && mundo[i][j][k].tipo<= 20)
                        (*qtdBlocos)+=1;
                   
                }
            }
        
        }
        
    tempo_mineracao = (*qtdBlocos) *  tempoPorBloco;
    return tempo_mineracao;

    for (i=0; i < m; i++) {
        for (j=0;j<n;j++)
            free (mundo[i][j]) ;
        free (mundo[i]);
    }
    free(mundo);

    for (i=0; i < m; i++)
        free (altitudes[i]) ;
    free (altitudes);
}