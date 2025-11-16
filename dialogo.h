#ifndef DIALOGO_H
#define DIALOGO_H

#include <windows.h>

/* Estrutura para passar dados para o dialogo */
typedef struct {
    char texto[256];
    const char* titulo;
    const char* mensagem;
} DadosDialogo;

/* Mostra dialogo para entrada de texto */
BOOL MostrarDialogoTexto(HWND hwndParent, const char* titulo, const char* mensagem, char* buffer, int tamanho);

/* Mostra dialogo para entrada de numero */
BOOL MostrarDialogoNumero(HWND hwndParent, const char* titulo, const char* mensagem, double* valor);

/* Mostra dialogo de selecao de categoria */
BOOL MostrarDialogoCategoria(HWND hwndParent, void* lista_categorias, char* categoria_selecionada);

#endif
