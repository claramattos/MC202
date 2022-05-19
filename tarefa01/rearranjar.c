#include <stdio.h>

int conversa (int fila[], int n_pessoas) {
    int primeiro_conversa, ultimo_conversa, posicao, indice, k, indice_primeiro_conversa, indice_ultimo_conversa;
    scanf ("%d", &primeiro_conversa);
    scanf("%d", &ultimo_conversa);
    for (k=0;k<n_pessoas; k++) {
        if (fila[k]==primeiro_conversa)
            indice_primeiro_conversa = k;
        if (fila[k]==ultimo_conversa)
            indice_ultimo_conversa =k;
    }
    for (indice=indice_primeiro_conversa; indice<indice_ultimo_conversa; indice++) {
        posicao = fila[indice_ultimo_conversa];
        fila[indice_ultimo_conversa] = fila[indice];
        fila[indice] = posicao;
        indice_ultimo_conversa--;
    }
    return 0;
}

int espera (int fila[], int n_pessoas) {
    int quem_espera,esperado, indice_quem_espera, indice_esperado, k;
    scanf ("%d", &quem_espera);
    scanf("%d", &esperado);
    for (k=0;k<n_pessoas; k++) {
        if (fila[k]==quem_espera)
            indice_quem_espera = k;
        if (fila[k]==esperado)
            indice_esperado = k;
    }
    for (k=indice_quem_espera; k<indice_esperado;k++) {
        fila[k] = fila[k+1];


    }
    fila[indice_esperado] = quem_espera;
    return 0;
} 


int main () {
    int n_pessoas, i, fila [10000], n_mudancas, tipo_mudanca;
    scanf("%d\n", &n_pessoas);
    for (i=0; i<n_pessoas; i++) {
        fila[i] = i + 1;
    }
    scanf("%d\n", &n_mudancas);
    for (i=1; i<=n_mudancas;i++) {
        scanf("%d", &tipo_mudanca);
        if (tipo_mudanca == 1)
            conversa(fila, n_pessoas);
        if (tipo_mudanca == 2)
            espera(fila, n_pessoas);
    }


    
    for (i=0; i<n_pessoas; i++) {
        printf ("%d ", fila[i]);
    }
    return 0;

}