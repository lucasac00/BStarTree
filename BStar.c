#include "Bstar.h"

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
    //tinha problema aqui e desabilitei as marcações de erro
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

/*MINHA PARTE -- LUIZ*/

// Função para encontrar o predecessor (máximo) de um nó
Noh* findMaxNode(Noh* node) {
    Noh* current = node;
    while (!current->ehfolha) {
        current = current->filhos[current->n];
    }
    return current;
}

// Função para remover uma chave da árvore B*
void _remove(BStarTree* arvore, Noh* node, int key) {
     if (node == NULL) {
        return;
    }

    // Encontra o índice da chave no nó atual
    int i = 0;
    while (i < node->n && key > node->chaves[i]) {
        i++;
    }

    // Caso 1: A chave está presente no nó atual
    if (i < node->n && key == node->chaves[i]) {
        if (node->ehfolha) {
            // Caso 1A: O nó é uma folha
            for (int j = i; j < node->n - 1; j++) {
                node->chaves[j] = node->chaves[j + 1];
            }
            node->n--;
            return;
        } else {
            // Caso 1B: O nó não é uma folha
            Noh* predecessor = findMaxNode(node->filhos[i]);
            node->chaves[i] = predecessor->chaves[predecessor->n - 1];
            _remove(arvore, predecessor, node->chaves[i]);
        }
    } else {
        // Caso 2: A chave não está presente no nó atual
        if (node->ehfolha) {
            // A chave não existe na árvore
            return;
        }

        // Verifica se o nó filho onde a chave poderia estar tem chaves suficientes
        if (node->filhos[i]->n >= arvore->ordem / 2) {
            _remove(arvore, node->filhos[i], key);
        } else {
            // Caso 3: O nó filho não tem chaves suficientes

            // Tenta emprestar uma chave do nó vizinho esquerdo
            if (i > 0 && node->filhos[i - 1]->n > arvore->ordem / 2) {
                // Move a chave do nó pai para o nó filho
                for (int j = node->filhos[i]->n; j > 0; j--) {
                    node->filhos[i]->chaves[j] = node->filhos[i]->chaves[j - 1];
                }
                node->filhos[i]->chaves[0] = node->chaves[i - 1];

                // Move o último filho do nó vizinho esquerdo para o nó filho
                if (!node->filhos[i]->ehfolha) {
                    for (int j = node->filhos[i]->n + 1; j > 0; j--) {
                        node->filhos[i]->filhos[j] = node->filhos[i]->filhos[j - 1];
                    }
                    node->filhos[i]->filhos[0] = node->filhos[i - 1]->filhos[node->filhos[i - 1]->n];
                    if (node->filhos[i - 1]->filhos[node->filhos[i - 1]->n] != NULL) {
                        node->filhos[i - 1]->filhos[node->filhos[i - 1]->n]->pai = node->filhos[i];
                    }
                }

                node->chaves[i - 1] = node->filhos[i - 1]->chaves[node->filhos[i - 1]->n - 1];
                node->filhos[i - 1]->n--;
                node->filhos[i]->n++;
            }
            // Tenta emprestar uma chave do nó vizinho direito
            else if (i < node->n && node->filhos[i + 1]->n > arvore->ordem / 2) {
                // Move a chave do nó pai para o nó filho
                node->filhos[i]->chaves[node->filhos[i]->n] = node->chaves[i];

                // Move o primeiro filho do nó vizinho direito para o nó filho
                if (!node->filhos[i]->ehfolha) {
                    node->filhos[i]->filhos[node->filhos[i]->n + 1] = node->filhos[i + 1]->filhos[0];
                    if (node->filhos[i + 1]->filhos[0] != NULL) {
                        node->filhos[i + 1]->filhos[0]->pai = node->filhos[i];
                    }
                }

                node->chaves[i] = node->filhos[i + 1]->chaves[0];

                // Move as chaves do nó vizinho direito para a esquerda
                for (int j = 0; j < node->filhos[i + 1]->n - 1; j++) {
                    node->filhos[i + 1]->chaves[j] = node->filhos[i + 1]->chaves[j + 1];
                }

                // Move os filhos do nó vizinho direito para a esquerda
                if (!node->filhos[i + 1]->ehfolha) {
                    for (int j = 0; j <= node->filhos[i + 1]->n; j++) {
                        node->filhos[i + 1]->filhos[j] = node->filhos[i + 1]->filhos[j + 1];
                        if (node->filhos[i + 1]->filhos[j + 1] != NULL) {
                            node->filhos[i + 1]->filhos[j + 1]->pai = node->filhos[i + 1];
                        }
                    }
                }

                node->filhos[i]->n++;
                node->filhos[i + 1]->n--;
            }
            // Caso 4: Fusão com um dos vizinhos
            else {
                if (i > 0) {
                    // Fusão com o nó vizinho esquerdo
                    mergeNoh(arvore, node, i - 1);
                    i--;
                } else {
                    // Fusão com o nó vizinho direito
                    mergeNoh(arvore, node, i);
                }
                _remove(arvore, node->filhos[i], key);
            }
        }
    }

    // Caso especial: Se a raiz ficar vazia após a remoção, atualize a raiz
    if (node->n == 0) {
        if (node == arvore->raiz) {
            free(node->chaves);
            free(node->filhos);
            free(node);
            arvore->raiz = NULL;
        } else {
            Noh* pai = node->pai;
            int j = 0;
            while (j <= pai->n && pai->filhos[j] != node) {
                j++;
            }
            for (int k = j; k < pai->n - 1; k++) {
                pai->chaves[k] = pai->chaves[k + 1];
                pai->filhos[k + 1] = pai->filhos[k + 2];
            }
            pai->n--;
            free(node->chaves);
            free(node->filhos);
            free(node);
            _remove(arvore, pai, key);
        }
    }
}
