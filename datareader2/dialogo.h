/******************************************************************************
 * DIALOGO.H - Sistema de Caixas de Dialogo Personalizadas
 *
 * Este modulo implementa tres tipos de dialogos personalizados usando Win32 API:
 * 1. Dialogo de entrada de texto generico
 * 2. Dialogo de entrada de numeros (usa o de texto + conversao)
 * 3. Dialogo de selecao de categoria com listbox
 *
 * Todos os dialogos sao modais e bloqueiam a janela pai ate serem fechados.
 *****************************************************************************/

#ifndef DIALOGO_H
#define DIALOGO_H

#include <windows.h>
#include <stdbool.h>

/******************************************************************************
 * ESTRUTURAS DE DADOS
 *****************************************************************************/

/* Estrutura para passar parametros para o dialogo de texto */
typedef struct {
    const char* titulo;      // Titulo da janela do dialogo
    const char* mensagem;    // Mensagem/pergunta mostrada ao usuario
    char texto[256];         // Buffer para armazenar o texto digitado
} DadosDialogoTexto;

/* Estrutura para passar parametros para o dialogo de categoria */
typedef struct {
    void* lista_categorias;        // Ponteiro para lista de categorias (NoCategoria*)
    char categoria_selecionada[256]; // Nome da categoria selecionada
} DadosDialogoCategoria;

/******************************************************************************
 * FUNCOES PUBLICAS
 *****************************************************************************/

/**
 * MostrarDialogoTexto - Mostra um dialogo modal para entrada de texto
 *
 * @param hwndParent: Janela pai (sera bloqueada enquanto dialogo esta aberto)
 * @param titulo: Titulo da janela do dialogo
 * @param mensagem: Mensagem/pergunta a ser exibida
 * @param buffer: Buffer onde o texto digitado sera armazenado
 * @param tamanho: Tamanho maximo do buffer
 *
 * @return TRUE se usuario clicou OK, FALSE se cancelou
 */
BOOL MostrarDialogoTexto(HWND hwndParent, const char* titulo,
                         const char* mensagem, char* buffer, int tamanho);

/**
 * MostrarDialogoNumero - Mostra um dialogo modal para entrada de numero
 *
 * Internamente usa MostrarDialogoTexto e converte o texto para double
 *
 * @param hwndParent: Janela pai
 * @param titulo: Titulo da janela
 * @param mensagem: Mensagem/pergunta
 * @param valor: Ponteiro onde o valor digitado sera armazenado
 *
 * @return TRUE se usuario digitou e clicou OK, FALSE se cancelou
 */
BOOL MostrarDialogoNumero(HWND hwndParent, const char* titulo,
                          const char* mensagem, double* valor);

/**
 * MostrarDialogoCategoria - Mostra um dialogo com listbox para selecao de categoria
 *
 * @param hwndParent: Janela pai
 * @param lista_categorias: Ponteiro para a lista encadeada de categorias
 * @param categoria_selecionada: Buffer onde o nome da categoria sera armazenado
 *
 * @return TRUE se usuario selecionou e clicou OK, FALSE se cancelou
 */
BOOL MostrarDialogoCategoria(HWND hwndParent, void* lista_categorias,
                             char* categoria_selecionada);

#endif /* DIALOGO_H */
