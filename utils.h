#ifndef UTILS_H
#define UTILS_H

#include "tipos.h"

/* Converte string de categoria para o tipo enumerado */
TipoCategoria string_para_categoria(const char* nome_categoria);

/* Converte tipo enumerado para string de categoria */
const char* categoria_para_string(TipoCategoria tipo);

#endif
