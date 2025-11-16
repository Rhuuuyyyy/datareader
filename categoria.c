#include "categoria.h"
#include "arvore.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Cria um novo nó de alimento */
NoAlimento* criar_no_alimento(AlimentoArquivo* alimento_arquivo) {
    NoAlimento* novo = (NoAlimento*)malloc(sizeof(NoAlimento));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para no de alimento.\n");
        exit(1);
    }

    novo->numero = alimento_arquivo->numero;
    strncpy(novo->descricao, alimento_arquivo->descricao, MAX_DESCRICAO - 1);
    novo->descricao[MAX_DESCRICAO - 1] = '\0';
    novo->umidade = alimento_arquivo->umidade;
    novo->energia_kcal = alimento_arquivo->energia_kcal;
    novo->proteina = alimento_arquivo->proteina;
    novo->carboidrato = alimento_arquivo->carboidrato;
    novo->proximo = NULL;

    return novo;
}

/* Insere um alimento em ordem alfabética na lista de alimentos */
NoAlimento* inserir_alimento_ordenado(NoAlimento* lista, NoAlimento* novo_alimento) {
    if (lista == NULL || strcmp(novo_alimento->descricao, lista->descricao) < 0) {
        novo_alimento->proximo = lista;
        return novo_alimento;
    }

    NoAlimento* atual = lista;
    while (atual->proximo != NULL && strcmp(novo_alimento->descricao, atual->proximo->descricao) > 0) {
        atual = atual->proximo;
    }

    novo_alimento->proximo = atual->proximo;
    atual->proximo = novo_alimento;
    return lista;
}

/* Cria um novo nó de categoria */
NoCategoria* criar_no_categoria(const char* nome) {
    NoCategoria* nova = (NoCategoria*)malloc(sizeof(NoCategoria));
    if (nova == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para no de categoria.\n");
        exit(1);
    }

    strncpy(nova->nome, nome, MAX_CATEGORIA - 1);
    nova->nome[MAX_CATEGORIA - 1] = '\0';
    nova->tipo = string_para_categoria(nome);
    nova->lista_alimentos = NULL;
    nova->arvore_energia = NULL;
    nova->arvore_proteina = NULL;
    nova->proximo = NULL;

    return nova;
}

/* Insere uma categoria em ordem alfabética na lista de categorias */
NoCategoria* inserir_categoria_ordenada(NoCategoria* lista, NoCategoria* nova_categoria) {
    if (lista == NULL || strcmp(nova_categoria->nome, lista->nome) < 0) {
        nova_categoria->proximo = lista;
        return nova_categoria;
    }

    NoCategoria* atual = lista;
    while (atual->proximo != NULL && strcmp(nova_categoria->nome, atual->proximo->nome) > 0) {
        atual = atual->proximo;
    }

    nova_categoria->proximo = atual->proximo;
    atual->proximo = nova_categoria;
    return lista;
}

/* Busca uma categoria pelo nome */
NoCategoria* buscar_categoria(NoCategoria* lista, const char* nome) {
    NoCategoria* atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

/* Constrói as árvores binárias de indexação para uma categoria */
void construir_arvores_categoria(NoCategoria* categoria) {
    if (categoria == NULL) {
        return;
    }

    NoAlimento* atual = categoria->lista_alimentos;
    while (atual != NULL) {
        categoria->arvore_energia = inserir_na_arvore(categoria->arvore_energia,
                                                      (double)atual->energia_kcal,
                                                      atual);
        categoria->arvore_proteina = inserir_na_arvore(categoria->arvore_proteina,
                                                       atual->proteina,
                                                       atual);
        atual = atual->proximo;
    }
}

/* Reconstrói as árvores binárias de uma categoria */
void reconstruir_arvores_categoria(NoCategoria* categoria) {
    if (categoria == NULL) {
        return;
    }

    liberar_arvore(categoria->arvore_energia);
    liberar_arvore(categoria->arvore_proteina);
    categoria->arvore_energia = NULL;
    categoria->arvore_proteina = NULL;

    construir_arvores_categoria(categoria);
}

/* Remove um alimento de uma categoria */
void remover_alimento_de_categoria(NoCategoria* categoria, int numero_alimento) {
    if (categoria == NULL || categoria->lista_alimentos == NULL) {
        return;
    }

    NoAlimento* atual = categoria->lista_alimentos;
    NoAlimento* anterior = NULL;

    while (atual != NULL && atual->numero != numero_alimento) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Alimento nao encontrado.\n");
        return;
    }

    if (anterior == NULL) {
        categoria->lista_alimentos = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    reconstruir_arvores_categoria(categoria);
    printf("Alimento removido com sucesso.\n");
}

/* Remove uma categoria da lista e retorna a nova lista */
NoCategoria* remover_categoria(NoCategoria* lista, const char* nome) {
    if (lista == NULL) {
        return NULL;
    }

    NoCategoria* atual = lista;
    NoCategoria* anterior = NULL;

    while (atual != NULL && strcmp(atual->nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Categoria nao encontrada.\n");
        return lista;
    }

    if (anterior == NULL) {
        lista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    liberar_alimentos(atual->lista_alimentos);
    liberar_arvore(atual->arvore_energia);
    liberar_arvore(atual->arvore_proteina);
    free(atual);

    printf("Categoria removida com sucesso.\n");
    return lista;
}

/* Libera a memória de todos os alimentos de uma categoria */
void liberar_alimentos(NoAlimento* lista) {
    NoAlimento* atual = lista;
    while (atual != NULL) {
        NoAlimento* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}

/* Libera a memória de todas as categorias */
void liberar_categorias(NoCategoria* lista) {
    NoCategoria* atual = lista;
    while (atual != NULL) {
        NoCategoria* proximo = atual->proximo;
        liberar_alimentos(atual->lista_alimentos);
        liberar_arvore(atual->arvore_energia);
        liberar_arvore(atual->arvore_proteina);
        free(atual);
        atual = proximo;
    }
}

/* Lista todas as categorias */
void listar_categorias(NoCategoria* lista) {
    if (lista == NULL) {
        printf("Nenhuma categoria cadastrada.\n");
        return;
    }

    printf("\n=== CATEGORIAS DE ALIMENTOS ===\n");
    int contador = 1;
    NoCategoria* atual = lista;
    while (atual != NULL) {
        printf("%2d. %s\n", contador, atual->nome);
        contador++;
        atual = atual->proximo;
    }
    printf("\n");
}

/* Lista todos os alimentos de uma categoria */
void listar_alimentos_categoria(NoCategoria* categoria) {
    if (categoria == NULL) {
        printf("Categoria nao encontrada.\n");
        return;
    }

    if (categoria->lista_alimentos == NULL) {
        printf("Nenhum alimento nesta categoria.\n");
        return;
    }

    printf("\n=== ALIMENTOS DA CATEGORIA: %s ===\n", categoria->nome);
    printf("  Num | %-50s | Energia | Proteina\n", "Descricao");
    printf("  %s\n", "----------------------------------------------------------------------");

    NoAlimento* atual = categoria->lista_alimentos;
    while (atual != NULL) {
        printf("  %3d | %-50s | %4d kcal | %5.1f g\n",
               atual->numero,
               atual->descricao,
               atual->energia_kcal,
               atual->proteina);
        atual = atual->proximo;
    }
    printf("\n");
}
