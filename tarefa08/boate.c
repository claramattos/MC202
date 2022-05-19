#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    float j;
    float valor_parcela;
    int n_parcela;
    int qtd_parcelas_faltantes;
    struct No *prox;
} No;

typedef struct No * p_no;

p_no proximaParcela (p_no lista) {
    p_no novo, atual = lista;
    novo = malloc(sizeof(No));
    
    while (atual->prox != NULL)
        atual = atual->prox;
    
    novo->valor_parcela = atual->valor_parcela + atual->valor_parcela*lista->j;
    novo->n_parcela = atual->n_parcela + 1;
    atual->prox = novo;
    
    if (novo->valor_parcela == (int)novo->valor_parcela) 
        printf("%.0f ", novo->valor_parcela);
    else
        printf("%.2f ", novo->valor_parcela);
    
    lista->qtd_parcelas_faltantes--;
    return lista;
}

int main(){
    
    p_no vetor[100];
    int tudo_calculado = 0;
    
    for (i=0;i<100;i++)
        vetor[i] = NULL;
    
    int n=5, i;
    float j=0.01, s=200;
    
    // aqui seria feito um "for" mas, como devo alocar as informações dentro do próprio código, 
    //sem um scanf, adicionei as informações manualmente
    
    p_no novo;
    novo = malloc(sizeof(No));
    novo->prox = NULL;
    novo->j=j;
    novo->valor_parcela = s;
    novo->n_parcela = 1;
    novo->qtd_parcelas_faltantes = n-1;
    
    if (novo->valor_parcela == (int)novo->valor_parcela) 
        printf("%.0f ", novo->valor_parcela);
    else
        printf("%.2f ", novo->valor_parcela);
    
    for (i=0;i<100;i++) {
        if (vetor[i] == NULL) {
            vetor[i] = novo;
            break;
        }
    }
    
    n=7;
    j=0.02;
    s=500;
    
    p_no novo;
    novo = malloc(sizeof(No));
    novo->prox = NULL;
    novo->j=j;
    novo->valor_parcela = s;
    novo->n_parcela = 1;
    novo->qtd_parcelas_faltantes = n-1;
    
    if (novo->valor_parcela == (int)novo->valor_parcela) 
        printf("%.0f ", novo->valor_parcela);
    else
        printf("%.2f ", novo->valor_parcela);
    
    for (i=0;i<100;i++) {
        if (vetor[i] == NULL) {
            vetor[i] = novo;
            break;
        }
    }
    
    while (tudo_calculado != 0) {
        tudo_calculado = 0;
        for (i=0;i<100;i++) {
            if (vetor[i] == NULL)
                break;
            else if (vetor[i]->qtd_operadores_faltantes>0) {
                vetor[i] = proximaParcela(vetor[i]);
                tudo calculado++;
            }
        }
    }


    return 0;
}
