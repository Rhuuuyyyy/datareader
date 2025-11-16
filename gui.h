#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include "tipos.h"

/* IDs dos controles da interface */
#define ID_BTN_LISTAR_CATEGORIAS 101
#define ID_BTN_LISTAR_ALIMENTOS 102
#define ID_BTN_ENERGIA_DESC 103
#define ID_BTN_PROTEINA_DESC 104
#define ID_BTN_INTERVALO_ENERGIA 105
#define ID_BTN_INTERVALO_PROTEINA 106
#define ID_BTN_REMOVER_CATEGORIA 107
#define ID_BTN_REMOVER_ALIMENTO 108
#define ID_BTN_SAIR 109
#define ID_LISTBOX_RESULTADO 201
#define ID_EDIT_INPUT 202
#define ID_STATUSBAR 203

/* Estrutura global para compartilhar dados entre funcoes */
typedef struct {
    NoCategoria* lista_categorias;
    HWND hwndMain;
    HWND hwndListBox;
    HWND hwndStatus;
    bool dados_modificados;
} AppData;

/* Funcoes da GUI */
void InicializarGUI(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CriarControles(HWND hwnd);
void LimparListBox(HWND hwndListBox);
void AdicionarItemListBox(HWND hwndListBox, const char* texto);
char* PedirEntrada(HWND hwnd, const char* titulo, const char* mensagem);
void MostrarMensagem(HWND hwnd, const char* titulo, const char* mensagem);

/* Funcoes de acao dos botoes */
void OnListarCategorias(AppData* app);
void OnListarAlimentos(AppData* app);
void OnListarPorEnergia(AppData* app);
void OnListarPorProteina(AppData* app);
void OnIntervaloEnergia(AppData* app);
void OnIntervaloProteina(AppData* app);
void OnRemoverCategoria(AppData* app);
void OnRemoverAlimento(AppData* app);

#endif
