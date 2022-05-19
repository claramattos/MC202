#include <stdlib.h>
#include <stdio.h>

int pode_inserir(int n, int linha, int coluna, int **matriz, int numero){
    int predios_vistos, predio_mais_alto;
    //verifica se o numero n pode ser inserido na posição indicada

    //verifica se n já existe naquela coluna
    //caso já exista, não pode ser inserido
    for (int i=1;i<linha;i++) {
        if (matriz[i][coluna] == numero){
            return 0;
        }
    }
    //verifica se n já existe naquela linha
    //caso já exista, não pode ser inserido
    for (int k=1;k<coluna;k++){
        if (matriz[linha][k] == numero) {
            return 0;
        }
    }

    //não deixa que um número diferente de n seja inserido após uma dica de número 1
    //não deixa que um número diferente de 1 seja inserido após uma dica de número n
    if (linha == 1) {
        if ((matriz[0][coluna]==n && numero != 1) || (matriz[0][coluna] ==1 && numero !=n))
            return 0;
    }
    if (coluna == 1) {
        if ((matriz[linha][0] == n && numero != 1) || (matriz[linha][0] == 1 && numero != n))
            return 0;
    }
    if (linha == n) {
        if ((matriz[n+1][coluna] == n && numero!=1) || (matriz [n+1][coluna] ==1 && numero != n))
            return 0;
    }
    if (coluna == n) {
        if ((matriz[linha][n+1] == n && numero != 1) || (matriz[linha][n+1] == 1 && numero !=n))
            return 0;
    }

    //ve se o numero de predios vistos de cima para baixo é o mesmo que a dica dada no início da coluna
    predios_vistos = 1;
    predio_mais_alto = matriz[1][coluna];
    for (int i=2;i<linha;i++){
        if (matriz[i][coluna] > predio_mais_alto){
            predio_mais_alto = matriz[i][coluna];
            predios_vistos += 1;
        }
        if (predios_vistos > matriz[0][coluna]){
            return 0;
        }
    }
    //caso estimermos checando a última linha e predios vistos seja diferente da dica, não pode ser inserido
    if (linha == n) {
        if (numero > predio_mais_alto) {
            predios_vistos++;
        }
        if (predios_vistos!= matriz[0][coluna])
            return 0;
    }

    //ve se o numero de predios vistos da esquerda para a direita é o mesmo que a dica dada no início da linha
    predios_vistos = 1;
    predio_mais_alto = matriz[linha][1];
    for (int i=2;i<coluna;i++){
        if (matriz[linha][i] > predio_mais_alto){
            predio_mais_alto = matriz[linha][i];
            predios_vistos += 1;
        }
        if (predios_vistos > matriz[linha][0]){
            return 0;
        }
    }
    //caso estimermos checando a última coluna e predios vistos seja diferente da dica, não pode ser inserido
    if (coluna == n) {
        if (numero > predio_mais_alto) {
            predios_vistos++;
        }
        if (predios_vistos!= matriz[linha][0])
            return 0;
    }

    //ve se o numero de predios vistos de baixo para cima esta certo
    //somente quando completarmos uma coluna
    if (linha == n){
        predios_vistos = 1; 
        predio_mais_alto = numero;
        for (int i=n-1;i>0;i--){
            if (matriz[i][coluna] > predio_mais_alto){
                predio_mais_alto = matriz[i][coluna];
                predios_vistos += 1;
            }
        }
        if (predios_vistos != matriz[n+1][coluna]){
            return 0;
        }
    }

    //ve se o numero de predios vistos da esquerda para a direita esta certo
    //somente quando completarmos uma linha
    if (coluna == n){
        predios_vistos = 1; 
        predio_mais_alto = numero;
        for (int i=n-1; i > 0; i--){
            if (matriz[linha][i] > predio_mais_alto){
                predio_mais_alto = matriz[linha][i];
                predios_vistos += 1;
            }
        }
        if (predios_vistos != matriz[linha][n+1]){
            return 0;
        }
    }

    //caso o número não caia em nenhuma condição de exclsão
    return 1;
}

int arranhaceu_recursivo(int n, int linha, int coluna, int **matriz){
    if (linha > n){
        //chegamos ao fim, vamos imprimir a matriz obtida
        for (int i=1; i<n+1; i++){
            for (int k=1; k< n+1; k++){
                printf("%d ", matriz[i][k]);
            }
            printf("\n");
        }
        return 1;
    }

    //calculamos a proxima posição
    int proxima_linha, proxima_coluna;
    if (0<coluna && coluna<n){
        proxima_linha = linha;
        proxima_coluna = coluna + 1;
    } 
    else {
        proxima_linha = linha + 1;
        proxima_coluna = 1;
    }

    //preenchemos a posição atual 
    for (int i=1;i<n+1;i++){
        //verificamos se o número pode ser inserido
        if(pode_inserir(n, linha, coluna, matriz, i) == 1) { 
            matriz[linha][coluna] = i;
            //chamamos a função para a posição seguinte
            if (arranhaceu_recursivo(n, proxima_linha, proxima_coluna, matriz) == 1) { 
                return 1;
            }
        }
    }

    return 0;
}

//destruímos uma matriz
void libera_matriz (int n, int **matriz) {
    for (int i=0; i < n+2; i++){
        free(matriz[i]);
    }
    free(matriz);
}

int main(){
    //dimensão da matriz sem as dicas
    int n;
    scanf("%d\n", &n);

    //criamos e preenchemos a matriz inicial
    int **matriz;
    matriz = malloc((n+2)*sizeof(int*));
    for (int i=0; i < n+2; i++){
        matriz[i] = malloc((n+2)* sizeof(int));
    }

    //lemos e preenchemos a matriz
    for (int i=0; i<n+2; i++){
        for (int k=0;k<n+2;k++){
            scanf("%d", &matriz[i][k]);
        }
    }

    //chamamos a função que resolverá o problema
    arranhaceu_recursivo(n, 1, 1, matriz);

    //liberamos a memoria 
    libera_matriz(n, matriz);

    return 0;
}