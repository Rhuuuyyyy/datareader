#include "menu.h"
#include "categoria.h"
#include "arvore.h"
#include <stdio.h>
#include <string.h>

/* Exibe o menu principal e retorna a opção escolhida */
int exibir_menu() {
    int opcao = 0;

    printf("\n========================================\n");
    printf("   SISTEMA DE GERENCIAMENTO DE ALIMENTOS\n");
    printf("========================================\n");
    printf("1. Listar todas as categorias\n");
    printf("2. Listar alimentos de uma categoria\n");
    printf("3. Listar alimentos por energia (decrescente)\n");
    printf("4. Listar alimentos por proteina (decrescente)\n");
    printf("5. Listar alimentos por intervalo de energia\n");
    printf("6. Listar alimentos por intervalo de proteina\n");
    printf("7. Remover uma categoria\n");
    printf("8. Remover um alimento\n");
    printf("9. Sair\n");
    printf("========================================\n");
    printf("Escolha uma opcao: ");

    if (scanf("%d", &opcao) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');

    return opcao;
}

/* Executa a opção 1: Listar todas as categorias */
void opcao_listar_categorias(NoCategoria* lista) {
    listar_categorias(lista);
}

/* Executa a opção 2: Listar alimentos de uma categoria */
void opcao_listar_alimentos(NoCategoria* lista) {
    char nome_categoria[MAX_CATEGORIA];

    printf("\nDigite o nome da categoria: ");
    if (fgets(nome_categoria, MAX_CATEGORIA, stdin) == NULL) {
        printf("Erro ao ler entrada.\n");
        return;
    }

    size_t len = strlen(nome_categoria);
    if (len > 0 && nome_categoria[len - 1] == '\n') {
        nome_categoria[len - 1] = '\0';
    }

    NoCategoria* categoria = buscar_categoria(lista, nome_categoria);
    listar_alimentos_categoria(categoria);
}

/* Executa a opção 3: Listar alimentos por energia (decrescente) */
void opcao_listar_por_energia(NoCategoria* lista) {
    char nome_categoria[MAX_CATEGORIA];

    printf("\nDigite o nome da categoria: ");
    if (fgets(nome_categoria, MAX_CATEGORIA, stdin) == NULL) {
        printf("Erro ao ler entrada.\n");
        return;
    }

    size_t len = strlen(nome_categoria);
    if (len > 0 && nome_categoria[len - 1] == '\n') {
        nome_categoria[len - 1] = '\0';
    }

    NoCategoria* categoria = buscar_categoria(lista, nome_categoria);
    if (categoria == NULL) {
        printf("Categoria nao encontrada.\n");
        return;
    }

    if (categoria->arvore_energia == NULL) {
        printf("Nenhum alimento nesta categoria.\n");
        return;
    }

    printf("\n=== ALIMENTOS POR ENERGIA (Decrescente): %s ===\n", categoria->nome);
    printf("  Num | %-50s | Energia | Proteina\n", "Descricao");
    printf("  %s\n", "----------------------------------------------------------------------");
    percorrer_decrescente(categoria->arvore_energia);
    printf("\n");
}

/* Executa a opção 4: Listar alimentos por proteína (decrescente) */
void opcao_listar_por_proteina(NoCategoria* lista) {
    char nome_categoria[MAX_CATEGORIA];

    printf("\nDigite o nome da categoria: ");
    if (fgets(nome_categoria, MAX_CATEGORIA, stdin) == NULL) {
        printf("Erro ao ler entrada.\n");
        return;
    }

    size_t len = strlen(nome_categoria);
    if (len > 0 && nome_categoria[len - 1] == '\n') {
        nome_categoria[len - 1] = '\0';
    }

    NoCategoria* categoria = buscar_categoria(lista, nome_categoria);
    if (categoria == NULL) {
        printf("Categoria nao encontrada.\n");
        return;
    }

    if (categoria->arvore_proteina == NULL) {
        printf("Nenhum alimento nesta categoria.\n");
        return;
    }

    printf("\n=== ALIMENTOS POR PROTEINA (Decrescente): %s ===\n", categoria->nome);
    printf("  Num | %-50s | Energia | Proteina\n", "Descricao");
    printf("  %s\n", "----------------------------------------------------------------------");
    percorrer_decrescente(categoria->arvore_proteina);
    printf("\n");
}

/* Executa a opção 5: Listar alimentos por intervalo de energia */
void opcao_intervalo_energia(NoCategoria* lista) {
    char nome_categoria[MAX_CATEGORIA];
    double min = 0.0;
    double max = 0.0;

    printf("\nDigite o nome da categoria: ");
    if (fgets(nome_categoria, MAX_CATEGORIA, stdin) == NULL) {
        printf("Erro ao ler entrada.\n");
        return;
    }

    size_t len = strlen(nome_categoria);
    if (len > 0 && nome_categoria[len - 1] == '\n') {
        nome_categoria[len - 1] = '\0';
    }

    NoCategoria* categoria = buscar_categoria(lista, nome_categoria);
    if (categoria == NULL) {
        printf("Categoria nao encontrada.\n");
        return;
    }

    printf("Digite o valor minimo de energia (kcal): ");
    if (scanf("%lf", &min) != 1) {
        printf("Valor invalido.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Digite o valor maximo de energia (kcal): ");
    if (scanf("%lf", &max) != 1) {
        printf("Valor invalido.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    if (categoria->arvore_energia == NULL) {
        printf("Nenhum alimento nesta categoria.\n");
        return;
    }

    printf("\n=== ALIMENTOS COM ENERGIA ENTRE %.1f e %.1f kcal: %s ===\n", min, max, categoria->nome);
    printf("  Num | %-50s | Energia | Proteina\n", "Descricao");
    printf("  %s\n", "----------------------------------------------------------------------");
    percorrer_intervalo(categoria->arvore_energia, min, max);
    printf("\n");
}

/* Executa a opção 6: Listar alimentos por intervalo de proteína */
void opcao_intervalo_proteina(NoCategoria* lista) {
    char nome_categoria[MAX_CATEGORIA];
    double min = 0.0;
    double max = 0.0;

    printf("\nDigite o nome da categoria: ");
    if (fgets(nome_categoria, MAX_CATEGORIA, stdin) == NULL) {
        printf("Erro ao ler entrada.\n");
        return;
    }

    size_t len = strlen(nome_categoria);
    if (len > 0 && nome_categoria[len - 1] == '\n') {
        nome_categoria[len - 1] = '\0';
    }

    NoCategoria* categoria = buscar_categoria(lista, nome_categoria);
    if (categoria == NULL) {
        printf("Categoria nao encontrada.\n");
        return;
    }

    printf("Digite o valor minimo de proteina (g): ");
    if (scanf("%lf", &min) != 1) {
        printf("Valor invalido.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Digite o valor maximo de proteina (g): ");
    if (scanf("%lf", &max) != 1) {
        printf("Valor invalido.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    if (categoria->arvore_proteina == NULL) {
        printf("Nenhum alimento nesta categoria.\n");
        return;
    }

    printf("\n=== ALIMENTOS COM PROTEINA ENTRE %.1f e %.1f g: %s ===\n", min, max, categoria->nome);
    printf("  Num | %-50s | Energia | Proteina\n", "Descricao");
    printf("  %s\n", "----------------------------------------------------------------------");
    percorrer_intervalo(categoria->arvore_proteina, min, max);
    printf("\n");
}

/* Executa a opção 7: Remover uma categoria */
NoCategoria* opcao_remover_categoria(NoCategoria* lista, bool* modificado) {
    char nome_categoria[MAX_CATEGORIA];

    printf("\nDigite o nome da categoria a remover: ");
    if (fgets(nome_categoria, MAX_CATEGORIA, stdin) == NULL) {
        printf("Erro ao ler entrada.\n");
        return lista;
    }

    size_t len = strlen(nome_categoria);
    if (len > 0 && nome_categoria[len - 1] == '\n') {
        nome_categoria[len - 1] = '\0';
    }

    NoCategoria* nova_lista = remover_categoria(lista, nome_categoria);
    if (nova_lista != lista || buscar_categoria(lista, nome_categoria) == NULL) {
        *modificado = true;
    }
    return nova_lista;
}

/* Executa a opção 8: Remover um alimento */
void opcao_remover_alimento(NoCategoria* lista, bool* modificado) {
    char nome_categoria[MAX_CATEGORIA];
    int numero_alimento = 0;

    printf("\nDigite o nome da categoria do alimento: ");
    if (fgets(nome_categoria, MAX_CATEGORIA, stdin) == NULL) {
        printf("Erro ao ler entrada.\n");
        return;
    }

    size_t len = strlen(nome_categoria);
    if (len > 0 && nome_categoria[len - 1] == '\n') {
        nome_categoria[len - 1] = '\0';
    }

    NoCategoria* categoria = buscar_categoria(lista, nome_categoria);
    if (categoria == NULL) {
        printf("Categoria nao encontrada.\n");
        return;
    }

    printf("Digite o numero do alimento a remover: ");
    if (scanf("%d", &numero_alimento) != 1) {
        printf("Numero invalido.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    remover_alimento_de_categoria(categoria, numero_alimento);
    *modificado = true;
}
