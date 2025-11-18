#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"
#include "arquivo.h"
#include "categoria.h"
#include "menu.h"

int main() {
    const char* arquivo_bin = "dados.bin";
    bool dados_modificados = false;

    printf("=== PROGRAMA P2 - Sistema de Gerenciamento de Alimentos ===\n\n");
    printf("Carregando dados do arquivo '%s'...\n", arquivo_bin);

    NoCategoria* lista_categorias = carregar_dados_binario(arquivo_bin);

    if (lista_categorias == NULL) {
        fprintf(stderr, "Falha ao carregar dados do arquivo binario.\n");
        fprintf(stderr, "Certifique-se de que o arquivo '%s' existe.\n", arquivo_bin);
        fprintf(stderr, "Execute o programa P1 primeiro para gerar o arquivo.\n");
        exit(1);
    }

    printf("Dados carregados com sucesso!\n");

    int opcao = 0;
    int sair = 0;

    while (sair == 0) {
        opcao = exibir_menu();

        if (opcao == 1) {
            opcao_listar_categorias(lista_categorias);
        } else if (opcao == 2) {
            opcao_listar_alimentos(lista_categorias);
        } else if (opcao == 3) {
            opcao_listar_por_energia(lista_categorias);
        } else if (opcao == 4) {
            opcao_listar_por_proteina(lista_categorias);
        } else if (opcao == 5) {
            opcao_intervalo_energia(lista_categorias);
        } else if (opcao == 6) {
            opcao_intervalo_proteina(lista_categorias);
        } else if (opcao == 7) {
            lista_categorias = opcao_remover_categoria(lista_categorias, &dados_modificados);
        } else if (opcao == 8) {
            opcao_remover_alimento(lista_categorias, &dados_modificados);
        } else if (opcao == 9) {
            sair = 1;
        } else {
            printf("\nOpcao invalida! Tente novamente.\n");
        }
    }

    if (dados_modificados) {
        printf("\nDados foram modificados. Salvando arquivo atualizado...\n");
        if (salvar_dados_binario(lista_categorias, arquivo_bin)) {
            printf("Arquivo '%s' atualizado com sucesso!\n", arquivo_bin);
        } else {
            fprintf(stderr, "Erro ao salvar o arquivo atualizado.\n");
        }
    }

    printf("\nLiberando memoria...\n");
    liberar_categorias(lista_categorias);

    printf("Programa encerrado. Ate logo!\n");
    return 0;
}
