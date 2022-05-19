#include "planejar.h"
#include <stdio.h>
#include <string.h>

int main (){
    Voo voos[150];
    char operacao[10], codigo_origem[4];
    int qtd_voos = 0;

    scanf("%s", operacao);
    while (strcmp(operacao, "planejar") != 0){
        if (strcmp(operacao, "registrar") == 0) {
            Voo novo_voo;
            scanf ("%d\n%s %s", &novo_voo.numero, novo_voo.origem, novo_voo.destino);
            novo_voo.data = ler_data();
            scanf ("%lf", &novo_voo.preco);
            novo_voo.cancela = 0;
            voos[qtd_voos] = novo_voo;
            qtd_voos+= 1;
        }
        else if (strcmp(operacao, "alterar") == 0) {
            int numero_voo;
            double novo_valor;
            scanf("%d %lf", &numero_voo, &novo_valor);
            alterar(voos, qtd_voos, numero_voo, novo_valor);
        }
        else if (strcmp(operacao, "cancelar") == 0) {
            int numero_voo;
            scanf("%d", &numero_voo);
            cancelar(voos, qtd_voos, numero_voo);
            
        }
        scanf("%s", operacao);
    }
    scanf ("%s", codigo_origem);
    Data data_inicio = ler_data();
    Data data_fim = ler_data();

    planejar(voos, qtd_voos, data_inicio, data_fim, codigo_origem);
    return 0;
}