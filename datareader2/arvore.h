#ifndef ARVORE_H
#define ARVORE_H

#include "tipos.h"

/* Cria um novo no de arvore binaria */
NoArvore* criar_no_arvore(double chave, NoAlimento* alimento);

/* Insere um no na arvore binaria mantendo a propriedade de BST */
NoArvore* inserir_na_arvore(NoArvore* raiz, double chave, NoAlimento* alimento);

/* Percorre a arvore em ordem decrescente (direita-raiz-esquerda) */
void percorrer_decrescente(NoArvore* raiz);

/* Percorre a arvore buscando valores em um intervalo [min, max] */
void percorrer_intervalo(NoArvore* raiz, double min, double max);

/* Libera toda a memoria alocada pela arvore */
void liberar_arvore(NoArvore* raiz);

/* Remove um alimento especifico da arvore e retorna a nova raiz */
NoArvore* remover_da_arvore(NoArvore* raiz, NoAlimento* alimento);

#endif
