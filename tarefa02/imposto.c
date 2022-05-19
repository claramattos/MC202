#include <stdio.h>

void impostos_retidos (float matriz[][12], float saida[][15], int n_funcionarios) {
    int i, k;
    for (i=0; i<n_funcionarios;i++) {
        for (k=0;k<12;k++) {
            if (matriz[i][k] < 1499.15)
                saida[i][k] = 0;
            else if (matriz[i][k] < 2246.75)
                saida[i][k] = (matriz[i][k] - 1499.15) * 0.075;
            else if (matriz[i][k] < 2995.70)
                saida[i][k] = 56.07 + (matriz[i][k] - 2246.75) * 0.15;
            else if (matriz[i][k] < 3743.19)
                saida[i][k] = 168.4125 + (matriz[i][k] - 2995.70) * 0.225;
            else 
                saida[i][k] = 336.59775 + (matriz[i][k] - 3743.19) * 0.275;
        }
    
    }
}
void calcula_total_retido_na_fonte (float saida[][15], int n_funcionarios) {
    int i, k;
    for (i=0;i<n_funcionarios;i++){
        float total_retido = 0.00;
        for (k=0; k<12;k++)
            total_retido += saida[i][k];
        saida[i][12]=total_retido;

    }
}

void calcula_imposto_devido_anual(float saida[][15], float matriz[][12], int n_funcionarios, float abatimento[][1]) {
    int i, k;
    for (i=0;i<n_funcionarios;i++){
        float total_recebido = 0.00;
        for (k=0; k<12;k++)
            total_recebido += matriz[i][k];
        total_recebido-= abatimento[i][0];
        if (total_recebido < 18000)
            saida[i][13] = 0.00;
        else if (total_recebido < 26400)
            saida[i][13] = (total_recebido-18000) * 0.075;
        else if (total_recebido < 36000)
            saida[i][13] = 630 + (total_recebido-26400) * 0.15;
        else if (total_recebido < 44400)
            saida[i][13] = 2070 + (total_recebido-36000) * 0.225;
        else 
            saida[i][13] = 3960 + (total_recebido-44400) * 0.275;
    }


}
void calcula_valor_do_ajuste (float saida[][15], int n_funcionarios){
    int k;
    float valor_ajuste;
    for (k=0;k<n_funcionarios;k++){
        valor_ajuste= saida[k][13]-saida[k][12];
        saida[k][14] = valor_ajuste;
    }


}

int main() {
    int i, k, n_funcionarios;
    float matriz[999][12], saida[999][15], abatimento[999][1];
    scanf("%d\n", &n_funcionarios);
    for (i=0; i<n_funcionarios;i++) {
        for (k=0;k<12;k++) {
            scanf("%f", &matriz[i][k]);
        } 
    }

    for (i=0; i<n_funcionarios;i++) {
        scanf("%f", &abatimento[i][0]);
    }

    impostos_retidos(matriz, saida, n_funcionarios);
    calcula_total_retido_na_fonte (saida, n_funcionarios);
    calcula_imposto_devido_anual(saida, matriz, n_funcionarios, abatimento);
    calcula_valor_do_ajuste(saida, n_funcionarios);

    printf ("Jan Fev Mar Abr Mai Jun Jul Ago Set Out Nov Dez Retido Devido Ajuste\n");
    for (i=0; i<n_funcionarios;i++) {
        for (k=0;k<15;k++) 
            printf ("%.2f ", saida[i][k]);
        printf ("\n");
    }

    return 0;

}