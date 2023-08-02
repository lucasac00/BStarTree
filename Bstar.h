#include <stdio.h>
#include <stdlib.h>

typedef struct Noh
{
    int *chaves;
    struct Noh **filhos;
    struct Noh *pai;
    int n;       // Conta quantos dos espaços de chave estão preenchidos
    int ehfolha; // Informa se o nó é um nó folha. 1 = é folha, 0 = não é folha
} Noh;

typedef struct BStarTree
{
    Noh *raiz;
    int ordem;
} BStarTree;

// Função para inicializar uma nova árvore B*
BStarTree *inicializaArvore(int ORDEM)
{
    BStarTree *novaArvore = (BStarTree *)malloc(sizeof(BStarTree));
    if (novaArvore == NULL)
    {
        printf("Erro: não foi possível alocar memória para a árvore.\n");
        return NULL;
    }

    novaArvore->raiz = inicializaNoh(ORDEM);
    if (novaArvore->raiz == NULL)
    {
        free(novaArvore);
        return NULL;
    }

    novaArvore->ordem = ORDEM;

    return novaArvore;
}

// Função para inicializar um novo nó
Noh *inicializaNoh(int ORDEM)
{
    Noh *novoNoh = (Noh *)malloc(sizeof(Noh));
    if (novoNoh == NULL)
    {
        printf("Erro: não foi possível alocar memória para o novo nó.\n");
        return NULL;
    }

    novoNoh->chaves = (int *)malloc((2 * ORDEM - 1) * sizeof(int));
    if (novoNoh->chaves == NULL)
    {
        printf("Erro: não foi possível alocar memória para as chaves do nó.\n");
        free(novoNoh);
        return NULL;
    }

    novoNoh->filhos = (Noh **)malloc(2 * ORDEM * sizeof(Noh *));
    if (novoNoh->filhos == NULL)
    {
        printf("Erro: não foi possível alocar memória para os filhos do nó.\n");
        free(novoNoh->chaves);
        free(novoNoh);
        return NULL;
    }

    novoNoh->pai = NULL;
    novoNoh->n = 0;
    novoNoh->ehfolha = 1;

    return novoNoh;
}

// Função de split usada quando há overflow em nós
void split2in3(BStarTree *arvore, Noh *noh, int index)
{
    // Cria um novo nó para a direita
    Noh *novoNohDireita = inicializaNoh(arvore->ordem);
    Noh *nodo = noh->filhos[index];
    int i;

    // Copia as últimas (ordem - 1) chaves e filhos do nó que estourou para o novo nó
    novoNohDireita->ehfolha = nodo->ehfolha;
    novoNohDireita->n = arvore->ordem - 1;
    for (i = 0; i < arvore->ordem - 1; i++)
        novoNohDireita->chaves[i] = nodo->chaves[i + arvore->ordem];

    if (!nodo->ehfolha)
    {
        for (i = 0; i < arvore->ordem; i++)
            novoNohDireita->filhos[i] = nodo->filhos[i + arvore->ordem];
    }

    // Reduz o número de chaves no nó que estourou
    nodo->n = arvore->ordem - 1;

    // Move os filhos do nó pai para criar espaço para o novo filho à direita
    for (i = noh->n; i >= index + 1; i--)
        noh->filhos[i + 1] = noh->filhos[i];
    noh->filhos[index + 1] = novoNohDireita;

    // Move as chaves do nó pai para criar espaço para a nova chave
    for (i = noh->n - 1; i >= index; i--)
        noh->chaves[i + 1] = noh->chaves[i];

    // Move a mediana do nó que estourou para o nó pai
    noh->chaves[index] = nodo->chaves[arvore->ordem - 1];

    // Incrementa o número de chaves no nó pai
    noh->n++;
}

// Função para fazer o merge de nós em caso de underflow
void mergeNoh(BStarTree *arvore, Noh *pai, int index)
{
    Noh *nodoEsquerda = pai->filhos[index];
    Noh *nodoDireita = pai->filhos[index + 1];

    // Move a chave do nó pai para o nó esquerdo
    nodoEsquerda->chaves[arvore->ordem - 1] = pai->chaves[index];

    // Move as chaves do nó direito para o nó esquerdo
    for (int i = 0; i < nodoDireita->n; i++)
        nodoEsquerda->chaves[i + arvore->ordem] = nodoDireita->chaves[i];

    // Move os filhos do nó direito para o nó esquerdo, caso não seja folha
    if (!nodoDireita->ehfolha)
    {
        for (int i = 0; i <= nodoDireita->n; i++)
            nodoEsquerda->filhos[i + arvore->ordem] = nodoDireita->filhos[i];
    }

    // Atualiza o número de chaves do nó esquerdo após o merge
    nodoEsquerda->n += nodoDireita->n + 1;

    // Move os filhos à esquerda para cobrir o espaço do nó direito que foi retirado
    for (int i = index + 1; i < pai->n; i++)
        pai->filhos[i] = pai->filhos[i + 1];

    // Atualiza o número de filhos do nó pai após o merge
    pai->n--;

    // Libera a memória alocada para o nó direito
    free(nodoDireita->chaves);
    free(nodoDireita->filhos);
    free(nodoDireita);
}

// Função caller da busca
BStarTree *busca(BStarTree *arvore, int chave); //Lucas

BStarTree *_busca(BStarTree *arvore, Noh *noh, int chave); // Lucas

// Função caller da inserção
void insere(BStarTree *arvore, int chave); // Lucas

void _insere(BStarTree *arvore, Noh *noh, int chave); // Lucas

// Função caller da remoção
void remove(BStarTree *arvore, int chave); // Luiz

void _remove(BStarTree *arvore, Noh *noh, int chave); // Luiz

// Função para imprimir todas as chaves da árvore, inorder
void print(BStarTree *arvore); // Luiz
