#ifndef CATEGORIA_H
#define CATEGORIA_H

#include "tipos.h"

/* Cria um novo no de alimento */
NoAlimento* criar_no_alimento(AlimentoArquivo* alimento_arquivo);

/* Insere um alimento em ordem alfabetica na lista de alimentos */
NoAlimento* inserir_alimento_ordenado(NoAlimento* lista, NoAlimento* novo_alimento);

/* Cria um novo no de categoria */
NoCategoria* criar_no_categoria(const char* nome);

/* Insere uma categoria em ordem alfabetica na lista de categorias */
NoCategoria* inserir_categoria_ordenada(NoCategoria* lista, NoCategoria* nova_categoria);

/* Busca uma categoria pelo nome */
NoCategoria* buscar_categoria(NoCategoria* lista, const char* nome);

/* Constroi as arvores binarias de indexacao para uma categoria */
void construir_arvores_categoria(NoCategoria* categoria);

/* Reconstroi as arvores binarias de uma categoria */
void reconstruir_arvores_categoria(NoCategoria* categoria);

/* Remove um alimento de uma categoria */
void remover_alimento_de_categoria(NoCategoria* categoria, int numero_alimento);

/* Remove uma categoria da lista e retorna a nova lista */
NoCategoria* remover_categoria(NoCategoria* lista, const char* nome);

/* Libera a memoria de todos os alimentos de uma categoria */
void liberar_alimentos(NoAlimento* lista);

/* Libera a memoria de todas as categorias */
void liberar_categorias(NoCategoria* lista);

/* Lista todas as categorias */
void listar_categorias(NoCategoria* lista);

/* Lista todos os alimentos de uma categoria */
void listar_alimentos_categoria(NoCategoria* categoria);

#endif
