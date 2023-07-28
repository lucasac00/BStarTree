#include <stdio.h>
#include <stdlib.h>

#define Ordem 4

typedef struct Noh
{
    int chaves[Ordem - 1];
    struct Noh *filhos[Ordem];
    struct Noh *pai;
    int n; // Conta quantos dos espaços de chave estão preenchidosssh
};