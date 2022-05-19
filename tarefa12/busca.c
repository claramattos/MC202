#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    char nome[30];
    int codigo;
    double preco;
    struct No *esq;
    struct No *dir;
} No;

typedef struct No * p_no;

//função que busca um elemento na árvore
p_no buscar(p_no raiz, int codigo) {
    if (raiz == NULL || codigo == raiz->codigo)
        return raiz;
    if (codigo < raiz->codigo)
        return buscar(raiz->esq, codigo);
    else
        return buscar(raiz->dir, codigo);
}

//função que insere um elemento na árvore baseado no código dele
p_no inserir(p_no raiz, char nome[30], int codigo, double preco) {
    p_no novo;
    if (raiz == NULL) {
        novo = malloc(sizeof(No));
        novo->esq = novo->dir = NULL;
        strcpy(novo->nome,nome);
        novo->codigo = codigo;
        novo->preco = preco;
        return novo;
    }
    if (codigo < raiz->codigo)
        raiz->esq = inserir(raiz->esq, nome, codigo, preco);
    else if (codigo > raiz->codigo){
        raiz->dir = inserir(raiz->dir, nome, codigo, preco);
    }
    //caso já exista o código a ser inserido na árvore, o próximo a ser inserido será ordenado a
    //partir da ordem alfabética do nome da loja
    else if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = inserir(raiz->esq, nome, codigo, preco);
    else
        raiz->dir = inserir(raiz->dir, nome, codigo, preco);

    return raiz;
}

//função que printa as ofertas disponíveis para determinado produto
int printa_inordem(p_no raiz, int codigo, double preco, int *existem_ofertas) {
    if (raiz != NULL) {
        printa_inordem(raiz->esq, codigo, preco, existem_ofertas);
        if (raiz->codigo == codigo && raiz->preco <= preco *1.1) {
            printf("%s %d %.2lf\n", raiz->nome, raiz->codigo, raiz->preco);
            *existem_ofertas = 1;
        }
        printa_inordem(raiz->dir, codigo, preco, existem_ofertas);
    }
    return *existem_ofertas;
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
    int numero_entradas, k, codigo, numero_procuras;
    double preco;
    char nome[31];
    p_no no, raiz = NULL;

    scanf("%d", &numero_entradas);
    for (k=0;k<numero_entradas;k++) {
        scanf("%s %d %lf", nome, &codigo, &preco);
        raiz = inserir(raiz, nome, codigo, preco);
    }

    scanf ("%d", &numero_procuras);
    for (k=0;k<numero_procuras;k++) {
        int existem_ofertas = 0;
        scanf("%d %lf", &codigo, &preco);

        //"nó" recebe o primeiro nó encontrado com o código desejado pois, a partir daí, todos os 
        //nós com código igual estarão obrigatoriamente abaixo dele
        no = buscar (raiz, codigo);
        printf ("OFERTAS para %d:\n", codigo);

        //como printa_inordem retorna 0 se não existirem ofertas para determinado produto:
        if (printa_inordem(no, codigo, preco, &existem_ofertas) == 0)
            printf("nenhuma boa oferta foi encontrada\n");
        printf ("\n");

    }
    destroi_arvore(raiz);
    return 0;
}