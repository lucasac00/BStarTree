#include <stdio.h>
#include <stdlib.h>

#define Ordem 4

typedef struct Noh
{
    int chaves[Ordem - 1];
    struct Noh *filhos[Ordem];
    struct Noh *pai;
    int n;       // Conta quantos dos espaços de chave estão preenchidos
    int ehfolha; // Informa se o nó é um nó folha. 1 = é folha, 0 = não é folha
} Noh;

typedef struct BStarTree
{
    Noh *raiz;
} BStarTree;

BStarTree *inicializaArvore(int ordem = Ordem);

BStarTree *inicializaNoh(int ordem = Ordem);

// Função de split usada quando há overflow em nós
void split2in3(BStarTree *arvore, Noh *noh, int index);

// Função para fazer o merge de nós em caso de underflow
void mergeNoh(BStarTree* arvore, Noh* pai, int index);

// Função caller da busca
BStarTree *busca(BStarTree *arvore, int chave);

BStarTree *_busca(BStarTree *arvore, Noh *noh, int chave);

// Função caller da inserção
void insere(BStarTree *arvore, int chave);

void _insere(BStarTree *arvore, Noh *noh, int chave);

// Função caller da remoção
void remove(BStarTree *arvore, int chave);

void _remove(BStarTree *arvore, Noh *noh, int chave);

// Função para imprimir todas as chaves da árvore, inorder
void print(BStarTree *arvore);
