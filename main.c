/*
IMPLEMENTAÇÃO DA ESTRUTURA DE DADOS ÁRVORE B
LUCAS DE ARAÚJO CARDOSO - 813583
ARTHUR BRAGA DA FONSECA - 811461
GIOVANNI ROSSI - 801301
*/

#include <stdlib.h>
#include "arvB.h"

int main()
{
    int vetor[50];
    for(int i = 0; i < 50; i++)
    {
        vetor[i] = rand() %100 ;
    }
    ArvB arvore = arvB_cria();

    // Inserindo chaves para testar remoção e contagem
    for (int i = 1; i <= 50; i++)
    {
        arvB_insere(&arvore, vetor[i] );
    }

    printf("Quantidade de nós: %d\n", arvB_qtd_nos(arvore));
    printf("Quantidade de chaves: %d\n", arvB_qtd_chaves(arvore));
    printf("Busca por 15: %d\n", arvB_busca(arvore, 15));
    arvB_imprime(arvore);

    arvB_remove(arvore, 15);

    printf("Quantidade de nós após remoção: %d\n", arvB_qtd_nos(arvore));
    printf("Quantidade de chaves após remoção: %d\n", arvB_qtd_chaves(arvore));
    printf("Busca por 15: %d\n", arvB_busca(arvore, 15));
    arvB_imprime(arvore);

    arvB_destroi(&arvore);

    return 0;
}
