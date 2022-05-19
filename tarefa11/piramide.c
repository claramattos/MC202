#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    char nome[20];
    double investimento;
    int nivel;
    struct No *esq;
    struct No *dir;
    struct No *pai;
} No;

typedef struct No * p_no;

//função chamada sempre que um novo nó é adicionado na árvore
p_no criar_arvore(char nome[20], double investimento, p_no esq, p_no dir, p_no pai, int *niveis) {
    p_no no = malloc(sizeof(No));

    strcpy(no->nome, nome);
    no->investimento = investimento;
    no->dir = dir;
    no->esq = esq;
    no->pai = pai;

    //é dada a bonificação ao investimento inicial quando um novo membro entra na pirâmide
    if (no->pai != NULL) {
        no->pai->investimento -= 0.1*no->investimento;
        no->investimento = 1.1*no->investimento; 
        no->nivel = no->pai->nivel+1;
    }
    else
        no->nivel = 1;

    //é guardado o número de níveis da árvore para quando formos imprimir
    if (no->nivel > *niveis)
        *niveis = no->nivel;
    return no;
}

//função que procura o recrutador citado na árvore
p_no procurar_no(p_no raiz, char nome[20]){
    p_no esq;
    if (raiz == NULL || strcmp(nome, raiz->nome)==0)
        return raiz;
    esq = procurar_no(raiz->esq, nome);
    if (esq != NULL)
        return esq;
    return procurar_no(raiz->dir, nome);
}

//função que remunera os 6 níveis da pirâmide quando um participante recruta duas pessoas
void remunera(p_no esq, p_no dir, double porcentagem) {
    if (strcmp(esq->nome, dir->nome) != 0) {
        porcentagem = 0.06;

        esq->pai->investimento += esq->investimento*porcentagem;
        dir->pai->investimento += dir->investimento*porcentagem;

        esq->investimento -= esq->investimento*porcentagem;
        dir->investimento -= dir->investimento*porcentagem;

        porcentagem-=0.01;
        remunera(dir->pai, dir->pai, porcentagem);
    }
    else if (esq->pai == NULL)
        return;
    else if (porcentagem != 0.00) {
        esq->pai->investimento += esq->investimento*porcentagem;
        esq->investimento-= esq->investimento*porcentagem;
        porcentagem-=0.01;
        remunera(esq->pai, esq->pai, porcentagem);
    }
    return;
}

//função que imprime todos os elementos que possuem o mesmo nível da pirâmide
void imprime_niveis(p_no raiz, int nivel, int x) {
    if (raiz==NULL)
        return;
    if (x==1)
        printf("Nivel %d: ", nivel);
    if (nivel == 1)
        printf("[%s %.2lf] ", raiz->nome, raiz->investimento);
    else if (nivel > 1) {
        imprime_niveis(raiz->esq, nivel-1, 0);
        imprime_niveis(raiz->dir, nivel-1, 0);
    }
}

//função responsável por chamar a função imprime_niveis corretamente
void imprime_arvore(p_no raiz, int niveis) {
    int h = niveis, i;
    for (i=1;i<=h;i++) {
        imprime_niveis(raiz,i, 1);
        printf("\n");
    }
}

//função que destrói uma árvore
void destroi_arvore(p_no raiz) {
    if (raiz != NULL) {
        destroi_arvore(raiz->esq);
        destroi_arvore(raiz->dir);
        free (raiz);
    }
}

int main() {
    char nome[20], recrutador[20];
    double investimento;
    int niveis = 1;
    p_no raiz, no;

    scanf ("Topo %s %lf", nome, &investimento);
    raiz = criar_arvore(nome, investimento, NULL, NULL, NULL, &niveis);
    imprime_arvore(raiz, niveis); 
    printf("\n");

    while (scanf("%s recruta %s %lf", recrutador, nome, &investimento) != EOF) {
        no = procurar_no(raiz, recrutador);
        if (no->esq == NULL) 
            no->esq = criar_arvore(nome, investimento, NULL, NULL, no, &niveis);
        else if (no->dir == NULL) {
            no->dir = criar_arvore(nome, investimento, NULL, NULL, no, &niveis);
            remunera (no->esq, no->dir, 0);
        }
        imprime_arvore(raiz, niveis);  
        printf("\n");
    }
    destroi_arvore(raiz);
    return 0;
}