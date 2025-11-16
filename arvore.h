#ifndef ARVORE_H
#define ARVORE_H

#include "tipos.h"

/* Cria um novo nó de árvore binária */
NoArvore* criar_no_arvore(double chave, NoAlimento* alimento);

/* Insere um nó na árvore binária mantendo a propriedade de BST */
NoArvore* inserir_na_arvore(NoArvore* raiz, double chave, NoAlimento* alimento);

/* Percorre a árvore em ordem decrescente (direita-raiz-esquerda) */
void percorrer_decrescente(NoArvore* raiz);

/* Percorre a árvore buscando valores em um intervalo [min, max] */
void percorrer_intervalo(NoArvore* raiz, double min, double max);

/* Libera toda a memória alocada pela árvore */
void liberar_arvore(NoArvore* raiz);

/* Remove um alimento específico da árvore e retorna a nova raiz */
NoArvore* remover_da_arvore(NoArvore* raiz, NoAlimento* alimento);

#endif
