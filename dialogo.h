/******************************************************************************
 * DIALOGO.H - Sistema de Caixas de Diálogo Personalizadas
 *
 * Este módulo implementa três tipos de diálogos personalizados usando Win32 API:
 * 1. Diálogo de entrada de texto genérico
 * 2. Diálogo de entrada de números (usa o de texto + conversão)
 * 3. Diálogo de seleção de categoria com listbox
 *
 * Todos os diálogos são modais e bloqueiam a janela pai até serem fechados.
 *****************************************************************************/

#ifndef DIALOGO_H
#define DIALOGO_H

#include <windows.h>
#include <stdbool.h>

/******************************************************************************
 * ESTRUTURAS DE DADOS
 *****************************************************************************/

/* Estrutura para passar parâmetros para o diálogo de texto */
typedef struct {
    const char* titulo;      // Título da janela do diálogo
    const char* mensagem;    // Mensagem/pergunta mostrada ao usuário
    char texto[256];         // Buffer para armazenar o texto digitado
} DadosDialogoTexto;

/* Estrutura para passar parâmetros para o diálogo de categoria */
typedef struct {
    void* lista_categorias;        // Ponteiro para lista de categorias (NoCategoria*)
    char categoria_selecionada[256]; // Nome da categoria selecionada
} DadosDialogoCategoria;

/******************************************************************************
 * FUNÇÕES PÚBLICAS
 *****************************************************************************/

/**
 * MostrarDialogoTexto - Mostra um diálogo modal para entrada de texto
 *
 * @param hwndParent: Janela pai (será bloqueada enquanto diálogo está aberto)
 * @param titulo: Título da janela do diálogo
 * @param mensagem: Mensagem/pergunta a ser exibida
 * @param buffer: Buffer onde o texto digitado será armazenado
 * @param tamanho: Tamanho máximo do buffer
 *
 * @return TRUE se usuário clicou OK, FALSE se cancelou
 */
BOOL MostrarDialogoTexto(HWND hwndParent, const char* titulo,
                         const char* mensagem, char* buffer, int tamanho);

/**
 * MostrarDialogoNumero - Mostra um diálogo modal para entrada de número
 *
 * Internamente usa MostrarDialogoTexto e converte o texto para double
 *
 * @param hwndParent: Janela pai
 * @param titulo: Título da janela
 * @param mensagem: Mensagem/pergunta
 * @param valor: Ponteiro onde o valor digitado será armazenado
 *
 * @return TRUE se usuário digitou e clicou OK, FALSE se cancelou
 */
BOOL MostrarDialogoNumero(HWND hwndParent, const char* titulo,
                          const char* mensagem, double* valor);

/**
 * MostrarDialogoCategoria - Mostra um diálogo com listbox para seleção de categoria
 *
 * @param hwndParent: Janela pai
 * @param lista_categorias: Ponteiro para a lista encadeada de categorias
 * @param categoria_selecionada: Buffer onde o nome da categoria será armazenado
 *
 * @return TRUE se usuário selecionou e clicou OK, FALSE se cancelou
 */
BOOL MostrarDialogoCategoria(HWND hwndParent, void* lista_categorias,
                             char* categoria_selecionada);

#endif /* DIALOGO_H */
