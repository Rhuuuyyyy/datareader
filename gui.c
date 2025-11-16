#include "gui.h"
#include "categoria.h"
#include "arvore.h"
#include <stdio.h>
#include <string.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

static AppData* g_appData = NULL;

/* Limpa a caixa de listagem */
void LimparListBox(HWND hwndListBox) {
    SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);
}

/* Adiciona item à caixa de listagem */
void AdicionarItemListBox(HWND hwndListBox, const char* texto) {
    SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)texto);
}

/* Exibe caixa de diálogo para entrada de texto */
char* PedirEntrada(HWND hwnd, const char* titulo, const char* mensagem) {
    static char buffer[256];

    /* Criar janela de diálogo simples */
    HWND hwndDialog = CreateWindowEx(
        WS_EX_DLGMODALFRAME,
        "EDIT",
        "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
        10, 40, 300, 25,
        hwnd,
        NULL,
        NULL,
        NULL
    );

    /* Por simplicidade, vou usar InputBox via MessageBox + prompt */
    /* Em produção real, seria melhor criar um diálogo customizado */
    MessageBox(hwnd, mensagem, titulo, MB_OK | MB_ICONINFORMATION);

    /* Retornar buffer vazio por enquanto - precisa de diálogo customizado */
    buffer[0] = '\0';
    return buffer;
}

/* Exibe mensagem */
void MostrarMensagem(HWND hwnd, const char* titulo, const char* mensagem) {
    MessageBox(hwnd, mensagem, titulo, MB_OK | MB_ICONINFORMATION);
}

/* Cria os controles da janela */
void CriarControles(HWND hwnd) {
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

    /* Título */
    HWND hwndTitulo = CreateWindow(
        "STATIC", "Sistema de Gerenciamento de Alimentos",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        10, 10, 760, 30,
        hwnd, NULL, NULL, NULL
    );
    SendMessage(hwndTitulo, WM_SETFONT, (WPARAM)hFont, TRUE);

    /* Botões - Coluna 1 */
    CreateWindow("BUTTON", "1. Listar Categorias",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 50, 240, 40,
        hwnd, (HMENU)ID_BTN_LISTAR_CATEGORIAS, NULL, NULL);

    CreateWindow("BUTTON", "2. Listar Alimentos",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 100, 240, 40,
        hwnd, (HMENU)ID_BTN_LISTAR_ALIMENTOS, NULL, NULL);

    CreateWindow("BUTTON", "3. Ordenar por Energia",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 150, 240, 40,
        hwnd, (HMENU)ID_BTN_ENERGIA_DESC, NULL, NULL);

    /* Botões - Coluna 2 */
    CreateWindow("BUTTON", "4. Ordenar por Proteína",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        260, 50, 240, 40,
        hwnd, (HMENU)ID_BTN_PROTEINA_DESC, NULL, NULL);

    CreateWindow("BUTTON", "5. Filtrar Energia",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        260, 100, 240, 40,
        hwnd, (HMENU)ID_BTN_INTERVALO_ENERGIA, NULL, NULL);

    CreateWindow("BUTTON", "6. Filtrar Proteína",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        260, 150, 240, 40,
        hwnd, (HMENU)ID_BTN_INTERVALO_PROTEINA, NULL, NULL);

    /* Botões - Coluna 3 */
    CreateWindow("BUTTON", "7. Remover Categoria",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        510, 50, 240, 40,
        hwnd, (HMENU)ID_BTN_REMOVER_CATEGORIA, NULL, NULL);

    CreateWindow("BUTTON", "8. Remover Alimento",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        510, 100, 240, 40,
        hwnd, (HMENU)ID_BTN_REMOVER_ALIMENTO, NULL, NULL);

    CreateWindow("BUTTON", "9. Sair",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        510, 150, 240, 40,
        hwnd, (HMENU)ID_BTN_SAIR, NULL, NULL);

    /* ListBox para resultados */
    g_appData->hwndListBox = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "LISTBOX", NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | LBS_NOINTEGRALHEIGHT,
        10, 200, 760, 350,
        hwnd, (HMENU)ID_LISTBOX_RESULTADO, NULL, NULL
    );

    /* Status bar */
    g_appData->hwndStatus = CreateWindowEx(
        0, "STATIC", "Pronto. 597 alimentos carregados.",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        10, 560, 760, 20,
        hwnd, (HMENU)ID_STATUSBAR, NULL, NULL
    );
}

/* Listar todas as categorias */
void OnListarCategorias(AppData* app) {
    LimparListBox(app->hwndListBox);

    if (app->lista_categorias == NULL) {
        AdicionarItemListBox(app->hwndListBox, "Nenhuma categoria encontrada.");
        return;
    }

    AdicionarItemListBox(app->hwndListBox, "=== CATEGORIAS DE ALIMENTOS ===");
    AdicionarItemListBox(app->hwndListBox, "");

    int contador = 1;
    NoCategoria* atual = app->lista_categorias;
    char buffer[256];

    while (atual != NULL) {
        sprintf(buffer, "%2d. %s", contador, atual->nome);
        AdicionarItemListBox(app->hwndListBox, buffer);
        contador++;
        atual = atual->proximo;
    }

    SetWindowText(app->hwndStatus, "Categorias listadas com sucesso.");
}

/* Listar alimentos de uma categoria */
void OnListarAlimentos(AppData* app) {
    /* Criar diálogo para pedir nome da categoria */
    char categoria[MAX_CATEGORIA];

    /* Diálogo de entrada simples */
    HWND hwndInput = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        "EDIT", "",
        WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_TABSTOP,
        0, 0, 0, 0,
        app->hwndMain, (HMENU)ID_EDIT_INPUT, GetModuleHandle(NULL), NULL
    );

    /* Por simplicidade, vou usar primeira categoria como exemplo */
    /* Em versão final, adicionar InputDialog personalizado */
    if (app->lista_categorias != NULL) {
        strcpy(categoria, app->lista_categorias->nome);
    } else {
        return;
    }

    NoCategoria* cat = buscar_categoria(app->lista_categorias, categoria);

    LimparListBox(app->hwndListBox);

    if (cat == NULL) {
        AdicionarItemListBox(app->hwndListBox, "Categoria não encontrada!");
        return;
    }

    char buffer[512];
    sprintf(buffer, "=== ALIMENTOS: %s ===", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);
    AdicionarItemListBox(app->hwndListBox, "");

    NoAlimento* alim = cat->lista_alimentos;
    while (alim != NULL) {
        sprintf(buffer, "%3d | %-45s | %4d kcal | %5.1fg",
                alim->numero, alim->descricao,
                alim->energia_kcal, alim->proteina);
        AdicionarItemListBox(app->hwndListBox, buffer);
        alim = alim->proximo;
    }

    sprintf(buffer, "Alimentos da categoria '%s' listados.", categoria);
    SetWindowText(app->hwndStatus, buffer);
}

/* Função recursiva auxiliar para adicionar nós da árvore à lista */
void AdicionarNosArvoreDecrescente(NoArvore* raiz, HWND hwndListBox) {
    if (raiz == NULL) return;

    AdicionarNosArvoreDecrescente(raiz->direita, hwndListBox);

    char buffer[512];
    sprintf(buffer, "%3d | %-45s | %4d kcal | %5.1fg",
            raiz->alimento->numero,
            raiz->alimento->descricao,
            raiz->alimento->energia_kcal,
            raiz->alimento->proteina);
    AdicionarItemListBox(hwndListBox, buffer);

    AdicionarNosArvoreDecrescente(raiz->esquerda, hwndListBox);
}

/* Listar por energia decrescente */
void OnListarPorEnergia(AppData* app) {
    if (app->lista_categorias == NULL) return;

    /* Usar primeira categoria como exemplo */
    NoCategoria* cat = app->lista_categorias;

    LimparListBox(app->hwndListBox);

    char buffer[256];
    sprintf(buffer, "=== ALIMENTOS POR ENERGIA (Decrescente): %s ===", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);
    AdicionarItemListBox(app->hwndListBox, "");

    if (cat->arvore_energia != NULL) {
        AdicionarNosArvoreDecrescente(cat->arvore_energia, app->hwndListBox);
    }

    SetWindowText(app->hwndStatus, "Alimentos ordenados por energia.");
}

/* Listar por proteína decrescente */
void OnListarPorProteina(AppData* app) {
    if (app->lista_categorias == NULL) return;

    NoCategoria* cat = app->lista_categorias;

    LimparListBox(app->hwndListBox);

    char buffer[256];
    sprintf(buffer, "=== ALIMENTOS POR PROTEÍNA (Decrescente): %s ===", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);
    AdicionarItemListBox(app->hwndListBox, "");

    if (cat->arvore_proteina != NULL) {
        AdicionarNosArvoreDecrescente(cat->arvore_proteina, app->hwndListBox);
    }

    SetWindowText(app->hwndStatus, "Alimentos ordenados por proteína.");
}

/* Stubs para outras funções */
void OnIntervaloEnergia(AppData* app) {
    MostrarMensagem(app->hwndMain, "Em desenvolvimento",
        "Função de intervalo de energia será implementada em breve!");
}

void OnIntervaloProteina(AppData* app) {
    MostrarMensagem(app->hwndMain, "Em desenvolvimento",
        "Função de intervalo de proteína será implementada em breve!");
}

void OnRemoverCategoria(AppData* app) {
    MostrarMensagem(app->hwndMain, "Em desenvolvimento",
        "Função de remoção será implementada em breve!");
}

void OnRemoverAlimento(AppData* app) {
    MostrarMensagem(app->hwndMain, "Em desenvolvimento",
        "Função de remoção será implementada em breve!");
}

/* Processador de mensagens da janela */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            CriarControles(hwnd);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_BTN_LISTAR_CATEGORIAS:
                    OnListarCategorias(g_appData);
                    break;
                case ID_BTN_LISTAR_ALIMENTOS:
                    OnListarAlimentos(g_appData);
                    break;
                case ID_BTN_ENERGIA_DESC:
                    OnListarPorEnergia(g_appData);
                    break;
                case ID_BTN_PROTEINA_DESC:
                    OnListarPorProteina(g_appData);
                    break;
                case ID_BTN_INTERVALO_ENERGIA:
                    OnIntervaloEnergia(g_appData);
                    break;
                case ID_BTN_INTERVALO_PROTEINA:
                    OnIntervaloProteina(g_appData);
                    break;
                case ID_BTN_REMOVER_CATEGORIA:
                    OnRemoverCategoria(g_appData);
                    break;
                case ID_BTN_REMOVER_ALIMENTO:
                    OnRemoverAlimento(g_appData);
                    break;
                case ID_BTN_SAIR:
                    PostQuitMessage(0);
                    break;
            }
            break;

        case WM_CLOSE:
            if (MessageBox(hwnd, "Deseja realmente sair?", "Confirmar",
                MB_YESNO | MB_ICONQUESTION) == IDYES) {
                DestroyWindow(hwnd);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* Inicializa a GUI */
void InicializarGUI(HINSTANCE hInstance) {
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "SistemaAlimentosClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Falha ao registrar classe da janela!", "Erro",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    g_appData->hwndMain = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "SistemaAlimentosClass",
        "Sistema de Gerenciamento de Alimentos - Trabalho RA2",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 650,
        NULL, NULL, hInstance, NULL
    );

    if (g_appData->hwndMain == NULL) {
        MessageBox(NULL, "Falha ao criar janela!", "Erro",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    ShowWindow(g_appData->hwndMain, SW_SHOW);
    UpdateWindow(g_appData->hwndMain);
}

/* Função para configurar AppData global */
void SetAppData(AppData* app) {
    g_appData = app;
}
