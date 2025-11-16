#ifndef TIPOS_H
#define TIPOS_H

#include <stdbool.h>

#define MAX_DESCRICAO 100
#define MAX_CATEGORIA 50

/* Enumerado para as 15 categorias de alimentos conforme especificacao */
typedef enum {
    ALIMENTOS_PREPARADOS,
    BEBIDAS,
    CARNES_E_DERIVADOS,
    CEREAIS_E_DERIVADOS,
    FRUTAS_E_DERIVADOS,
    GORDURAS_E_OLEOS,
    LEGUMINOSAS_E_DERIVADOS,
    LEITE_E_DERIVADOS,
    MISCELANEAS,
    NOZES_E_SEMENTES,
    OUTROS_ALIMENTOS_INDUSTRIALIZADOS,
    OVOS_E_DERIVADOS,
    PESCADOS_E_FRUTOS_DO_MAR,
    PRODUTOS_ACUCARADOS,
    VERDURAS_HORTALICAS_E_DERIVADOS
} TipoCategoria;

/* Estrutura para armazenar alimento lido do arquivo binario */
typedef struct {
    int numero;
    char descricao[MAX_DESCRICAO];
    double umidade;
    int energia_kcal;
    double proteina;
    double carboidrato;
    char categoria[MAX_CATEGORIA];
} AlimentoArquivo;

/* No de alimento em lista ligada */
typedef struct NoAlimento {
    int numero;
    char descricao[MAX_DESCRICAO];
    double umidade;
    int energia_kcal;
    double proteina;
    double carboidrato;
    struct NoAlimento* proximo;
} NoAlimento;

/* No de categoria em lista ligada */
typedef struct NoCategoria {
    char nome[MAX_CATEGORIA];
    TipoCategoria tipo;
    NoAlimento* lista_alimentos;
    struct NoArvore* arvore_energia;
    struct NoArvore* arvore_proteina;
    struct NoCategoria* proximo;
} NoCategoria;

/* No de arvore binaria de indexacao */
typedef struct NoArvore {
    double chave;
    NoAlimento* alimento;
    struct NoArvore* esquerda;
    struct NoArvore* direita;
} NoArvore;

#endif
