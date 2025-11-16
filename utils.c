#include "utils.h"
#include <string.h>

/* Converte string de categoria para o tipo enumerado */
TipoCategoria string_para_categoria(const char* nome_categoria) {
    if (strcmp(nome_categoria, "Alimentos preparados") == 0)
        return ALIMENTOS_PREPARADOS;
    if (strcmp(nome_categoria, "Bebidas (alcoolicas e nao alcoolicas)") == 0 ||
        strcmp(nome_categoria, "Bebidas") == 0)
        return BEBIDAS;
    if (strcmp(nome_categoria, "Carnes e derivados") == 0)
        return CARNES_E_DERIVADOS;
    if (strcmp(nome_categoria, "Cereais e derivados") == 0)
        return CEREAIS_E_DERIVADOS;
    if (strcmp(nome_categoria, "Frutas e derivados") == 0)
        return FRUTAS_E_DERIVADOS;
    if (strcmp(nome_categoria, "Gorduras e oleos") == 0)
        return GORDURAS_E_OLEOS;
    if (strcmp(nome_categoria, "Leguminosas e derivados") == 0)
        return LEGUMINOSAS_E_DERIVADOS;
    if (strcmp(nome_categoria, "Leite e derivados") == 0)
        return LEITE_E_DERIVADOS;
    if (strcmp(nome_categoria, "Miscelaneas") == 0)
        return MISCELANEAS;
    if (strcmp(nome_categoria, "Nozes e sementes") == 0)
        return NOZES_E_SEMENTES;
    if (strcmp(nome_categoria, "Outros alimentos industrializados") == 0)
        return OUTROS_ALIMENTOS_INDUSTRIALIZADOS;
    if (strcmp(nome_categoria, "Ovos e derivados") == 0)
        return OVOS_E_DERIVADOS;
    if (strcmp(nome_categoria, "Pescados e frutos do mar") == 0)
        return PESCADOS_E_FRUTOS_DO_MAR;
    if (strcmp(nome_categoria, "Produtos acucarados") == 0)
        return PRODUTOS_ACUCARADOS;
    if (strcmp(nome_categoria, "Verduras, hortalicas e derivados") == 0)
        return VERDURAS_HORTALICAS_E_DERIVADOS;

    return CEREAIS_E_DERIVADOS;
}

/* Converte tipo enumerado para string de categoria */
const char* categoria_para_string(TipoCategoria tipo) {
    switch(tipo) {
        case ALIMENTOS_PREPARADOS: return "Alimentos preparados";
        case BEBIDAS: return "Bebidas (alcoolicas e nao alcoolicas)";
        case CARNES_E_DERIVADOS: return "Carnes e derivados";
        case CEREAIS_E_DERIVADOS: return "Cereais e derivados";
        case FRUTAS_E_DERIVADOS: return "Frutas e derivados";
        case GORDURAS_E_OLEOS: return "Gorduras e oleos";
        case LEGUMINOSAS_E_DERIVADOS: return "Leguminosas e derivados";
        case LEITE_E_DERIVADOS: return "Leite e derivados";
        case MISCELANEAS: return "Miscelaneas";
        case NOZES_E_SEMENTES: return "Nozes e sementes";
        case OUTROS_ALIMENTOS_INDUSTRIALIZADOS: return "Outros alimentos industrializados";
        case OVOS_E_DERIVADOS: return "Ovos e derivados";
        case PESCADOS_E_FRUTOS_DO_MAR: return "Pescados e frutos do mar";
        case PRODUTOS_ACUCARADOS: return "Produtos acucarados";
        case VERDURAS_HORTALICAS_E_DERIVADOS: return "Verduras, hortalicas e derivados";
        default: return "Categoria desconhecida";
    }
}
