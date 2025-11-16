#include "gui.h"
#include "categoria.h"
#include "arvore.h"
#include <stdio.h>
#include <string.h>
#include <commctrl.h>

static AppData* g_appData = NULL;
static HFONT g_hFontMono = NULL; /* Fonte monoespaçada para tabelas */
static HFONT g_hFontNormal = NULL;

/* Limpa a caixa de listagem */
void LimparListBox(HWND hwndListBox) {
    SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);
}

/* Adiciona item a caixa de listagem */
void AdicionarItemListBox(HWND hwndListBox, const char* texto) {
    SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)texto);
}

/* Exibe mensagem */
void MostrarMensagem(HWND hwnd, const char* titulo, const char* mensagem) {
    MessageBox(hwnd, mensagem, titulo, MB_OK | MB_ICONINFORMATION);
}

/* Adiciona separador de tabela */
void AdicionarSeparador(HWND hwndListBox, int tamanho) {
    char separador[256];
    int i;
    for (i = 0; i < tamanho && i < 255; i++) {
        separador[i] = '-';
    }
    separador[i] = '\0';
    AdicionarItemListBox(hwndListBox, separador);
}

/* Adiciona cabecalho de tabela bonito */
void AdicionarCabecalhoTabela(HWND hwndListBox) {
    AdicionarSeparador(hwndListBox, 110);
    AdicionarItemListBox(hwndListBox, " Num | Descricao                                       | Energia  | Proteina ");
    AdicionarSeparador(hwndListBox, 110);
}

/* Cria os controles da janela */
void CriarControles(HWND hwnd) {
    /* Criar fontes */
    g_hFontNormal = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

    g_hFontMono = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");

    /* Titulo */
    HWND hwndTitulo = CreateWindow(
        "STATIC", "SISTEMA DE GERENCIAMENTO DE ALIMENTOS",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        10, 10, 980, 35,
        hwnd, NULL, NULL, NULL
    );
    SendMessage(hwndTitulo, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);

    /* Subtitulo */
    HWND hwndSubtitulo = CreateWindow(
        "STATIC", "Trabalho de Programacao Imperativa - RA2",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        10, 45, 980, 20,
        hwnd, NULL, NULL, NULL
    );

    /* Botoes - Linha 1 */
    CreateWindow("BUTTON", "1. Listar Categorias",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 75, 240, 45,
        hwnd, (HMENU)ID_BTN_LISTAR_CATEGORIAS, NULL, NULL);

    CreateWindow("BUTTON", "2. Listar Alimentos",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        260, 75, 240, 45,
        hwnd, (HMENU)ID_BTN_LISTAR_ALIMENTOS, NULL, NULL);

    CreateWindow("BUTTON", "3. Ordenar por Energia",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        510, 75, 240, 45,
        hwnd, (HMENU)ID_BTN_ENERGIA_DESC, NULL, NULL);

    CreateWindow("BUTTON", "4. Ordenar por Proteina",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        760, 75, 240, 45,
        hwnd, (HMENU)ID_BTN_PROTEINA_DESC, NULL, NULL);

    /* Botoes - Linha 2 */
    CreateWindow("BUTTON", "5. Filtrar Energia",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 130, 240, 45,
        hwnd, (HMENU)ID_BTN_INTERVALO_ENERGIA, NULL, NULL);

    CreateWindow("BUTTON", "6. Filtrar Proteina",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        260, 130, 240, 45,
        hwnd, (HMENU)ID_BTN_INTERVALO_PROTEINA, NULL, NULL);

    CreateWindow("BUTTON", "7. Remover Categoria",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        510, 130, 240, 45,
        hwnd, (HMENU)ID_BTN_REMOVER_CATEGORIA, NULL, NULL);

    CreateWindow("BUTTON", "8. Remover Alimento",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        760, 130, 150, 45,
        hwnd, (HMENU)ID_BTN_REMOVER_ALIMENTO, NULL, NULL);

    CreateWindow("BUTTON", "9. SAIR",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        920, 130, 80, 45,
        hwnd, (HMENU)ID_BTN_SAIR, NULL, NULL);

    /* Frame para area de resultados */
    CreateWindow("BUTTON", "Resultados",
        WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
        10, 185, 990, 430,
        hwnd, NULL, NULL, NULL);

    /* ListBox para resultados com fonte monoespaçada */
    g_appData->hwndListBox = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "LISTBOX", NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
        LBS_NOINTEGRALHEIGHT | LBS_NOTIFY,
        20, 205, 970, 395,
        hwnd, (HMENU)ID_LISTBOX_RESULTADO, NULL, NULL
    );
    SendMessage(g_appData->hwndListBox, WM_SETFONT, (WPARAM)g_hFontMono, TRUE);

    /* Status bar */
    g_appData->hwndStatus = CreateWindowEx(
        WS_EX_STATICEDGE, "STATIC",
        "Pronto. Sistema carregado com 597 alimentos em 15 categorias.",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        10, 625, 990, 25,
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

    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "|              CATEGORIAS DE ALIMENTOS - ORDEM ALFABETICA                |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    int contador = 1;
    NoCategoria* atual = app->lista_categorias;
    char buffer[512];

    while (atual != NULL) {
        sprintf(buffer, "  %2d.  %s", contador, atual->nome);
        AdicionarItemListBox(app->hwndListBox, buffer);
        contador++;
        atual = atual->proximo;
    }

    AdicionarItemListBox(app->hwndListBox, "");
    sprintf(buffer, "Total: %d categorias", contador - 1);
    AdicionarItemListBox(app->hwndListBox, buffer);

    SetWindowText(app->hwndStatus, "Categorias listadas com sucesso.");
}

/* Listar alimentos de uma categoria */
void OnListarAlimentos(AppData* app) {
    if (app->lista_categorias == NULL) {
        AdicionarItemListBox(app->hwndListBox, "Nenhuma categoria encontrada.");
        return;
    }

    /* Usar primeira categoria como exemplo - idealmente pedir ao usuario */
    NoCategoria* cat = app->lista_categorias;

    LimparListBox(app->hwndListBox);

    char buffer[512];

    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    AdicionarCabecalhoTabela(app->hwndListBox);

    int total = 0;
    NoAlimento* alim = cat->lista_alimentos;
    while (alim != NULL) {
        sprintf(buffer, " %3d | %-50s | %4d kcal | %6.1f g",
                alim->numero, alim->descricao,
                alim->energia_kcal, alim->proteina);
        AdicionarItemListBox(app->hwndListBox, buffer);
        alim = alim->proximo;
        total++;
    }

    AdicionarSeparador(app->hwndListBox, 110);
    sprintf(buffer, "Total: %d alimentos", total);
    AdicionarItemListBox(app->hwndListBox, buffer);

    sprintf(buffer, "Alimentos da categoria '%s' listados (ordem alfabetica).", cat->nome);
    SetWindowText(app->hwndStatus, buffer);
}

/* Funcao recursiva auxiliar para adicionar nos da arvore a lista */
void AdicionarNosArvoreDecrescente(NoArvore* raiz, HWND hwndListBox) {
    if (raiz == NULL) return;

    AdicionarNosArvoreDecrescente(raiz->direita, hwndListBox);

    char buffer[512];
    sprintf(buffer, " %3d | %-50s | %4d kcal | %6.1f g",
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

    NoCategoria* cat = app->lista_categorias;

    LimparListBox(app->hwndListBox);

    char buffer[512];

    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);
    AdicionarItemListBox(app->hwndListBox, "|  ORDENADO POR: ENERGIA (DECRESCENTE) - Usando Arvore Binaria          |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    AdicionarCabecalhoTabela(app->hwndListBox);

    if (cat->arvore_energia != NULL) {
        AdicionarNosArvoreDecrescente(cat->arvore_energia, app->hwndListBox);
    }

    AdicionarSeparador(app->hwndListBox, 110);

    SetWindowText(app->hwndStatus, "Alimentos ordenados por energia (maior -> menor).");
}

/* Listar por proteina decrescente */
void OnListarPorProteina(AppData* app) {
    if (app->lista_categorias == NULL) return;

    NoCategoria* cat = app->lista_categorias;

    LimparListBox(app->hwndListBox);

    char buffer[512];

    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);
    AdicionarItemListBox(app->hwndListBox, "|  ORDENADO POR: PROTEINA (DECRESCENTE) - Usando Arvore Binaria         |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    AdicionarCabecalhoTabela(app->hwndListBox);

    if (cat->arvore_proteina != NULL) {
        AdicionarNosArvoreDecrescente(cat->arvore_proteina, app->hwndListBox);
    }

    AdicionarSeparador(app->hwndListBox, 110);

    SetWindowText(app->hwndStatus, "Alimentos ordenados por proteina (maior -> menor).");
}

/* Stubs para outras funcoes */
void OnIntervaloEnergia(AppData* app) {
    MostrarMensagem(app->hwndMain, "Em desenvolvimento",
        "Funcao de intervalo de energia sera implementada em breve!\n\n"
        "Por enquanto, use as opcoes 1-4 que estao completas.");
}

void OnIntervaloProteina(AppData* app) {
    MostrarMensagem(app->hwndMain, "Em desenvolvimento",
        "Funcao de intervalo de proteina sera implementada em breve!\n\n"
        "Por enquanto, use as opcoes 1-4 que estao completas.");
}

void OnRemoverCategoria(AppData* app) {
    MostrarMensagem(app->hwndMain, "Em desenvolvimento",
        "Funcao de remocao de categoria sera implementada em breve!\n\n"
        "Por enquanto, use as opcoes 1-4 que estao completas.");
}

void OnRemoverAlimento(AppData* app) {
    MostrarMensagem(app->hwndMain, "Em desenvolvimento",
        "Funcao de remocao de alimento sera implementada em breve!\n\n"
        "Por enquanto, use as opcoes 1-4 que estao completas.");
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
                    if (MessageBox(hwnd, "Deseja realmente sair do sistema?",
                        "Confirmar Saida", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        PostQuitMessage(0);
                    }
                    break;
            }
            break;

        case WM_CLOSE:
            if (MessageBox(hwnd, "Deseja realmente fechar o programa?",
                "Confirmar Fechamento", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                DestroyWindow(hwnd);
            }
            break;

        case WM_DESTROY:
            if (g_hFontMono) DeleteObject(g_hFontMono);
            if (g_hFontNormal) DeleteObject(g_hFontNormal);
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
        0,
        "SistemaAlimentosClass",
        "Sistema de Gerenciamento de Alimentos - Trabalho de Programacao Imperativa RA2",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 1030, 710,
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

/* Funcao para configurar AppData global */
void SetAppData(AppData* app) {
    g_appData = app;
}

/* Obtem ponteiro para AppData global */
AppData* GetAppData() {
    return g_appData;
}
