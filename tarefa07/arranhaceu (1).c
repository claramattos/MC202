#include <stdio.h>
#include <stdlib.h>

int podeInserir(int n, int linha, int coluna, int numero, int **grade){
    /* verifica se o numero n pode ser inserido na posicao da grade definida */

    for (int c=1; c < coluna; c++){
        if (grade[linha][c] == numero){ // verificar se ja aparece na linha
            return 0;
        }
    }

    for (int l=1; l < linha; l++){
        if (grade[l][coluna] == numero){ // verificar se ja aparece na coluna
            return 0;
        }
    }

    // verificar se o numero de predios vistos da esquerda para a direita esta certo
    int contador = 1, maisAlto = grade[linha][1];
    for (int c=2; c < coluna; c++){
        if (grade[linha][c] > maisAlto){
            maisAlto = grade[linha][c];
            contador += 1;
        }
        if (contador > grade[linha][0]){
            return 0;
        }
    }
    if (coluna == n){
        if(numero > maisAlto){
            contador += 1;
        }
        if (contador != grade[linha][0]){
            return 0;
        }
    }

    // verificar se o numero de predios vistos de cima para baixo esta certo
    contador = 1; maisAlto = grade[1][coluna];
    for (int l=2; l < linha; l++){
        if (grade[l][coluna] > maisAlto){
            maisAlto = grade[l][coluna];
            contador += 1;
        }
        if (contador > grade[0][coluna]){
            return 0;
        }
    }
    if (linha == n){
        if(numero > maisAlto){
            contador += 1;
        }
        if (contador != grade[0][coluna]){
            return 0;
        }
    }

    // quando a linha estiver completa, verificar se o numero de predios vistos da direita para a esquerda esta certo
    if (coluna == n){
        contador = 1; maisAlto = numero;
        for (int c=n-1; c > 0; c--){
            if (grade[linha][c] > maisAlto){
                maisAlto = grade[linha][c];
                contador += 1;
            }
        }
        if (contador != grade[linha][n+1]){
            return 0;
        }
    }

    // quando a coluna estiver completa, verificar se o numero de predios vistos de baixo para cima esta certo
    if (linha == n){
        contador = 1; maisAlto = numero;
        for (int l=n-1; l > 0; l--){
            if (grade[l][coluna] > maisAlto){
                maisAlto = grade[l][coluna];
                contador += 1;
            }
        }
        if (contador != grade[n+1][coluna]){
            return 0;
        }
    }
    return 1;
}

int arranhaceuR(int n, int linha, int coluna, int **grade){
    if (linha > n){
        // acabou
        // impressao
        for (int l=1; l <= n; l++){
            for (int c=1; c <= n; c++){
                printf("%d ", grade[l][c]);
            }
            printf("\n");
        }
        return 1;
    }

    // calculo da proxima posicao
    int proxLinha, proxColuna;
    if (0 < coluna && coluna < n){
        proxLinha = linha;
        proxColuna = coluna + 1;
    } else {
        proxLinha = linha + 1;
        proxColuna = 1;
    }

    // preencher a posicao atual
    for (int i=1; i <= n; i++){
        if(podeInserir(n, linha, coluna, i, grade)){ // verifica se o numero pode ser inserido
            grade[linha][coluna] = i;
            if (arranhaceuR(n, proxLinha, proxColuna, grade)){ // chama a funcao arranhaceuR recursivamente para a proxima posicao
                return 1;
            }
        }
    }
    grade[linha][coluna] = 0;
    return 0;
}

void arranhaceu(int n){
    // realiza a alocacao da memoria de uma matriz de dimensao nxn, faz a leitura da grade inicial e a preenche, de maneira a resolve-la

    // alocacao da memoria
    int **grade;
    grade = (int**)calloc(n+2, sizeof(int*));
    for (int i=0; i < n+2; i++){
        grade[i] = (int*)calloc(n+2, sizeof(int));
    }

    // leitura e preenchimento
    for (int l=0; l < n+2; l++){
        for (int c=0; c < n+2; c++){
            scanf("%d ", &grade[l][c]);
        }
    }

    arranhaceuR(n, 1, 1, grade);

    // liberacao da memoria
    for (int i=0; i < n+2; i++){
        free(grade[i]);
    }
    free(grade);
}

int main(){
    // leitura inicial
    int n;
    scanf("%d ", &n);

    //chamada da funcao
    arranhaceu(n);

    return 0;
}