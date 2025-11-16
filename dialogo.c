#include "dialogo.h"
#include "tipos.h"
#include <stdio.h>
#include <string.h>

#define ID_EDIT_DIALOGO 301
#define ID_BTN_OK 302
#define ID_BTN_CANCELAR 303
#define ID_LISTBOX_CATEGORIAS 304

static DadosDialogo g_dadosDialogo = {0};
static HWND g_hwndEdit = NULL;
static BOOL g_dialogoOK = FALSE;

/* Procedimento do dialogo de texto */
LRESULT CALLBACK DialogoTextoProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_INITDIALOG:
            /* Centralizar janela */
            {
                RECT rc;
                GetWindowRect(hwnd, &rc);
                int x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
                int y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;
                SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }

            /* Criar controles */
            CreateWindow("STATIC", g_dadosDialogo.mensagem,
                WS_VISIBLE | WS_CHILD,
                10, 10, 380, 40,
                hwnd, NULL, NULL, NULL);

            g_hwndEdit = CreateWindow("EDIT", "",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                10, 55, 380, 25,
                hwnd, (HMENU)ID_EDIT_DIALOGO, NULL, NULL);

            CreateWindow("BUTTON", "OK",
                WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                220, 90, 80, 30,
                hwnd, (HMENU)ID_BTN_OK, NULL, NULL);

            CreateWindow("BUTTON", "Cancelar",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                310, 90, 80, 30,
                hwnd, (HMENU)ID_BTN_CANCELAR, NULL, NULL);

            SetFocus(g_hwndEdit);
            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BTN_OK) {
                GetWindowText(g_hwndEdit, g_dadosDialogo.texto, 256);
                g_dialogoOK = TRUE;
                EndDialog(hwnd, IDOK);
                return TRUE;
            } else if (LOWORD(wParam) == ID_BTN_CANCELAR) {
                g_dialogoOK = FALSE;
                EndDialog(hwnd, IDCANCEL);
                return TRUE;
            }
            break;

        case WM_CLOSE:
            g_dialogoOK = FALSE;
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
    }
    return FALSE;
}

/* Mostra dialogo para entrada de texto */
BOOL MostrarDialogoTexto(HWND hwndParent, const char* titulo, const char* mensagem, char* buffer, int tamanho) {
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = DefDlgProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "DialogoTextoClass";

    RegisterClassEx(&wc);

    g_dadosDialogo.titulo = titulo;
    g_dadosDialogo.mensagem = mensagem;
    g_dadosDialogo.texto[0] = '\0';
    g_dialogoOK = FALSE;

    HWND hwndDialogo = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        "DialogoTextoClass",
        titulo,
        WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
        0, 0, 420, 160,
        hwndParent,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    DialogoTextoProc(hwndDialogo, WM_INITDIALOG, 0, 0);

    /* Loop de mensagens modal */
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_QUIT || !IsWindow(hwndDialogo)) {
            break;
        }
        if (!IsDialogMessage(hwndDialogo, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    if (g_dialogoOK && buffer != NULL) {
        strncpy(buffer, g_dadosDialogo.texto, tamanho - 1);
        buffer[tamanho - 1] = '\0';
    }

    UnregisterClass("DialogoTextoClass", GetModuleHandle(NULL));
    return g_dialogoOK;
}

/* Mostra dialogo para entrada de numero */
BOOL MostrarDialogoNumero(HWND hwndParent, const char* titulo, const char* mensagem, double* valor) {
    char buffer[64];

    if (MostrarDialogoTexto(hwndParent, titulo, mensagem, buffer, sizeof(buffer))) {
        *valor = atof(buffer);
        return TRUE;
    }

    return FALSE;
}

/* Procedimento do dialogo de categoria */
static NoCategoria* g_listaCategorias = NULL;
static char g_categoriaSelecionada[MAX_CATEGORIA];

LRESULT CALLBACK DialogoCategoriaProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_INITDIALOG:
            {
                RECT rc;
                GetWindowRect(hwnd, &rc);
                int x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
                int y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;
                SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }

            CreateWindow("STATIC", "Selecione uma categoria:",
                WS_VISIBLE | WS_CHILD,
                10, 10, 380, 20,
                hwnd, NULL, NULL, NULL);

            HWND hwndList = CreateWindow("LISTBOX", NULL,
                WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
                10, 35, 380, 250,
                hwnd, (HMENU)ID_LISTBOX_CATEGORIAS, NULL, NULL);

            /* Preencher lista de categorias */
            NoCategoria* cat = g_listaCategorias;
            while (cat != NULL) {
                SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)cat->nome);
                cat = cat->proximo;
            }

            CreateWindow("BUTTON", "OK",
                WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                220, 295, 80, 30,
                hwnd, (HMENU)ID_BTN_OK, NULL, NULL);

            CreateWindow("BUTTON", "Cancelar",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                310, 295, 80, 30,
                hwnd, (HMENU)ID_BTN_CANCELAR, NULL, NULL);

            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BTN_OK) {
                HWND hwndList = GetDlgItem(hwnd, ID_LISTBOX_CATEGORIAS);
                int sel = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

                if (sel != LB_ERR) {
                    SendMessage(hwndList, LB_GETTEXT, sel, (LPARAM)g_categoriaSelecionada);
                    g_dialogoOK = TRUE;
                    EndDialog(hwnd, IDOK);
                } else {
                    MessageBox(hwnd, "Selecione uma categoria!", "Aviso", MB_OK | MB_ICONWARNING);
                }
                return TRUE;
            } else if (LOWORD(wParam) == ID_BTN_CANCELAR) {
                g_dialogoOK = FALSE;
                EndDialog(hwnd, IDCANCEL);
                return TRUE;
            } else if (LOWORD(wParam) == ID_LISTBOX_CATEGORIAS && HIWORD(wParam) == LBN_DBLCLK) {
                /* Duplo clique = OK */
                HWND hwndList = GetDlgItem(hwnd, ID_LISTBOX_CATEGORIAS);
                int sel = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

                if (sel != LB_ERR) {
                    SendMessage(hwndList, LB_GETTEXT, sel, (LPARAM)g_categoriaSelecionada);
                    g_dialogoOK = TRUE;
                    EndDialog(hwnd, IDOK);
                }
                return TRUE;
            }
            break;

        case WM_CLOSE:
            g_dialogoOK = FALSE;
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
    }
    return FALSE;
}

/* Mostra dialogo de selecao de categoria */
BOOL MostrarDialogoCategoria(HWND hwndParent, void* lista_categorias, char* categoria_selecionada) {
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = DefDlgProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "DialogoCategoriaClass";

    RegisterClassEx(&wc);

    g_listaCategorias = (NoCategoria*)lista_categorias;
    g_categoriaSelecionada[0] = '\0';
    g_dialogoOK = FALSE;

    HWND hwndDialogo = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        "DialogoCategoriaClass",
        "Selecionar Categoria",
        WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
        0, 0, 420, 370,
        hwndParent,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    DialogoCategoriaProc(hwndDialogo, WM_INITDIALOG, 0, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_QUIT || !IsWindow(hwndDialogo)) {
            break;
        }
        if (!IsDialogMessage(hwndDialogo, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    if (g_dialogoOK && categoria_selecionada != NULL) {
        strcpy(categoria_selecionada, g_categoriaSelecionada);
    }

    UnregisterClass("DialogoCategoriaClass", GetModuleHandle(NULL));
    return g_dialogoOK;
}
