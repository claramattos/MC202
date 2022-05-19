#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//struct de um navio ou uma mercadoria em uma doca
typedef struct No {
    char nome[21];
    char carrega_descarrega[15];
    char mercadoria[21];
    int n_conteiners;
    struct No *prox;
} No;

typedef struct No * p_no;

//struct de uma fila
typedef struct {
    p_no ini, fim;
} Fila;

typedef Fila * p_fila;

//struct de uma pilha
typedef struct {
    p_no topo;
    int capacidade;
} Pilha;

typedef Pilha * p_pilha;

//função que destrói uma lista ligada
void destruir_lista (p_no no) {
    if(no != NULL){
        p_no prox = no->prox;
        free(no);
        destruir_lista(prox);
    }
}

//função que cria uma fila
p_fila criar_fila() {
    p_fila f;
    f = malloc(sizeof(Fila));
    f->ini = NULL;
    f->fim = NULL;
    return f;
}

//função que cria uma pilha
p_pilha criar_pilha(int capacidade_doca) {
    p_pilha p;
    p = malloc(sizeof(Pilha));
    p->topo = NULL;
    p->capacidade = capacidade_doca;
    return p;
}

//função que insere um nó no final de uma fila
void enfileira(p_fila f, p_no novo) {
    novo->prox = NULL;
    if (f->ini == NULL)
        f->ini = novo;
    else
        f->fim->prox = novo;
    f->fim = novo;
}

//função que retira um nó do início de uma fila
void desenfileira(p_fila f) {
    f->ini = f->ini->prox;
    if (f->ini == NULL)
        f->fim = NULL;
}

//função que adiciona um nó no topo de uma pilha
void empilhar(p_pilha pilha, char mercadoria[21]) {
    p_no novo = malloc(sizeof(No));
    strcpy(novo->mercadoria, mercadoria);
    novo->prox = pilha->topo;
    pilha->topo = novo;
}

//função que retira um nó do topo de uma pilha e o libera
void desempilhar(p_pilha pilha) {
    p_no topo = pilha->topo;
    pilha->topo = pilha->topo->prox;
    free(topo);
}

//função que, dada uma lista ligada, calcula o número de elementos nela
int calcula_tamanho_fila (p_fila fila) {
    p_no no  = fila->ini;
    int tamanho = 0;
    while (no != NULL) {
        tamanho++;
        no = no->prox;
    }
    return tamanho;
}

int main () {
    int n_docas, n_conteiners, capacidade_doca, n_navios, i;
    char nome[21], carega_descarrega[15], mercadoria[21];

    //lemos as informações iniciais
    scanf ("%d %d %d", &n_docas, &capacidade_doca, &n_navios);

    //criamos um vetor de pilhas
    //ele terá número de elementos igual ao número de docas e cada elemento será uma pilha
    p_pilha* docas  = malloc(n_docas*sizeof(p_pilha));
    for (i=0;i<n_docas;i++)
        docas[i] = criar_pilha(capacidade_doca);

    //criamos a fila de navios
    p_fila fila = criar_fila();
    //adicionamos todos os navios da entrada na fila
    for (int i=0;i<n_navios;i++) {
        p_no novo = malloc (sizeof(No));
        scanf("%s %s %s %d", nome, carega_descarrega, mercadoria, &n_conteiners);
        novo->n_conteiners = n_conteiners;
        strcpy(novo->carrega_descarrega, carega_descarrega);
        strcpy(novo->mercadoria, mercadoria);
        strcpy(novo->nome, nome);
        enfileira(fila, novo);
    }

    p_no atual = fila->ini;
    //o número contador indicará a quantos elementos da lista não empilhamos os desempilhamos
    //se contador chegar ao mesmo números de navios na fila, significa que já percorremos toda
    //a fila e não conseguimos realizar nenhuma operação. 
    int contador = 0;
    //enquanto não chegarmos ao final da fila ou não tivermos mais o que fazes com os navios do porto, empilhamos
    //ou desempilhamos mercadorias
    while (atual != NULL && contador < calcula_tamanho_fila(fila)) {
        p_no prox = atual->prox;

        //descarregamos um navio na primeira doca que tenha capacidade para pelo menos uma mercadoria
        //é realizado até que a doca não tenha mais capacidade ou o navio não tenha mais conteineres
        if(strcmp (atual->carrega_descarrega, "descarrega") == 0) {
            for (i=0;i<n_docas;i++) {
                int achou_doca_descarregar =0, k=0;
                while (docas[i]->capacidade>0 && atual->n_conteiners>0) {
                    strcpy (mercadoria, atual->mercadoria);
                    empilhar(docas[i], mercadoria);
                    docas[i]->capacidade--;
                    atual->n_conteiners--;
                    //k recebe o número de navios descarregados
                    k++;
                    achou_doca_descarregar = 1;
                }
                //printamos a operação realizada
                if(achou_doca_descarregar==1) {
                    printf ("%s descarrega %s doca: %d conteineres: %d\n", atual->nome, atual->mercadoria, i, k);
                    //como realizamos uma operação, o contador torna a ser 0
                    contador = 0;
                    //caso o navio ainda tenha conteineres não descarregados, o enviamos para o fim da fila
                    if (atual->n_conteiners>0) {
                        desenfileira(fila);
                        enfileira(fila, atual);
                    }
                    //caso contrário, liberamos o navio
                    else {
                        desenfileira(fila);
                        free(atual);
                    }
                    break;
                }
                //caso estivermos na última doca e ainda não tenhamos conseguido descarregar nenhuma mercadoria, 
                //enviamos o navio de volta ao início da fila e aumentamos o contador
                if (i == n_docas-1 && achou_doca_descarregar!=1) {
                    desenfileira(fila);
                    enfileira(fila, atual);
                    contador++;
                }
            }
        }

        //carregamos o navio na primeira doca que tenha a mercadoria necessitada
        //é realizado até que a doca não tenha mais a merrcadoria no topo ou até que o navio não precise mais dos conteineres
        else if(strcmp (atual->carrega_descarrega, "carrega") == 0) {
            for (i=0;i<n_docas;i++) {
                int achou_doca_carregar = 0, n=0;
                if (docas[i]->topo != NULL) {
                    strcpy (mercadoria, atual->mercadoria);
                    while (strcmp (docas[i]->topo->mercadoria, mercadoria) == 0 && atual->n_conteiners>0) {
                        strcpy (mercadoria, atual->mercadoria);
                        desempilhar(docas[i]);
                        atual->n_conteiners--;
                        docas[i]->capacidade++;
                        //n recebe o número de mercadorias carregadas
                        n++;
                        achou_doca_carregar = 1;
                        //caso não exista mais mercadorias na doca, encerramos a operação
                        if (docas[i]->topo == NULL)
                            break;
                    }
                    //printamos a operação realizada
                    if (achou_doca_carregar == 1) {
                        printf ("%s carrega %s doca: %d conteineres: %d\n", atual->nome, atual->mercadoria, i, n);
                        //como realizamos uma operação, o contador volta a ser 0
                        contador = 0;
                        //caso o navio ainda necessite mais daquela mercadoria, colocamos ele no final da fila
                        if (atual->n_conteiners > 0) {
                            desenfileira(fila);
                            enfileira(fila, atual);
                        }
                        //caso contrário, liberamos o navio
                        else {
                            desenfileira(fila);
                            free(atual);
                        }
                        break;
                    }
                }
                //caso estivermos na última doca e o navio ainda não tiver encontrado a mercadoria necessitada, colocamos ele
                //no final da fila e aumentamos o contador
                if (i == n_docas-1 && achou_doca_carregar!=1) {
                    desenfileira(fila);
                    enfileira(fila, atual);
                    contador++;
                }
            }
        }
        //realizaremos as mesmas operações com o próximo navio da fila
        atual = prox;
    }
    
    //caso não tenhamos conseguido realizar as necessidades de todos os navios, isto é, visitamos a fila não vazia
    //inteira e não tivemos nenhuma operação sendo realizada
    if (contador != 0) {
        printf ("ALERTA: impossivel esvaziar fila, restam %d navios.", contador);
    }

    //a memória é liberada
    destruir_lista(fila->ini);
    for (i=0;i<n_docas;i++) {
        destruir_lista(docas[i]->topo);
        free(docas[i]);
    }
    free(docas);
    free(fila);

    return 0;
}
