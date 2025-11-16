#ifndef MENU_H
#define MENU_H

#include "tipos.h"

/* Exibe o menu principal e retorna a opção escolhida */
int exibir_menu();

/* Executa a opção 1: Listar todas as categorias */
void opcao_listar_categorias(NoCategoria* lista);

/* Executa a opção 2: Listar alimentos de uma categoria */
void opcao_listar_alimentos(NoCategoria* lista);

/* Executa a opção 3: Listar alimentos por energia (decrescente) */
void opcao_listar_por_energia(NoCategoria* lista);

/* Executa a opção 4: Listar alimentos por proteína (decrescente) */
void opcao_listar_por_proteina(NoCategoria* lista);

/* Executa a opção 5: Listar alimentos por intervalo de energia */
void opcao_intervalo_energia(NoCategoria* lista);

/* Executa a opção 6: Listar alimentos por intervalo de proteína */
void opcao_intervalo_proteina(NoCategoria* lista);

/* Executa a opção 7: Remover uma categoria */
NoCategoria* opcao_remover_categoria(NoCategoria* lista, bool* modificado);

/* Executa a opção 8: Remover um alimento */
void opcao_remover_alimento(NoCategoria* lista, bool* modificado);

#endif
