#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id, prioridade, incremento;
} Item;

typedef struct {
    Item *v;
    int n, tamanho_max;
} FP;

typedef FP * p_fp;

#define PAI(i) ((i-1)/2)
#define F_ESQ(i) (2*i+1) /*Filho esquerdo de i*/
#define F_DIR(i) (2*i+2) /*Filho direito de i*/

void troca(Item *a, Item *b) {
    Item t = *a;
    *a = *b;
    *b = t;
}

//cria fila de prioridades
p_fp criar_filaprio(int tamanho_max) {
    p_fp fprio = malloc(sizeof(FP));
    fprio->v = malloc(tamanho_max * sizeof(Item));
    fprio->n = 0;
    fprio->tamanho_max = tamanho_max;
    return fprio;
}

//função que sobe "arrumando" a fila após uma inserção
void sobe_no_heap(p_fp fprio, int k) {
    if (k > 0 && fprio->v[PAI(k)].prioridade < fprio->v[k].prioridade) {
        troca(&fprio->v[k], &fprio->v[PAI(k)]);
        sobe_no_heap(fprio, PAI(k));
    }
}

//função que insere um novo pacote na fila
void insere(p_fp fprio, Item item) {
    //caso um pacote chegue depois da fila ja estar cheia, ele não é adicionado na fila
    if (fprio->n < fprio->tamanho_max) {
        fprio->v[fprio->n] = item;
        fprio->n++;
        sobe_no_heap(fprio, fprio->n - 1);
    }
}

//função que desce "arrumando" a fila
void desce_no_heap(p_fp fprio, int k) {
    int maior_filho;
    if (F_ESQ(k) < fprio->n) {
        maior_filho = F_ESQ(k);
        if (F_DIR(k) < fprio->n &&
            fprio->v[F_ESQ(k)].prioridade < fprio->v[F_DIR(k)].prioridade)
        maior_filho = F_DIR(k);
        if (fprio->v[k].prioridade < fprio->v[maior_filho].prioridade) {
            troca(&fprio->v[k], &fprio->v[maior_filho]);
            desce_no_heap(fprio, maior_filho);
        }
    }
}

//função que retira o item com maior prioridade da fila e o retorna
Item extrai_maximo(p_fp fprio) {
    Item item = fprio->v[0];
    troca(&fprio->v[0], &fprio->v[fprio->n - 1]);
    fprio->n--;
    desce_no_heap(fprio, 0);
    return item;
}

//função que muda a prioridade, adicionando o incremento de cada pacote após cada tick
void muda_prioridade(p_fp fprio, int k) {
    fprio->v[k].prioridade += fprio->v[k].incremento;
    sobe_no_heap(fprio, k);
}

int main () {
    int k, m, id, prioridade, incremento, i, n_tick = 0, tamanho;
    Item item, maximo;
    scanf ("%d %d", &k, &m);
    p_fp fprio = criar_filaprio(m);

    while (scanf("%d %d %d", &id, &prioridade, &incremento) != EOF){ 
        while (id != 0 || prioridade!=0 || incremento !=0){
            item.prioridade = prioridade;
            item.id = id;
            item.incremento = incremento;
            insere(fprio, item);
            //cria-se a fila 
            break;  
        }
        
        if (id == 0 && prioridade == 0 && incremento ==0) {
            n_tick++;
            printf ("TICK %d\n", n_tick);
            //caso existam mais elementos que podem ser enviados que elementos na fila
            //todos os elementos da fila são retirados
            if (fprio->n<=k) {
                tamanho= fprio->n;
                for (i=0;i<tamanho;i++) {
                    maximo = extrai_maximo(fprio);
                    printf ("%d %d %d\n", maximo.id, maximo.prioridade, maximo.incremento);
                }
            }
            //caso contrário, retiramos a quantidade de elementos que pode ser retirada
            else if (fprio->n>k) {
                for (i=0;i<k;i++) {
                    maximo = extrai_maximo(fprio);
                    printf ("%d %d %d\n", maximo.id, maximo.prioridade, maximo.incremento);
                }
                //mudamos a prioridade dos elementos restantes
                for (i=0;i<fprio->n;i++)
                    muda_prioridade(fprio, i);
            }
        }
    }
    //liberamos a memória
    free(fprio->v);
    free(fprio);
    return 0;
}