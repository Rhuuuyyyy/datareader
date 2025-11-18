#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "tipos.h"

/* Le o arquivo binario e constroi as listas ligadas de categorias e alimentos */
NoCategoria* carregar_dados_binario(const char* arquivo_bin);

/* Salva os dados atualizados no arquivo binario */
bool salvar_dados_binario(NoCategoria* lista_categorias, const char* arquivo_bin);

#endif
