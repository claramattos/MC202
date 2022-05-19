#include <stdio.h>
#include <stdlib.h>

void imprime_char(char *v, int n) {
    int i;
    for (i = 0; i < n; i++)
        printf("%c	", v[i]);
    printf("\n");
}
void imprime_int(int *v, int n) {
    int i;
    for (i = 0; i < n; i++)
        printf("%d	", v[i]);
    printf("\n");
}

int main() {
    char *dna, *rna;
    int *id;
    char dna_padrao[14], rna_padrao[14];
    int n, i, base, contagem[14];
    
    for (i=0;i<14;i++)
        contagem[i] = -1;

    scanf ("%s\n", dna_padrao);
    scanf ("%s\n", rna_padrao);
    scanf ("%d\n", &n);

    dna = malloc((n + 1)*sizeof(char));
    rna = malloc((n + 1)*sizeof(char));
    id = malloc(n*sizeof(int));

    for (i=0;i<n;i++) {
        scanf ("%c", &dna[i]);
        for (base=0;base<14;base++){
            if (dna[i] == dna_padrao[base]) {
                rna[i] = rna_padrao[base];
                contagem[base]++;
                id[i]=contagem[base];
            }
        }
    }

    printf ("ID: 	");
    imprime_int(id, n);
    printf ("DNA:	");
    imprime_char (dna,n);

    for (i=0;i<n;i++ )
        printf("	|");

    printf ("\nRNA: 	");
    imprime_char (rna,n);

    free (dna);
    free (rna);
    free (id);

    return 0;
}