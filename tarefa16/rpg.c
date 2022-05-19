#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//estrutura de um elemento de rpg (cela e jogador)
typedef struct elemento_rpg {
    char nome[50];
    int forca, destreza, constituicao, inteligencia, sabedoria, carisma, vizinho;
} elemento_rpg;

typedef elemento_rpg * p_elemento;

//estrutura de um grafo
typedef struct Grafo{
    elemento_rpg **adj;
    int jogadores;
    int celas;
} Grafo;

typedef Grafo * p_grafo;

//função que destrói um grafo
void destroi_grafo(p_grafo g) {
    int i;
    for (i = 0; i < g->celas+1; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

//função que insere uma aresta em uma posição de um grafo
void insere_aresta(p_grafo g, int u, int v) {
    g->adj[u][v].vizinho = 1;
}

//função que monta um grafo com as entradas
p_grafo le_e_monta_grafo() {
    p_grafo g = malloc(sizeof(Grafo));

    char nome[50];
    int n_celas, n_jogadores;

    scanf ("%d", &n_celas);
    g->celas = n_celas;
    //é criado um vetor com todas as celas que constam no rpg com tamanho n_celas+1
    //a primeira posição desse vetor é deixada em branco
    p_elemento celas = malloc((n_celas + 1) *sizeof(elemento_rpg));
    for (int i =1;i<n_celas+1;i++) {
        scanf (" %s", nome);
        strcpy(celas[i].nome, nome);
        scanf ("%d %d %d %d %d %d", &celas[i].forca, &celas[i].destreza, &celas[i].constituicao, &celas[i].inteligencia, &celas[i].sabedoria, &celas[i].carisma);  
    }

    scanf ("%d", &n_jogadores);
    g->jogadores = n_jogadores;
    //é criado um vetor com todos os jogadores do rpg com tamanho n_jogadores+1
    //a primeira posição desse vetor é deixada em branco
    p_elemento jogadores = malloc ((n_jogadores+1)*sizeof(elemento_rpg));
    for (int i =1;i<n_jogadores+1;i++) {
        scanf ("%s", nome);
        strcpy(jogadores[i].nome, nome);
        scanf ("%d %d %d %d %d %d", &jogadores[i].forca, &jogadores[i].destreza, &jogadores[i].constituicao, &jogadores[i].inteligencia, &jogadores[i].sabedoria, &jogadores[i].carisma);  
    }

    //uma vez que temos ambas as dimensões de nosso grafo, podemos construí-lo
    //suas dimensões serão n_celas+1 * n_jogadores+1
    g->adj = malloc((n_celas + 1)* sizeof(elemento_rpg *));
    for (int i = 0; i < n_celas+1; i++)
        g->adj[i] = malloc((n_jogadores+1) * sizeof(elemento_rpg));
    
    //adicionaremos na primeira coluna desse grafo as celas armazenadas no vetor anteriormente criado
    for (int i=1;i<n_celas+1;i++)
        g->adj[i][0] = celas[i];
    //adicionaremos na primeira linha desse grafo os jogadores armazenados no vetor anteriormente criado
    for (int j=1;j<n_jogadores+1;j++)
        g->adj[0][j] = jogadores[j];

    //inicializamos todos os valores das celas quenão contem os vértices como 0, e nesses elementos utilizaremos apenas a chave "vizinho".
    for (int i=1;i<g->celas+1;i++) {
        for (int j=1; j<g->jogadores+1;j++) 
            g->adj[i][j].forca = g->adj[i][j].carisma = g->adj[i][j].constituicao = g->adj[i][j].destreza = g->adj[i][j].inteligencia = g->adj[i][j].sabedoria = g->adj[i][j].vizinho = 0;
        
    }
    //liberamos os vetores auxiliares criados e é retornado o grafo
    free(jogadores);
    free(celas);
    return g;
}

p_grafo adiciona_vizinhanca (p_grafo g) {
    int i, j;
    //percorremos todos os elementos não cela/jogador do grafo
    for (i=1;i<g->celas+1;i++) {
        for (j=1;j<g->jogadores+1;j++) {
            int diferenca = 0;
            //para cada elemento, calculamos a diferença entre os atributos do jogador presente na posição 0
            //da coluna e os atributos da cela da posição 0 da linha. Caso essa diferença seja menor ou igual a 2, 
            //o número de rodadas para que ele tenha acesso à cela será no máximo 2, e então inserimos uma aresta na posição inspecionada.
            if (g->adj[i][0].forca>g->adj[0][j].forca)
                diferenca = g->adj[i][0].forca - g->adj[0][j].forca;
            if (g->adj[i][0].destreza>g->adj[0][j].destreza)
                diferenca = diferenca + g->adj[i][0].destreza - g->adj[0][j].destreza;
            if (g->adj[i][0].constituicao>g->adj[0][j].constituicao)
                diferenca = diferenca + g->adj[i][0].constituicao - g->adj[0][j].constituicao;
            if (g->adj[i][0].inteligencia>g->adj[0][j].inteligencia)
                diferenca = diferenca + g->adj[i][0].inteligencia - g->adj[0][j].inteligencia;
            if (g->adj[i][0].sabedoria>g->adj[0][j].sabedoria)
                diferenca = diferenca + g->adj[i][0].sabedoria - g->adj[0][j].sabedoria;
            if (g->adj[i][0].carisma>g->adj[0][j].carisma)
                diferenca = diferenca + g->adj[i][0].carisma - g->adj[0][j].carisma;
            if (diferenca <= 2)
                insere_aresta(g, i, j);
        }
    }
    return g;
}

//função que checa quais celas estão aptas a serem a solução do rpg
void checa_e_imprime (p_grafo g) {
    //existe_sala é inicializado como falso
    int i, j, existe_sala = 0;
    for (i=1;i<g->celas+1;i++) {
        //para cada linha, utilizaremos a variável jogadores_com_acesso para verificar quantos jogadores são adjacentes à cela inspecionada
        //inicializamos a variável como 0
        int jogadores_com_acesso = 0;
        for (j=1; j<g->jogadores+1;j++) {
            //como vizinho é inicializado como 0 e recebe 1 caso há adjacencia, somamos todos os seus valores em uma linha
            jogadores_com_acesso += g->adj[i][j].vizinho;
        }
        //caso a soma seja igual ao número de jogadores, todos os jogadores daquela linha são adjacentes a cela da linha.
        //assim, a cela é uma possível solução para o rpg. printamos a cela e mudamos existe_sala para verdadeiro
        if (jogadores_com_acesso == g->jogadores) {
            printf ("%s\n", g->adj[i][0].nome);
            existe_sala = 1;
        }
    }
    //caso não exista salas possíveis como solução, avisamos o usuário
    if (existe_sala == 0)
        printf ("Impossivel terminar em duas rodadas.");
}

int main () {
    //inicializamos e montamos o grafo
    p_grafo g = le_e_monta_grafo();
    //inserimos as vizinhancas na tabela de adjacencias
    g = adiciona_vizinhanca(g);
    //imprimimos as soluções possíveis
    checa_e_imprime(g);
    //destruímos o grafo
    destroi_grafo(g);
    return 0;
}