/*
IMPLEMENTAÇÃO DA ESTRUTURA DE DADOS ÁRVORE B* (ÁRVORE B ESTRELA)
LUCAS DE ARAÚJO CARDOSO - 813583
ARTHUR BRAGA DA FONSECA
LUIZ OTÁVIO DE MELO
*/
#include <stdio.h>
#include <stdlib.h>

#define ORDEM 8

typedef struct NO
{
    int num_chaves;
    int chaves[ORDEM - 1];
    struct NO *filhos[ORDEM];
    int folha;
} ArvBNoh;

typedef ArvBNoh *ArvB;

// Função que cria uma árvore B estrela.
ArvB arvB_cria()
{
    ArvB nova_arvore = (ArvB)malloc(sizeof(ArvBNoh));
    nova_arvore->num_chaves = 0;
    nova_arvore->folha = 1;
    for (int i = 0; i < ORDEM; i++)
    {
        nova_arvore->filhos[i] = NULL; // Ainda não há filhos portanto apontamos para NULL
    }
    return nova_arvore;
}

// Função recursiva para liberar o espaço alocado da árvore
void arvB_destroi_recursivo(ArvB no)
{
    if (no == NULL)
    {
        return;
    }

    if (!no->folha)
    {
        for (int i = 0; i <= no->num_chaves; i++)
        {
            arvB_destroi_recursivo(no->filhos[i]);
        }
    }

    free(no);
}

// Função caller para a função recursiva que libera o espaço de memória alocado pela árvore
void arvB_destroi(ArvB *raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {
        arvB_destroi_recursivo(*raiz);
        *raiz = NULL;
    }
}

// Retorna 1 se a chave existe na árvore e 0 se não existe
int arvB_busca(ArvB no, int valor)
{
    int i = 0;
    while (i < no->num_chaves && valor > no->chaves[i])
    {
        i++;
    }
    if (i < no->num_chaves && valor == no->chaves[i])
    {
        return 1;
    }
    else if (no->folha)
    {
        return 0;
    }
    else
    {
        return arvB_busca(no->filhos[i], valor);
    }
}

// Função auxiliar da inserção em nó cheio, executa a divisão (split)
void arvB_divide_filho(ArvB pai, int indice_filho)
{
    ArvB filho = pai->filhos[indice_filho];
    ArvB novo_filho = arvB_cria();
    novo_filho->folha = filho->folha;
    novo_filho->num_chaves = ORDEM / 2 - 1;

    for (int i = 0; i < ORDEM / 2 - 1; i++)
    {
        novo_filho->chaves[i] = filho->chaves[i + ORDEM / 2];
    }

    if (!filho->folha)
    {
        for (int i = 0; i < ORDEM / 2; i++)
        {
            novo_filho->filhos[i] = filho->filhos[i + ORDEM / 2];
        }
    }

    filho->num_chaves = ORDEM / 2 - 1;

    for (int i = pai->num_chaves; i >= indice_filho + 1; i--)
    {
        pai->filhos[i + 1] = pai->filhos[i];
    }

    pai->filhos[indice_filho + 1] = novo_filho;

    for (int i = pai->num_chaves - 1; i >= indice_filho; i--)
    {
        pai->chaves[i + 1] = pai->chaves[i];
    }

    pai->chaves[indice_filho] = filho->chaves[ORDEM / 2 - 1];
    pai->num_chaves++;
}

// Função principal de inserção 
int arvB_insere_nao_cheio(ArvB no, int valor)
{
    int i = no->num_chaves - 1;

    if (no->folha)
    {
        while (i >= 0 && valor < no->chaves[i])
        {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        no->chaves[i + 1] = valor;
        no->num_chaves++;
    }
    else
    {
        while (i >= 0 && valor < no->chaves[i])
        {
            i--;
        }
        i++;
        if (no->filhos[i]->num_chaves == ORDEM - 1)
        {
            arvB_divide_filho(no, i);
            if (valor > no->chaves[i])
            {
                i++;
            }
        }
        arvB_insere_nao_cheio(no->filhos[i], valor);
    }

    return 1;
}

// Função de inserção pública, chamada para inserir um valor na árvore B
int arvB_insere(ArvB *raiz, int valor)
{
    ArvB raiz_atual = *raiz;
    if (raiz_atual->num_chaves == ORDEM - 1)
    {
        ArvB nova_raiz = arvB_cria();
        nova_raiz->folha = 0;
        nova_raiz->filhos[0] = raiz_atual;
        arvB_divide_filho(nova_raiz, 0);
        arvB_insere_nao_cheio(nova_raiz, valor);
        *raiz = nova_raiz;
    }
    else
    {
        arvB_insere_nao_cheio(raiz_atual, valor);
    }
    return 1;
}

// Função auxiliar para encontrar o índice da primeira chave maior ou igual a 'valor'
int encontrar_indice_maior_igual(const int chaves[], int num_chaves, int valor)
{
    int i = 0;
    while (i < num_chaves && valor > chaves[i])
    {
        i++;
    }
    return i;
}

// Função auxiliar para remover uma chave de um nó folha
void remover_chave_folha(ArvB no, int indice_chave)
{
    for (int i = indice_chave; i < no->num_chaves - 1; i++)
    {
        no->chaves[i] = no->chaves[i + 1];
    }
    no->num_chaves--;
}

// Função auxiliar para trocar uma chave por seu predecessor em um nó não folha
int trocar_por_predecessor(ArvB no, int indice_chave)
{
    ArvB no_atual = no->filhos[indice_chave];
    while (!no_atual->folha)
    {
        no_atual = no_atual->filhos[no_atual->num_chaves];
    }
    int predecessor = no_atual->chaves[no_atual->num_chaves - 1];
    no->chaves[indice_chave] = predecessor;
    return predecessor;
}

// Função auxiliar para trocar uma chave por seu sucessor em um nó não folha
int trocar_por_sucessor(ArvB no, int indice_chave)
{
    ArvB no_atual = no->filhos[indice_chave + 1];
    while (!no_atual->folha)
    {
        no_atual = no_atual->filhos[0];
    }
    int sucessor = no_atual->chaves[0];
    no->chaves[indice_chave] = sucessor;
    return sucessor;
}

// Função para fundir dois nós quando necessário durante a remoção
void merge_nos(ArvB pai, int indice_filho)
{
    ArvB filho_esquerdo = pai->filhos[indice_filho];
    ArvB filho_direito = pai->filhos[indice_filho + 1];

    filho_esquerdo->chaves[ORDEM / 2 - 1] = pai->chaves[indice_filho];

    for (int i = 0; i < filho_direito->num_chaves; i++)
    {
        filho_esquerdo->chaves[i + ORDEM / 2] = filho_direito->chaves[i];
    }

    if (!filho_direito->folha)
    {
        for (int i = 0; i <= filho_direito->num_chaves; i++)
        {
            filho_esquerdo->filhos[i + ORDEM / 2] = filho_direito->filhos[i];
        }
    }

    for (int i = indice_filho + 1; i < pai->num_chaves; i++)
    {
        pai->chaves[i - 1] = pai->chaves[i];
    }

    for (int i = indice_filho + 2; i <= pai->num_chaves; i++)
    {
        pai->filhos[i - 1] = pai->filhos[i];
    }

    filho_esquerdo->num_chaves += filho_direito->num_chaves + 1;
    pai->num_chaves--;

    free(filho_direito);
}

// Função para emprestar uma chave do nó antecessor
void empresta_do_antecessor(ArvB pai, int indice_filho)
{
    ArvB filho = pai->filhos[indice_filho];
    ArvB irmao = pai->filhos[indice_filho - 1];

    for (int i = filho->num_chaves - 1; i >= 0; i--)
    {
        filho->chaves[i + 1] = filho->chaves[i];
    }

    if (!filho->folha)
    {
        for (int i = filho->num_chaves; i >= 0; i--)
        {
            filho->filhos[i + 1] = filho->filhos[i];
        }
    }

    filho->chaves[0] = pai->chaves[indice_filho - 1];

    if (!irmao->folha)
    {
        filho->filhos[0] = irmao->filhos[irmao->num_chaves];
    }

    pai->chaves[indice_filho - 1] = irmao->chaves[irmao->num_chaves - 1];

    filho->num_chaves++;
    irmao->num_chaves--;
}

// Função para emprestar uma chave do nó sucessor
void empresta_do_sucessor(ArvB pai, int indice_chave)
{
    ArvB filho_atual = pai->filhos[indice_chave];
    ArvB filho_sucessor = pai->filhos[indice_chave + 1];

    filho_atual->chaves[filho_atual->num_chaves] = pai->chaves[indice_chave];
    filho_atual->num_chaves++;

    pai->chaves[indice_chave] = filho_sucessor->chaves[0];

    for (int i = 1; i < filho_sucessor->num_chaves; i++)
    {
        filho_sucessor->chaves[i - 1] = filho_sucessor->chaves[i];
    }

    if (!filho_sucessor->folha)
    {
        for (int i = 1; i <= filho_sucessor->num_chaves; i++)
        {
            filho_sucessor->filhos[i - 1] = filho_sucessor->filhos[i];
        }
    }

    filho_sucessor->num_chaves--;
}

// Função de remoção pública, chamada para remover um valor da árvore B
int arvB_remove(ArvB no, int valor)
{
    int indice_chave = encontrar_indice_maior_igual(no->chaves, no->num_chaves, valor);

    if (indice_chave < no->num_chaves && no->chaves[indice_chave] == valor)
    {
        if (no->folha)
        {
            remover_chave_folha(no, indice_chave);
        }
        else
        {
            int chave_substituta;
            if (no->filhos[indice_chave]->num_chaves >= ORDEM / 2)
            {
                chave_substituta = trocar_por_predecessor(no, indice_chave);
                arvB_remove(no->filhos[indice_chave], chave_substituta);
            }
            else if (no->filhos[indice_chave + 1]->num_chaves >= ORDEM / 2)
            {
                chave_substituta = trocar_por_sucessor(no, indice_chave + 1);
                arvB_remove(no->filhos[indice_chave + 1], chave_substituta);
            }
            else
            {
                merge_nos(no, indice_chave);
                arvB_remove(no->filhos[indice_chave], valor);
            }
        }
    }
    else
    {
        if (no->folha)
        {
            return 0;
        }
        int filho = indice_chave;
        if (no->filhos[indice_chave]->num_chaves == ORDEM / 2 - 1)
        {
            if (indice_chave > 0 && no->filhos[indice_chave - 1]->num_chaves >= ORDEM / 2)
            {
                empresta_do_antecessor(no, indice_chave);
            }
            else if (indice_chave < no->num_chaves && no->filhos[indice_chave + 1]->num_chaves >= ORDEM / 2)
            {
                empresta_do_sucessor(no, indice_chave);
            }
            else
            {
                if (indice_chave < no->num_chaves)
                {
                    merge_nos(no, indice_chave);
                }
                else
                {
                    merge_nos(no, indice_chave - 1);
                }
            }
        }
        arvB_remove(no->filhos[filho], valor);
    }
    return 1;
}

// Função auxiliar para contar a quantidade de nós na árvore de forma recursiva
int arvB_qtd_nos_recursivo(ArvB no)
{
    if (no == NULL)
    {
        return 0;
    }
    int qtd = 1;
    for (int i = 0; i <= no->num_chaves; i++)
    {
        qtd += arvB_qtd_nos_recursivo(no->filhos[i]);
    }
    return qtd;
}

// Função para imprimir as chaves da árvore em ordem
void arvB_imprime_recursivo(ArvB no)
{
    if (no != NULL)
    {
        for (int i = 0; i < no->num_chaves; i++)
        {
            arvB_imprime_recursivo(no->filhos[i]);
            printf("%d ", no->chaves[i]);
        }
        arvB_imprime_recursivo(no->filhos[no->num_chaves]);
    }
}

// Função de impressão pública, chamada para imprimir as chaves da árvore em ordem
void arvB_imprime(ArvB raiz)
{
    arvB_imprime_recursivo(raiz);
    printf("\n");
}

int arvB_qtd_nos(ArvB raiz)
{
    return arvB_qtd_nos_recursivo(raiz);
}

int arvB_qtd_chaves_recursivo(ArvB no)
{
    if (no == NULL)
    {
        return 0;
    }
    int qtd = no->num_chaves;
    for (int i = 0; i <= no->num_chaves; i++)
    {
        qtd += arvB_qtd_chaves_recursivo(no->filhos[i]);
    }
    return qtd;
}

int arvB_qtd_chaves(ArvB raiz)
{
    return arvB_qtd_chaves_recursivo(raiz);
}
