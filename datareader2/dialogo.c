/******************************************************************************
 * DIALOGO.C - Sistema de Dialogos Modais Robusto
 *
 * Usa PeekMessage para evitar bloquear o loop principal
 *****************************************************************************/

#include "dialogo.h"
#include "tipos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ID_EDIT_DIALOGO       301
#define ID_BTN_OK             302
#define ID_BTN_CANCELAR       303
#define ID_LISTBOX_CATEGORIAS 304

/* Variaveis globais para comunicacao */
static char g_texto_resultado[256];
static char g_categoria_selecionada[256];
static BOOL g_dialogo_confirmado = FALSE;

/******************************************************************************
 * PROCEDIMENTO DO DIALOGO DE TEXTO
 *****************************************************************************/
LRESULT CALLBACK DialogoTextoProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_BTN_OK:
                    GetDlgItemText(hwnd, ID_EDIT_DIALOGO, g_texto_resultado, 256);
                    g_dialogo_confirmado = TRUE;
                    EnableWindow(GetParent(hwnd), TRUE);
                    SetForegroundWindow(GetParent(hwnd));
                    DestroyWindow(hwnd);
                    return 0;

                case ID_BTN_CANCELAR:
                    g_dialogo_confirmado = FALSE;
                    EnableWindow(GetParent(hwnd), TRUE);
                    SetForegroundWindow(GetParent(hwnd));
                    DestroyWindow(hwnd);
                    return 0;
            }
            break;

        case WM_CLOSE:
            g_dialogo_confirmado = FALSE;
            EnableWindow(GetParent(hwnd), TRUE);
            SetForegroundWindow(GetParent(hwnd));
            DestroyWindow(hwnd);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/******************************************************************************
 * PROCEDIMENTO DO DIALOGO DE CATEGORIA
 *****************************************************************************/
LRESULT CALLBACK DialogoCategoriaProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_BTN_OK: {
                    HWND hwndList = GetDlgItem(hwnd, ID_LISTBOX_CATEGORIAS);
                    int sel = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

                    if (sel == LB_ERR) {
                        MessageBox(hwnd, "Selecione uma categoria!", "Aviso", MB_OK | MB_ICONWARNING);
                        return 0;
                    }

                    SendMessage(hwndList, LB_GETTEXT, sel, (LPARAM)g_categoria_selecionada);
                    g_dialogo_confirmado = TRUE;
                    EnableWindow(GetParent(hwnd), TRUE);
                    SetForegroundWindow(GetParent(hwnd));
                    DestroyWindow(hwnd);
                    return 0;
                }

                case ID_BTN_CANCELAR:
                    g_dialogo_confirmado = FALSE;
                    EnableWindow(GetParent(hwnd), TRUE);
                    SetForegroundWindow(GetParent(hwnd));
                    DestroyWindow(hwnd);
                    return 0;

                case ID_LISTBOX_CATEGORIAS:
                    if (HIWORD(wParam) == LBN_DBLCLK) {
                        HWND hwndList = GetDlgItem(hwnd, ID_LISTBOX_CATEGORIAS);
                        int sel = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

                        if (sel != LB_ERR) {
                            SendMessage(hwndList, LB_GETTEXT, sel, (LPARAM)g_categoria_selecionada);
                            g_dialogo_confirmado = TRUE;
                            EnableWindow(GetParent(hwnd), TRUE);
                            SetForegroundWindow(GetParent(hwnd));
                            DestroyWindow(hwnd);
                        }
                        return 0;
                    }
                    break;
            }
            break;

        case WM_CLOSE:
            g_dialogo_confirmado = FALSE;
            EnableWindow(GetParent(hwnd), TRUE);
            SetForegroundWindow(GetParent(hwnd));
            DestroyWindow(hwnd);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/******************************************************************************
 * MostrarDialogoTexto - Usa PeekMessage para nao bloquear
 *****************************************************************************/
BOOL MostrarDialogoTexto(HWND hwndParent, const char* titulo,
                         const char* mensagem, char* buffer, int tamanho) {
    g_texto_resultado[0] = '\0';
    g_dialogo_confirmado = FALSE;

    /* Registrar classe */
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = DialogoTextoProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszClassName = "DialogoTextoClassV3";
    RegisterClassEx(&wc);

    /* Criar janela */
    HWND hwndDialogo = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        "DialogoTextoClassV3",
        titulo,
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        0, 0, 420, 160,
        hwndParent, NULL, GetModuleHandle(NULL), NULL
    );

    if (!hwndDialogo) {
        UnregisterClass("DialogoTextoClassV3", GetModuleHandle(NULL));
        return FALSE;
    }

    /* Centralizar */
    RECT rc;
    GetWindowRect(hwndDialogo, &rc);
    int x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;
    SetWindowPos(hwndDialogo, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE);

    /* Criar controles */
    CreateWindow("STATIC", mensagem, WS_VISIBLE | WS_CHILD,
        10, 10, 390, 35, hwndDialogo, NULL, NULL, NULL);

    CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        10, 50, 390, 25, hwndDialogo, (HMENU)ID_EDIT_DIALOGO, NULL, NULL);

    CreateWindow("BUTTON", "OK", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        220, 85, 80, 30, hwndDialogo, (HMENU)ID_BTN_OK, NULL, NULL);

    CreateWindow("BUTTON", "Cancelar", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        310, 85, 90, 30, hwndDialogo, (HMENU)ID_BTN_CANCELAR, NULL, NULL);

    SetFocus(GetDlgItem(hwndDialogo, ID_EDIT_DIALOGO));
    EnableWindow(hwndParent, FALSE);

    /* Loop de mensagens usando PeekMessage */
    MSG msg;
    while (IsWindow(hwndDialogo)) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                UnregisterClass("DialogoTextoClassV3", GetModuleHandle(NULL));
                return g_dialogo_confirmado;
            }

            if (!IsDialogMessage(hwndDialogo, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        /* Pequeno sleep para nao consumir 100% da CPU */
        Sleep(10);
    }

    UnregisterClass("DialogoTextoClassV3", GetModuleHandle(NULL));

    if (g_dialogo_confirmado && buffer != NULL) {
        strncpy(buffer, g_texto_resultado, tamanho - 1);
        buffer[tamanho - 1] = '\0';
    }

    return g_dialogo_confirmado;
}

/******************************************************************************
 * MostrarDialogoNumero
 *****************************************************************************/
BOOL MostrarDialogoNumero(HWND hwndParent, const char* titulo,
                          const char* mensagem, double* valor) {
    char buffer[64];

    if (MostrarDialogoTexto(hwndParent, titulo, mensagem, buffer, sizeof(buffer))) {
        *valor = atof(buffer);
        return TRUE;
    }

    return FALSE;
}

/******************************************************************************
 * MostrarDialogoCategoria - Usa PeekMessage para nao bloquear
 *****************************************************************************/
BOOL MostrarDialogoCategoria(HWND hwndParent, void* lista_categorias,
                             char* categoria_selecionada) {
    g_categoria_selecionada[0] = '\0';
    g_dialogo_confirmado = FALSE;

    /* Registrar classe */
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = DialogoCategoriaProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszClassName = "DialogoCategoriaClassV3";
    RegisterClassEx(&wc);

    /* Criar janela */
    HWND hwndDialogo = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        "DialogoCategoriaClassV3",
        "Selecionar Categoria",
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        0, 0, 420, 380,
        hwndParent, NULL, GetModuleHandle(NULL), NULL
    );

    if (!hwndDialogo) {
        UnregisterClass("DialogoCategoriaClassV3", GetModuleHandle(NULL));
        return FALSE;
    }

    /* Centralizar */
    RECT rc;
    GetWindowRect(hwndDialogo, &rc);
    int x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;
    SetWindowPos(hwndDialogo, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE);

    /* Criar controles */
    CreateWindow("STATIC", "Selecione uma categoria da lista:",
        WS_VISIBLE | WS_CHILD, 10, 10, 390, 20,
        hwndDialogo, NULL, NULL, NULL);

    HWND hwndList = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
        10, 35, 390, 260, hwndDialogo, (HMENU)ID_LISTBOX_CATEGORIAS, NULL, NULL);

    /* Preencher listbox */
    NoCategoria* cat = (NoCategoria*)lista_categorias;
    while (cat != NULL) {
        SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)cat->nome);
        cat = cat->proximo;
    }

    SendMessage(hwndList, LB_SETCURSEL, 0, 0);

    CreateWindow("BUTTON", "OK", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        220, 305, 80, 30, hwndDialogo, (HMENU)ID_BTN_OK, NULL, NULL);

    CreateWindow("BUTTON", "Cancelar", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        310, 305, 90, 30, hwndDialogo, (HMENU)ID_BTN_CANCELAR, NULL, NULL);

    EnableWindow(hwndParent, FALSE);
    SetFocus(hwndList);

    /* Loop de mensagens usando PeekMessage */
    MSG msg;
    while (IsWindow(hwndDialogo)) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                UnregisterClass("DialogoCategoriaClassV3", GetModuleHandle(NULL));
                return g_dialogo_confirmado;
            }

            if (!IsDialogMessage(hwndDialogo, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        /* Pequeno sleep para nao consumir 100% da CPU */
        Sleep(10);
    }

    UnregisterClass("DialogoCategoriaClassV3", GetModuleHandle(NULL));

    if (g_dialogo_confirmado && categoria_selecionada != NULL) {
        strcpy(categoria_selecionada, g_categoria_selecionada);
    }

    return g_dialogo_confirmado;
}
