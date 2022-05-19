#include <stdio.h>
#include <stdlib.h>

//define-se o nó de um integrante da roda
typedef struct No {
    int altura;
    struct No *ant;
    struct No *prox;
} No;

typedef struct No * p_no;

//define-se a estrutura de uma lista dupla 
typedef struct Lista_dupla {   
    int tamanho; 
    p_no inicio;   
    p_no fim;     
} Lista_dupla;   

typedef struct Lista_dupla * p_dupla;

//função que cria uma lista circular dupla.
p_dupla criar_lista_circular () {
    p_dupla lista = malloc(sizeof(Lista_dupla));

    lista->tamanho = 0;
    lista->inicio = NULL;
    lista->fim = NULL;
    return lista;
}

//função que insere um integrante no início da roda
p_dupla inserir_circular (p_dupla lista, int x) {
    p_no novo = malloc(sizeof(No));
    novo->altura = x;

    if (lista->tamanho == 0) {
        novo->prox = novo;
        novo->ant = novo;
        lista->inicio = novo; 
        lista->fim = novo;
    } 
    else {
        novo->prox = lista->inicio;
        lista->inicio->ant = novo;
        novo->ant = lista->fim;
        lista->fim->prox = novo;
        lista->fim = novo;
    }
    lista->tamanho++;
    return lista;
}

//função quqe insere um integrante da roda em um time assim que ele sai da roda
p_dupla inserir_time (p_dupla time, p_no integrante){
    int k;
    p_no atual, novo;
    novo = malloc(sizeof(No));
    novo->altura = integrante->altura;

    if (time->tamanho == 0) {
        time->inicio = novo;
        time->fim = novo;
        novo->ant = novo;
        novo->prox = novo;
    }

    else {
        atual = time->inicio;
        for (k=0;k<time->tamanho;k++) {
            if (novo->altura < time->inicio->altura) {
                novo->ant = time->fim;
                time->fim->prox = novo;
                novo->prox = time->inicio;
                time->inicio->ant = novo;
                time->inicio = novo;
                break;
            }
            if (k == time->tamanho -1) {
                if (novo->altura < time->fim->altura) {
                    if (time->tamanho == 1) {
                        time->inicio = novo;
                        novo->prox = time->fim;
                        time->fim->ant = novo;
                        time->fim->prox=novo;
                        novo->ant = time->fim;
                        break;
                    }
                    else {
                        time->fim->ant->prox = novo;
                        novo->ant = time->fim->ant;
                        time->fim->ant = novo;
                        novo->prox = time-> fim;
                        break;
                    }
                }
                else {
                    time->fim->prox = novo;
                    novo->ant = time->fim;
                    novo->prox = time->inicio;
                    time->inicio->ant = novo;
                    time->fim = novo;
                    break;
                }
            }
            else if (atual->altura <= novo->altura && atual->prox->altura >= novo->altura ) {
                    atual->prox->ant = novo;
                    novo->prox = atual->prox;
                    atual->prox = novo;
                    novo->ant = atual;
                    break;
                }
            
            else
                atual = atual->prox;
        }
    }
    time->tamanho++;
    return time;
}

//função que imprime uma lista circular, usada para imprimir os times
void imprimir_lista_circular(p_no inicio) {
    p_no no = inicio;
    do {
        printf("%d ", no->altura);
        no = no->prox;
    } while (no != inicio);
}

//retira um integrante à esquerda da pessoa que joga o dado
p_dupla retira_esquerda (p_dupla lista, int n_dado, p_dupla time1){
    int k;
    p_no atual;
    atual = lista->inicio;
    for (k=1;k<n_dado;k++) {
        atual = atual->ant;
    }

    if (atual == lista->inicio){
        lista->inicio = (lista->inicio)->prox;
    }

    time1 = inserir_time(time1, atual);

    atual->ant->prox = atual->prox;
    atual->prox->ant = atual->ant;
    lista->tamanho-=1;
    
    free (atual);

    return lista;
}

//retira um integrante à direita da pessoa que joga o dado
p_dupla retira_direita(p_dupla lista, int n_dado, p_dupla time2){
    int k;
    p_no atual = lista->inicio;

    for (k=1;k<n_dado;k++) {
        atual = atual->prox;
    }

    if (atual == lista->inicio){
        lista->inicio = lista->inicio->prox;
    }

    time2 = inserir_time(time2, atual);
    
    atual->ant->prox = atual->prox;
    atual->prox->ant = atual->ant;
    lista->tamanho-=1;
    free (atual);
    return lista;
}

//função que destrói uma lista dupla circular
void destroi_lista (p_dupla lista){
    int k;
    p_no atual, auxiliar;
    auxiliar = lista->inicio;
    for (k=0; k<lista->tamanho;k++) {
        atual = auxiliar;
        auxiliar = atual->prox;
        free (atual);
    }
    free (lista);
}

int main() {
    int n_amigos, k, altura, n_dado;
    p_dupla lista, time1, time2;

    lista = criar_lista_circular ();
    time1 = criar_lista_circular ();
    time2 = criar_lista_circular ();

    scanf ("%d\n", &n_amigos);
    for (k=0;k<n_amigos;k++) {
        scanf("%d", &altura);
        lista = inserir_circular(lista, altura);
    }

    //são alternadas as funções a serem chamadas a cada dado jogado
    for (k=0;k<n_amigos;k++) {
        scanf("%d", &n_dado);
        if (k%2==0) 
            lista = retira_esquerda(lista, n_dado, time1);
        else if (k%2!=0)
            lista = retira_direita(lista, n_dado, time2);
    }

    imprimir_lista_circular(time1->inicio);
    printf ("\n");
    imprimir_lista_circular(time2->inicio);

    destroi_lista(time1);
    destroi_lista(time2);
    destroi_lista(lista);

    return 0;
}