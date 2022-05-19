#include "planejar.h"
#include <stdio.h>
#include <string.h>

Data ler_data() {
    Data data1;
    scanf("%d/%d/%d", &data1.dia, &data1.mes, &data1.ano);
    return data1;
}

int compara_data(Data data1, Data data2)  {
    if (data1.ano>data2.ano)
        return 1;
    else if (data1.ano<data2.ano)
        return -1;
    else if (data1.mes>data2.mes)
        return 1;
    else if (data1.mes<data2.mes)
        return -1;
    else if (data1.dia>data2.dia)
        return 1;
    else if (data1.dia<data2.dia)
        return -1;
    else    
        return 0;
}

int diferenca_data(Data data1, Data data2)  {
    int meses[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31}, dias_diferenca = 0, k, dias_datamaior = 0, dias_datamenor = 0;
    Data data_maior, data_menor;
    
    if (compara_data(data1, data2) == 1) {
        data_maior = data1;
        data_menor = data2;
    }
    else if (compara_data(data1,data2)==-1) {
        data_maior = data2;
        data_menor = data1;
    }
    else if (compara_data(data1,data2)== 0) {
        dias_diferenca = 0;
        return dias_diferenca;
    }

    for (k=0;k<=data_maior.mes;k++)  {
        if (k == data_maior.mes)
            dias_datamaior += data_maior.dia;
        else {
            dias_datamaior += meses[k];
        }
    }
    for (k=0;k<=data_menor.mes;k++)  {
            if (k == data_menor.mes)
                dias_datamenor += data_menor.dia;
            else {
                dias_datamenor += meses[k];
            }
        }
    if (data_menor.ano == data_maior.ano)
        dias_diferenca = dias_datamaior-dias_datamenor +1;
    else
        dias_diferenca = 365 - dias_datamenor + dias_datamaior;

    return dias_diferenca;
}


void cancelar(Voo voos[], int qtd_voos, int numero_voo) {
    for (int i=0;i<qtd_voos;i++){
        if (voos[i].numero == numero_voo)
            voos[i].cancela = 1;
    }
}

void alterar(Voo voos[], int qtd_voos, int numero_voo, float novo_valor){
    for (int i=0;i<qtd_voos;i++){
        if (voos[i].numero == numero_voo)
            voos[i].preco = novo_valor;
    }
}

void planejar(Voo voos[], int qtd_voos, Data data_inicio, Data data_fim, char codigo_origem[4]) {
    int ida =0, volta = 0;
    double menor_preco = 10000000.00, preco =0; 

    printf("\n");

    for (int i=0;i<qtd_voos;i++) {
        if (strcmp (voos[i].origem, codigo_origem) == 0 && voos[i].cancela != 1) {
            for (int k=0;k<qtd_voos;k++) {
                preco = voos[i].preco + voos[k].preco;
                if (strcmp(voos[i].destino, voos[k].origem) == 0 && voos[k].cancela != 1 &&  preco<menor_preco && diferenca_data(voos[i].data, voos[k].data) >=3) {
                    if (compara_data(voos[i].data, data_inicio) != -1 && compara_data(voos[k].data, data_fim) !=1 && compara_data(voos[i].data, voos[k].data) != 1) {
                        ida = voos[i].numero;
                        volta= voos[k].numero;
                        menor_preco = voos[i].preco + voos[k].preco;
                    }
                }
            }
        }
    }
    printf ("%d\n%d", ida, volta);
}
