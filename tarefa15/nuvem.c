#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct No {
    char palavra[50]; 
    int vezes;
    struct No *prox;
} No;
typedef No * p_no;

typedef struct {
    p_no *vetor;
} Hash;
typedef Hash * p_hash;

//função que, caso dois hash coincidam, insere um nó na lista ligada desse hash
p_no inserir_lista (p_no no, char palavra[50], int vezes) {
    p_no novo = malloc(sizeof(No));
    novo->vezes = vezes;
    strcpy(novo->palavra, palavra);
    novo->prox = no;
    return novo;
}

//calcula o hash de uma palavra
int hash(char *palavra, int max) {
    int n = 0;
    for (int i = 0; i < strlen(palavra); i++)
        n = (256 * n + palavra[i]) % max;
    return n;
}

//procura um elemento em uma lista ligada
p_no procurar_elemento(p_no no, char *palavra) {
    p_no aux, elemento = NULL;
        aux = no;
    while(aux && strcmp(aux->palavra, palavra) !=0)
        aux = aux->prox;
    if (aux)
        elemento = aux;
    return elemento;
}

//cria uma tabela de hash
p_hash cria_hash (int max) {
    p_hash tabela_hash = malloc(sizeof(Hash));
    tabela_hash->vetor = malloc(max*sizeof(No));
    for (int i=0;i<max;i++) 
        tabela_hash->vetor[i] = NULL;
    return tabela_hash;
}

//insere um elemento na tabela de hash
void inserir(p_hash tabela_hash, char *palavra, int vezes, int max) {
    int n = hash(palavra, max);
    tabela_hash->vetor[n] = inserir_lista(tabela_hash->vetor[n], palavra, vezes);
}

//retira todos os caracteres não alfabéticos de uma string
void transforma_string (char string[50]) {
   for (int i = 0, j; string[i] != '\0'; ++i) { 
      while (!(string[i] >= 'a' && string[i] <= 'z') && !(string[i] >= 'A' && string[i] <= 'Z') && !(string[i] == '\0')) {
         for (j = i; string[j] != '\0'; ++j) {
            string[j] = string[j + 1];
         }
         string[j] = '\0';
      }
   }
}

//torn todos os caracteres de uma string minúsculos
void minusculo(char string[50]) {
    int i=0;
    while(string[i] != '\0'){
        string[i] = tolower(string[i]);
        i++;
    }
}

// funções que ordenam um vetor do menor número de ocorrências de uma palavra até o maior
void merge(p_no v, int l, int m, int r, int max) {
    p_no aux = malloc (max*sizeof(No));
    int i = l, j = m + 1, k = 0;
    while (i <= m && j <= r) {
        if (v[i].vezes < v[j].vezes)
            aux[k++] = v[i++];
        else if (v[i].vezes>v[j].vezes)
            aux[k++] = v[j++];
        else if (strcmp(v[i].palavra, v[j].palavra) < 0)
            //caso tenhamos duas palavras com a mesma ocorrência, colocamos a que vem antes em ordem alfabética no
            //final do vetor, pois esse será impresso de trás para frente
            aux[k++] = v[j++];
        else
            aux[k++] = v[i++];
    }
    /*copia o resto do subvetor que não terminou*/
    while (i <= m)
        aux[k++] = v[i++];
    while (j <= r)
        aux[k++] = v[j++];
    /*copia de volta para v*/
    for (i = l, k = 0; i <= r; i++, k++)
        v[i] = aux[k];
    free(aux);   
}

void mergesort(p_no v, int l, int r, int max) {
    int m = (l + r) / 2;
    if (l < r) {
        mergesort(v, l, m, max);
        mergesort(v, m + 1, r, max);
        merge(v, l, m, r, max);
    }
}

//destrói uma lista ligada
void destruir_lista(p_no lista) {
    if (lista != NULL) {
        destruir_lista(lista->prox);
        free(lista);
    }
}

//destrói um hash
void liberaHash(p_hash hash, int max) {
    int i;
    for (i=0;i<max;i++) {
        //destrói a lista ligada contida em cada posição do vetor do hash
        destruir_lista(hash->vetor[i]);
    }
    free(hash->vetor);
    free (hash);
}

//função que cria um vetor auxiliar com todos as palavras não stopwords presentes no hash
p_no cria_vetor_aux (int k, p_hash hash, int max) {
    p_no vetor_aux = malloc(k*(sizeof(No)));
    int n =0;
    for (int i=0;i<max;i++) {
        if(hash->vetor[i] != NULL) {
            p_no atual = hash->vetor[i];
            while (atual != NULL)  {
                if (atual->vezes>0) {
                    vetor_aux[n] = *atual;
                    n++;
                }
                atual = atual->prox;
            }
        }
    }
    return vetor_aux;
}

int main () {
    p_hash tabela_hash;
    int max, n_stopwords;
    char stopword[50], palavra[50];

    scanf("%d\n", &max);
    //é criado o hash com o número de posições max
    tabela_hash = cria_hash(max);

    scanf("%d", &n_stopwords);
    //todas as stopwords são lidas e as que tem tamanho > 1 são inseridas no hahs com ocorrência -1 (dummy)
    for (int i=0;i<n_stopwords;i++) {
        scanf(" %s", stopword);
        transforma_string(stopword);
        minusculo(stopword);
        if (strlen(stopword) > 1)
            inserir(tabela_hash, stopword, -1, max);
    }
    
    //k é o número de palavras diferentes que apareceram nas letra de música
    int k=0;
    while (scanf(" %s", palavra) != EOF){ 
        transforma_string(palavra);
        minusculo(palavra);
        if (strlen(palavra)>1) {
            //procuramos a palavra lida na tabela de hash
            p_no no_palavra = procurar_elemento(tabela_hash->vetor[hash(palavra, max)], palavra);
            //não existe na tabela (então também não é stopword, pois essas já foram adicionadas)
            if (no_palavra == NULL) {
                inserir(tabela_hash, palavra, 1, max);
                //como encontramos uma palavra nova, acrescemos o k
                k++;
            }
            //já existe na tabela e não é stopword (sua ocorrência não é -1)
            else if(no_palavra->vezes != -1) {
                //o número de vezes que a palavra apareceu é acrescido
                no_palavra->vezes++;   
            }
        }
    }
    //criamos o vetor auxiliar que conterá todas as palavras diferentes e suas ocorrências
    p_no vetor_aux = cria_vetor_aux(k,tabela_hash, max);
    //ordenamos o vetor
    mergesort(vetor_aux, 0, k-1, k);

    //como o vetor está ordenado crescentemente, printamos ele de trás para frente
    for (int i = k-1; i>k-51; i--)
        printf("%s %d\n", vetor_aux[i].palavra, vetor_aux[i].vezes);
        
    //liberamos as estruturas alocadas dinamicamente
    free (vetor_aux);
    liberaHash(tabela_hash, max);
    
    return 0;
}