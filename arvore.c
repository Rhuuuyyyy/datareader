#include "arvore.h"
#include <stdio.h>
#include <stdlib.h>

/* Cria um novo nó de árvore binária */
NoArvore* criar_no_arvore(double chave, NoAlimento* alimento) {
    NoArvore* novo = (NoArvore*)malloc(sizeof(NoArvore));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para no de arvore.\n");
        exit(1);
    }
    novo->chave = chave;
    novo->alimento = alimento;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

/* Insere um nó na árvore binária mantendo a propriedade de BST */
NoArvore* inserir_na_arvore(NoArvore* raiz, double chave, NoAlimento* alimento) {
    if (raiz == NULL) {
        return criar_no_arvore(chave, alimento);
    }

    if (chave < raiz->chave) {
        raiz->esquerda = inserir_na_arvore(raiz->esquerda, chave, alimento);
    } else {
        raiz->direita = inserir_na_arvore(raiz->direita, chave, alimento);
    }

    return raiz;
}

/* Percorre a árvore em ordem decrescente (direita-raiz-esquerda) */
void percorrer_decrescente(NoArvore* raiz) {
    if (raiz == NULL) {
        return;
    }

    percorrer_decrescente(raiz->direita);
    printf("  %3d | %-50s | Energia: %4d kcal | Proteina: %5.1f g\n",
           raiz->alimento->numero,
           raiz->alimento->descricao,
           raiz->alimento->energia_kcal,
           raiz->alimento->proteina);
    percorrer_decrescente(raiz->esquerda);
}

/* Percorre a árvore buscando valores em um intervalo [min, max] */
void percorrer_intervalo(NoArvore* raiz, double min, double max) {
    if (raiz == NULL) {
        return;
    }

    if (raiz->chave > min) {
        percorrer_intervalo(raiz->esquerda, min, max);
    }

    if (raiz->chave >= min && raiz->chave <= max) {
        printf("  %3d | %-50s | Energia: %4d kcal | Proteina: %5.1f g\n",
               raiz->alimento->numero,
               raiz->alimento->descricao,
               raiz->alimento->energia_kcal,
               raiz->alimento->proteina);
    }

    if (raiz->chave < max) {
        percorrer_intervalo(raiz->direita, min, max);
    }
}

/* Libera toda a memória alocada pela árvore */
void liberar_arvore(NoArvore* raiz) {
    if (raiz == NULL) {
        return;
    }

    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}

/* Função auxiliar para encontrar o menor nó (mais à esquerda) */
static NoArvore* encontrar_minimo(NoArvore* raiz) {
    NoArvore* atual = raiz;
    while (atual != NULL && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

/* Remove um alimento específico da árvore e retorna a nova raiz */
NoArvore* remover_da_arvore(NoArvore* raiz, NoAlimento* alimento) {
    if (raiz == NULL) {
        return NULL;
    }

    if (raiz->alimento == alimento) {
        /* Caso 1: Nó sem filhos */
        if (raiz->esquerda == NULL && raiz->direita == NULL) {
            free(raiz);
            return NULL;
        }

        /* Caso 2: Nó com apenas um filho */
        if (raiz->esquerda == NULL) {
            NoArvore* temp = raiz->direita;
            free(raiz);
            return temp;
        }
        if (raiz->direita == NULL) {
            NoArvore* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        /* Caso 3: Nó com dois filhos */
        NoArvore* sucessor = encontrar_minimo(raiz->direita);
        raiz->chave = sucessor->chave;
        raiz->alimento = sucessor->alimento;
        raiz->direita = remover_da_arvore(raiz->direita, sucessor->alimento);
        return raiz;
    }

    raiz->esquerda = remover_da_arvore(raiz->esquerda, alimento);
    raiz->direita = remover_da_arvore(raiz->direita, alimento);
    return raiz;
}
