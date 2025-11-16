#ifndef MENU_H
#define MENU_H

#include "tipos.h"

/* Exibe o menu principal e retorna a opcao escolhida */
int exibir_menu();

/* Executa a opcao 1: Listar todas as categorias */
void opcao_listar_categorias(NoCategoria* lista);

/* Executa a opcao 2: Listar alimentos de uma categoria */
void opcao_listar_alimentos(NoCategoria* lista);

/* Executa a opcao 3: Listar alimentos por energia (decrescente) */
void opcao_listar_por_energia(NoCategoria* lista);

/* Executa a opcao 4: Listar alimentos por proteina (decrescente) */
void opcao_listar_por_proteina(NoCategoria* lista);

/* Executa a opcao 5: Listar alimentos por intervalo de energia */
void opcao_intervalo_energia(NoCategoria* lista);

/* Executa a opcao 6: Listar alimentos por intervalo de proteina */
void opcao_intervalo_proteina(NoCategoria* lista);

/* Executa a opcao 7: Remover uma categoria */
NoCategoria* opcao_remover_categoria(NoCategoria* lista, bool* modificado);

/* Executa a opcao 8: Remover um alimento */
void opcao_remover_alimento(NoCategoria* lista, bool* modificado);

#endif
