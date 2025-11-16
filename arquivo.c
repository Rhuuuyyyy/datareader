#include "arquivo.h"
#include "categoria.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Le o arquivo binario e constroi as listas ligadas de categorias e alimentos */
NoCategoria* carregar_dados_binario(const char* arquivo_bin) {
    FILE* arquivo = fopen(arquivo_bin, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo binario");
        return NULL;
    }

    NoCategoria* lista_categorias = NULL;
    AlimentoArquivo alimento_temp;

    while (fread(&alimento_temp, sizeof(AlimentoArquivo), 1, arquivo) == 1) {
        NoCategoria* categoria = buscar_categoria(lista_categorias, alimento_temp.categoria);

        if (categoria == NULL) {
            categoria = criar_no_categoria(alimento_temp.categoria);
            lista_categorias = inserir_categoria_ordenada(lista_categorias, categoria);
        }

        NoAlimento* novo_alimento = criar_no_alimento(&alimento_temp);
        categoria->lista_alimentos = inserir_alimento_ordenado(categoria->lista_alimentos, novo_alimento);
    }

    fclose(arquivo);

    NoCategoria* atual = lista_categorias;
    while (atual != NULL) {
        construir_arvores_categoria(atual);
        atual = atual->proximo;
    }

    return lista_categorias;
}

/* Salva os dados atualizados no arquivo binario */
bool salvar_dados_binario(NoCategoria* lista_categorias, const char* arquivo_bin) {
    FILE* arquivo = fopen(arquivo_bin, "wb");
    if (arquivo == NULL) {
        perror("Erro ao criar arquivo binario");
        return false;
    }

    NoCategoria* cat_atual = lista_categorias;
    while (cat_atual != NULL) {
        NoAlimento* alim_atual = cat_atual->lista_alimentos;
        while (alim_atual != NULL) {
            AlimentoArquivo temp;
            memset(&temp, 0, sizeof(AlimentoArquivo));

            temp.numero = alim_atual->numero;
            temp.umidade = alim_atual->umidade;
            temp.energia_kcal = alim_atual->energia_kcal;
            temp.proteina = alim_atual->proteina;
            temp.carboidrato = alim_atual->carboidrato;

            strncpy(temp.descricao, alim_atual->descricao, MAX_DESCRICAO - 1);
            strncpy(temp.categoria, cat_atual->nome, MAX_CATEGORIA - 1);

            fwrite(&temp, sizeof(AlimentoArquivo), 1, arquivo);
            alim_atual = alim_atual->proximo;
        }
        cat_atual = cat_atual->proximo;
    }

    fclose(arquivo);
    return true;
}
