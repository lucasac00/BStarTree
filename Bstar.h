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

BStarTree *inicializaArvore(int ORDEM); //Arthur

BStarTree *inicializaNoh(int ORDEM); // Arthur

// Função de split usada quando há overflow em nós
void split2in3(BStarTree *arvore, Noh *noh, int index); //Arthur 

// Função para fazer o merge de nós em caso de underflow
void mergeNoh(BStarTree *arvore, Noh *pai, int index); //Arthur

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
