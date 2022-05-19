#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//estrutura de uma cidade
typedef struct cidade {
    char nome[50];
    double populacao;
    double centralidade;
} Cidade;

typedef Cidade * p_cidade;

//estrutura de um grafo
typedef struct Grafo{
    double **adj;
    int n;
} Grafo;

typedef Grafo * p_grafo;

//função que destrói um grafo
void destroi_grafo(p_grafo g) {
    int i;
    for (i = 0; i < g->n; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

//função que descobre se existe um caminho possível de uma cidade para outra
int existe_caminho(int n, double ** matriz_distancias, int indice_cidade1, int indice_cidade2) {
    if (matriz_distancias[indice_cidade1][indice_cidade2] != 50000) 
        return 1;
    return 0;
}

//função que insere uma aresta entre duas cidades no grafico de adjacencia equivalente à distância entre as duas
void insere_aresta(p_grafo g, int u, int v, double distancia) {
    g->adj[u][v] = distancia;
    g->adj[v][u] = distancia;
}

//função que cria um grafo
p_grafo criar_grafo(int n) {
    int i, j;
    p_grafo g = malloc(sizeof(Grafo));
    g->n = n;
    g->adj = malloc(n * sizeof(double *));
    for (i = 0; i < n; i++)
        g->adj[i] = malloc(n * sizeof(double));
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            if (i!=j)
                //caso tenhamos indices diferentes, adicionamos um valor dummy que indica que não existe caminho entre as duas cidades
                g->adj[i][j] = 50000;
            else
                //a distância de uma cidade até ela mesma é sempre 0
                g->adj[i][j] = 0;   
        }

    return g;
}

//função que calcula a menor distância possível entre duas cidades, seguindo o método de Floyd Warshall
void floydWarshall (int n, double** G, int max) {
    for(int k=0; k<n; k++)
        for(int i=0; i<n; i++)
            if( i!=k && G[i][k]<max )
                for(int j=0; j<n; j++)
                //caso encontre um caminho menor entre duas cidades que o previamente declarado, altera-o na matriz dada
                    if( G[i][j] > G[i][k]+G[k][j] )
                         G[i][j] = G[i][k]+G[k][j];                           
}

double calcula_fator_centralidade (int indice_cidade, p_cidade vetor_cidades, p_grafo grafo, double populacao_total, double**matriz_distancias) {
    double populacao_alcancada = 0, populacao_vezes_distancia = 0, fator_centralidade = 0;
 
    //calculamos a populacao que a cidade dada consegue ter acesso somando as populações de todas as cidades as quais existe caminho para a cidade dada
    //multiplicamos a população dessas cidades pela menor distância que estão da cidade dada (presente na matriz de distancias)
    for (int i=0;i<grafo->n;i++) {
        if (existe_caminho(grafo->n, matriz_distancias, indice_cidade, i) == 1) {
            populacao_alcancada+=vetor_cidades[i].populacao;
            populacao_vezes_distancia += vetor_cidades[i].populacao*matriz_distancias[i][indice_cidade];
        }   
    }
    //caso uma cidade esteja apta a ser capital, ou seja, tenha acesso a pelo menor metade da população total do estado, calculamos seu fator de centralidade
    if (populacao_alcancada >= populacao_total/2) {
        fator_centralidade = populacao_vezes_distancia/populacao_alcancada;
    }
    //retornamos o fator de centralidade (que pode ser 0, caso uma cidade não esteja apta a ser capital)
    return fator_centralidade;
}

// funções que ordenam um vetor do menor número de fator de centralidade até o maior
void merge(p_cidade v, int l, int m, int r, int max) {
    p_cidade aux = malloc (max*sizeof(Cidade));
    int i = l, j = m + 1, k = 0;
    while (i <= m && j <= r) {
        if (v[i].centralidade < v[j].centralidade)
            aux[k++] = v[i++];
        else if (v[i].centralidade>v[j].centralidade)
            aux[k++] = v[j++];
        else if (strcmp(v[i].nome, v[j].nome) < 0)
            //caso tenhamos duas cidades com o mesmo fator de centralidade, colocamo-as em ordem alfabética dentro do vetor
            aux[k++] = v[i++];
        else
            aux[k++] = v[j++];
    }
    //copia o resto do subvetor que não terminou
    while (i <= m)
        aux[k++] = v[i++];
    while (j <= r)
        aux[k++] = v[j++];
    //copia de volta para v
    for (i = l, k = 0; i <= r; i++, k++)
        v[i] = aux[k];

    //liberamos o vetor auxiliar criado
    free (aux);
}

void mergesort(p_cidade v, int l, int r, int max) {
    int m = (l + r) / 2;
    if (l < r) {
        mergesort(v, l, m, max);
        mergesort(v, m + 1, r, max);
        merge(v, l, m, r, max);
    }
}

int main () {
    int n, i, indice1, indice2, k=0;
    double distancia, populacao_total = 0;
    char nome[50], cidade1[50], cidade2[50];
    p_grafo grafo;

    scanf("%d", &n);
    //criamos o grafo com o número de cidades do estado
    grafo = criar_grafo(n);
    grafo->n = n;

    //vetor que armazernará todas as cidades
    p_cidade vetor_cidades = malloc (n*sizeof(Cidade));
    //vetor que armazenará as capitais potenciais do estado (tem acesso a mais de metade da população)
    p_cidade potenciais_capitais = malloc (n*sizeof(Cidade));

    //adicionamos todas as cidades lidas no vetor de cidades
    for (i=0;i<n;i++) {
        scanf ("%s %lf", nome, &vetor_cidades[i].populacao);
        strcpy(vetor_cidades[i].nome, nome);
    }

    //lemos todas as esrtadas existentes e inserimos uma aresta entre elas
    while (scanf("%s %s %lf", cidade1, cidade2, &distancia) != EOF){ 

        for (i=0;i<n;i++) {
            if (strcmp (vetor_cidades[i].nome, cidade1) == 0) {
                indice1 = i;
                break;
            }
        }

        for (i=0;i<n;i++) {
            if (strcmp (vetor_cidades[i].nome, cidade2) == 0) {
                indice2 = i;
                break;
            }
        }

        if (indice1 != indice2)
            //caso não estivermos adicionando uma distância de uma cidade até ela mesma (pois o valor sempre será 0)
            insere_aresta(grafo, indice1, indice2, distancia);
    }
    
    //descobrimos o valor da população do estado somando a população de todas as cidades
    for (i=0;i<n;i++) {
        populacao_total += vetor_cidades[i].populacao;
    }

    //criamos uma matriz que terá as menores distâncias entre duas cidades
    double ** matriz_distancias;
    matriz_distancias = malloc(grafo->n * sizeof(double *));
    for (int i = 0; i < grafo->n; i++)
        matriz_distancias[i] = malloc(grafo->n * sizeof(double));
    for (int k=0;k<grafo->n;k++)
        for (int i=0;i<grafo->n;i++)
            matriz_distancias[k][i] = grafo->adj[k][i];

    //calculamos todas as menores distâncias de uma cidade até outra nessa matriz
    floydWarshall(grafo->n, matriz_distancias, 50000);
    
    //calculamos o fator de centralidade de todas as cidades do estado
    for (i=0;i<n;i++) {
        vetor_cidades[i].centralidade = calcula_fator_centralidade (i, vetor_cidades, grafo, populacao_total, matriz_distancias);
            if (vetor_cidades[i].centralidade != 0) {
                //caso a cidade seja uma potencial capital (tenha acesso a mais da metade da população), adicionamos no vetor correspondente
                potenciais_capitais[k] = vetor_cidades[i];
                k++;
            }
    }

    //colocamos o vetor de potenciais capitais em ordem crescente de acordo com seus fatores de centralidade
    mergesort(potenciais_capitais, 0, k-1, k);
    for (i=0;i<k;i++) {
        printf ("%s %.2lf\n", potenciais_capitais[i].nome, potenciais_capitais[i].centralidade);
    }

    //liberamos  a memória alocada
    free(vetor_cidades);
    free(potenciais_capitais);
    destroi_grafo(grafo);
    for (i=0;i<n;i++)
        free (matriz_distancias[i]);
    free (matriz_distancias);

    return 0;
}