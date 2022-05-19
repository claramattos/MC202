#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Cor {VERMELHO , PRETO};

//struct usada para ips e para dominios
typedef struct No {
    char dominio_ou_ip[101];
    char servidor[17];
    int visitas;
    enum Cor cor;
    struct No *esq;
    struct No *dir;
} No;

typedef struct No * p_no;

//funções padrão para árvores rubro negras esquerdistas
int ehVermelho(p_no x) {
    if (x == NULL)
    return 0;
    return x->cor == VERMELHO;
}
int ehPreto(p_no x) {
    if (x == NULL)
    return 1;
    return x->cor == PRETO;
}
p_no rotaciona_para_esquerda(p_no raiz) {
    p_no x = raiz->dir;
    raiz->dir = x->esq;
    x->esq = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return x;
}
p_no rotaciona_para_direita(p_no raiz) {
    p_no x = raiz->esq;
    raiz->esq = x->dir;
    x->dir = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return x;
}
void sobe_vermelho(p_no raiz) {
    raiz->cor = VERMELHO;
    raiz->esq->cor = PRETO;
    raiz->dir->cor = PRETO;
}

//função padrão de inserção em uma árvore rubro negra esquerdista
p_no inserir_rec(p_no raiz, char chave[101], char servidor [17], int x) {
    p_no novo;
    if (raiz == NULL) {
        novo = malloc(sizeof(No));
        novo->esq = novo->dir = NULL;
        strcpy (novo->dominio_ou_ip, chave);
        //passaremos o servidor como nulo quando chamarmos a função para inserção de um id
        if (servidor != NULL )
            strcpy(novo->servidor, servidor);
        //passaremos x=0 para a inserção de um ip quando o usuário for mencionado pela primeira vez, e x=NULL quando passarmos a função para adicionarmos domínios
        if (x == 0)
            novo->visitas = 1;
        novo->cor = VERMELHO;
        return novo;
    }
    if  (strcmp(chave, raiz->dominio_ou_ip) <0)
        raiz->esq = inserir_rec(raiz->esq, chave, servidor,x);
    else
        raiz->dir = inserir_rec(raiz->dir, chave, servidor,x);

    if (ehVermelho(raiz->dir) && ehPreto(raiz->esq))
        raiz = rotaciona_para_esquerda(raiz);
    if (ehVermelho(raiz->esq) && ehVermelho(raiz->esq->esq))
        raiz = rotaciona_para_direita(raiz);
    if (ehVermelho(raiz->esq) && ehVermelho(raiz->dir))
        sobe_vermelho(raiz);
    return raiz;
}

//função padrão de busca de um nó em uma árvore dde busca
p_no procurar_no(p_no raiz, char chave[101]) {
    if (raiz == NULL || strcmp(chave, raiz->dominio_ou_ip)==0)
        return raiz;
    if (strcmp (chave, raiz->dominio_ou_ip) <0)
        return procurar_no(raiz->esq, chave);
    else
        return procurar_no(raiz->dir, chave);
}

//função que destrói uma árvore
void destroi_arvore(p_no raiz) {
    if (raiz != NULL) {
        destroi_arvore(raiz->esq);
        destroi_arvore(raiz->dir);
        free (raiz);
    }
}

int main () {
    int consultas_permitidas, n_entradas, k;
    char dominio [101], endereco[17];
    p_no raiz_arvore_dominio = NULL, raiz_arvore_ip = NULL, usuario, p_dominio;

    scanf ("%d", &consultas_permitidas);
    scanf ("%d", &n_entradas);

    for (k=0;k<n_entradas;k++) {
        scanf ("%s %s", dominio, endereco);
        //criamos uma árvore com os domínios desejados
        raiz_arvore_dominio = inserir_rec(raiz_arvore_dominio, dominio, endereco,0);
    }

    scanf ("%d", &n_entradas);
    for (k=0;k<n_entradas;k++) {
        scanf (" GET %s FROM %s", dominio, endereco);

        //procuramos os nós referentes ao usuário e ao domínio mencionados
        //vale ressaltar que existem duas árvores, uma de domínios e outra de usuários, 
        //apesar do código de criação dea árvore de ips ainda não ter aparecido
        usuario = procurar_no(raiz_arvore_ip, endereco);
        p_dominio = procurar_no(raiz_arvore_dominio, dominio);

        //caso o usuário ainda não tenha sido adicionado na árvore de ips, o adicionamos e atualizamos o p_no usuario
        if (usuario == NULL) {
            raiz_arvore_ip = inserir_rec(raiz_arvore_ip, endereco, NULL, 0);
            usuario = procurar_no(raiz_arvore_ip, endereco);
        }

        //caso o visitante já tenha extrapolado o número de consultas permitidas, emitimos a mensagem de proibição
        if (usuario->visitas > consultas_permitidas)
            printf ("FORBIDDEN %s FROM %s\n", dominio, endereco);
        // caso o domínio não exista na árvore de domínios, exibimos a mensagem de erro
        else if (p_dominio == NULL) 
            printf ("NOTFOUND %s FROM %s\n", dominio, endereco);
        //caso esteja tudo certo, é exibida a mensagem de acesso
        else
            printf("ACCEPTED %s FROM %s RESPOND %s\n", dominio, endereco, p_dominio->servidor);
        //é adicionado 1 ao número de visitas do usuário (já inicializado como 1 em inserir_rec)
        usuario->visitas++;
    }
    //as árvores são liberadas
    destroi_arvore(raiz_arvore_ip);
    destroi_arvore(raiz_arvore_dominio);
    return 0;
}
