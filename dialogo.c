/******************************************************************************
 * DIALOGO.C - Implementação do Sistema de Diálogos Personalizados
 *
 * Este arquivo implementa três tipos de diálogos modais usando Win32 API.
 * A estratégia usada é criar janelas popup com controles (edit, listbox, botões)
 * e processar mensagens de forma simplificada através de procedimentos próprios.
 *
 * ARQUITETURA:
 * - Cada diálogo tem seu próprio procedimento de janela (Window Procedure)
 * - Os diálogos são modais: desabilitam a janela pai durante exibição
 * - Não usam loop de mensagens separado para evitar conflitos
 * - Usam SetWindowLongPtr para armazenar dados do diálogo na janela
 *****************************************************************************/

#include "dialogo.h"
#include "tipos.h"          // Para MAX_CATEGORIA, MAX_DESCRICAO, NoCategoria
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************************************************************************
 * CONSTANTES - IDs dos Controles
 *****************************************************************************/
#define ID_EDIT_DIALOGO       301  // ID do campo de texto (EDIT)
#define ID_BTN_OK             302  // ID do botão OK
#define ID_BTN_CANCELAR       303  // ID do botão Cancelar
#define ID_LISTBOX_CATEGORIAS 304  // ID da lista de categorias (LISTBOX)

/******************************************************************************
 * VARIÁVEIS GLOBAIS PRIVADAS
 *
 * Estas variáveis são usadas para comunicação entre a função que cria o
 * diálogo e o procedimento que processa as mensagens.
 *****************************************************************************/
static BOOL g_dialogoResultadoOK = FALSE;  // TRUE se usuário clicou OK, FALSE se cancelou

/******************************************************************************
 * PROCEDIMENTO DO DIÁLOGO DE TEXTO
 *
 * Esta função processa todas as mensagens (eventos) da janela de diálogo de texto.
 * É chamada automaticamente pelo Windows sempre que um evento ocorre.
 *
 * PARÂMETROS:
 * - hwnd: Handle (identificador) da janela do diálogo
 * - msg: Tipo de mensagem (WM_INITDIALOG, WM_COMMAND, WM_CLOSE, etc.)
 * - wParam: Parâmetro 1 da mensagem (depende do tipo de mensagem)
 * - lParam: Parâmetro 2 da mensagem (depende do tipo de mensagem)
 *
 * RETORNO:
 * - TRUE se a mensagem foi processada
 * - FALSE se deve usar processamento padrão
 *****************************************************************************/
INT_PTR CALLBACK DialogoTextoProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    /* Recuperar ponteiro para dados do diálogo armazenado na janela */
    /* GWLP_USERDATA = posição de memória onde Windows guarda dados do usuário */
    DadosDialogoTexto* dados = (DadosDialogoTexto*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (msg) {
        /*** INICIALIZAÇÃO DO DIÁLOGO ***/
        case WM_INITDIALOG: {
            /* Esta mensagem é enviada quando o diálogo é criado */
            /* lParam contém o ponteiro que passamos em CreateDialogParam */
            dados = (DadosDialogoTexto*)lParam;

            /* Guardar ponteiro aos dados na janela para acesso posterior */
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)dados);

            /* Definir o título da janela */
            SetWindowText(hwnd, dados->titulo);

            /* Centralizar a janela na tela */
            /* 1. Obter retângulo (coordenadas) da janela */
            RECT rc;
            GetWindowRect(hwnd, &rc);

            /* 2. Calcular largura e altura da janela */
            int largura = rc.right - rc.left;
            int altura = rc.bottom - rc.top;

            /* 3. Obter tamanho da tela */
            int largura_tela = GetSystemMetrics(SM_CXSCREEN);
            int altura_tela = GetSystemMetrics(SM_CYSCREEN);

            /* 4. Calcular posição central */
            int x = (largura_tela - largura) / 2;
            int y = (altura_tela - altura) / 2;

            /* 5. Mover janela para o centro */
            SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

            /* Definir a mensagem no texto estático */
            SetDlgItemText(hwnd, 100, dados->mensagem);

            /* Dar foco ao campo de texto para usuário começar a digitar */
            SetFocus(GetDlgItem(hwnd, ID_EDIT_DIALOGO));

            /* Retornar FALSE porque definimos o foco manualmente */
            return FALSE;
        }

        /*** COMANDOS DE BOTÕES E CONTROLES ***/
        case WM_COMMAND: {
            /* Esta mensagem é enviada quando usuário clica em botões ou interage com controles */
            /* LOWORD(wParam) = ID do controle que gerou o evento */
            /* HIWORD(wParam) = tipo de notificação (BN_CLICKED para botões) */

            switch (LOWORD(wParam)) {
                case ID_BTN_OK: {
                    /* Usuário clicou no botão OK */

                    /* Recuperar o texto digitado no campo EDIT */
                    GetDlgItemText(hwnd, ID_EDIT_DIALOGO, dados->texto, 256);

                    /* Marcar que diálogo foi confirmado */
                    g_dialogoResultadoOK = TRUE;

                    /* Fechar o diálogo retornando IDOK */
                    EndDialog(hwnd, IDOK);
                    return TRUE;
                }

                case ID_BTN_CANCELAR: {
                    /* Usuário clicou no botão Cancelar */

                    /* Marcar que diálogo foi cancelado */
                    g_dialogoResultadoOK = FALSE;

                    /* Fechar o diálogo retornando IDCANCEL */
                    EndDialog(hwnd, IDCANCEL);
                    return TRUE;
                }
            }
            break;
        }

        /*** FECHAMENTO DA JANELA (X) ***/
        case WM_CLOSE: {
            /* Usuário clicou no X da janela */
            /* Tratar como cancelamento */
            g_dialogoResultadoOK = FALSE;
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
    }

    /* Retornar FALSE para mensagens não processadas */
    /* Windows irá usar o processamento padrão */
    return FALSE;
}

/******************************************************************************
 * PROCEDIMENTO DO DIÁLOGO DE CATEGORIA
 *
 * Similar ao DialogoTextoProc, mas para seleção de categoria em listbox.
 *****************************************************************************/
INT_PTR CALLBACK DialogoCategoriaProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    /* Recuperar dados do diálogo */
    DadosDialogoCategoria* dados = (DadosDialogoCategoria*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (msg) {
        /*** INICIALIZAÇÃO ***/
        case WM_INITDIALOG: {
            /* Guardar ponteiro aos dados */
            dados = (DadosDialogoCategoria*)lParam;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)dados);

            /* Centralizar janela */
            RECT rc;
            GetWindowRect(hwnd, &rc);
            int largura = rc.right - rc.left;
            int altura = rc.bottom - rc.top;
            int x = (GetSystemMetrics(SM_CXSCREEN) - largura) / 2;
            int y = (GetSystemMetrics(SM_CYSCREEN) - altura) / 2;
            SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

            /* Preencher listbox com categorias */
            /* Converter void* para NoCategoria* (lista encadeada) */
            NoCategoria* cat = (NoCategoria*)dados->lista_categorias;

            /* Obter handle do listbox */
            HWND hwndList = GetDlgItem(hwnd, ID_LISTBOX_CATEGORIAS);

            /* Percorrer lista encadeada e adicionar cada categoria */
            while (cat != NULL) {
                /* SendMessage com LB_ADDSTRING adiciona item ao listbox */
                SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)cat->nome);
                cat = cat->proximo;  // Avançar para próxima categoria
            }

            /* Selecionar primeiro item por padrão */
            SendMessage(hwndList, LB_SETCURSEL, 0, 0);

            return TRUE;
        }

        /*** COMANDOS ***/
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_BTN_OK: {
                    /* Obter handle do listbox */
                    HWND hwndList = GetDlgItem(hwnd, ID_LISTBOX_CATEGORIAS);

                    /* Obter índice do item selecionado */
                    /* LB_GETCURSEL retorna índice ou LB_ERR se nada selecionado */
                    int sel = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

                    if (sel != LB_ERR) {
                        /* Há item selecionado */

                        /* Obter o texto do item selecionado */
                        /* LB_GETTEXT copia o texto para o buffer */
                        SendMessage(hwndList, LB_GETTEXT, sel, (LPARAM)dados->categoria_selecionada);

                        /* Marcar sucesso e fechar */
                        g_dialogoResultadoOK = TRUE;
                        EndDialog(hwnd, IDOK);
                    } else {
                        /* Nada selecionado - avisar usuário */
                        MessageBox(hwnd,
                            "Por favor, selecione uma categoria da lista!",
                            "Selecao Obrigatoria",
                            MB_OK | MB_ICONWARNING);
                    }
                    return TRUE;
                }

                case ID_BTN_CANCELAR: {
                    /* Cancelar operação */
                    g_dialogoResultadoOK = FALSE;
                    EndDialog(hwnd, IDCANCEL);
                    return TRUE;
                }

                case ID_LISTBOX_CATEGORIAS: {
                    /* Verificar se foi duplo-clique no listbox */
                    /* HIWORD(wParam) contém o código de notificação */
                    if (HIWORD(wParam) == LBN_DBLCLK) {
                        /* Duplo-clique = confirmar seleção (atalho para OK) */
                        HWND hwndList = GetDlgItem(hwnd, ID_LISTBOX_CATEGORIAS);
                        int sel = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

                        if (sel != LB_ERR) {
                            SendMessage(hwndList, LB_GETTEXT, sel, (LPARAM)dados->categoria_selecionada);
                            g_dialogoResultadoOK = TRUE;
                            EndDialog(hwnd, IDOK);
                        }
                    }
                    return TRUE;
                }
            }
            break;
        }

        /*** FECHAMENTO ***/
        case WM_CLOSE: {
            g_dialogoResultadoOK = FALSE;
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************
 * FUNÇÃO AUXILIAR: Criar Template de Diálogo em Memória
 *
 * Win32 requer um template de diálogo para DialogBoxIndirectParam.
 * Esta função cria um template em memória dinamicamente.
 *
 * PARÂMETROS:
 * - largura, altura: tamanho da janela em unidades de diálogo
 * - titulo: título da janela
 *
 * RETORNO:
 * - Ponteiro para template alocado (deve ser liberado com free)
 *****************************************************************************/
static LPCDLGTEMPLATE CriarTemplateDialogo(int largura, int altura, const char* titulo) {
    /* Alocar memória para o template */
    /* Template consiste de uma estrutura DLGTEMPLATE seguida de dados variáveis */
    DWORD tamanho = sizeof(DLGTEMPLATE) + 512;  // 512 bytes extra para strings
    LPCDLGTEMPLATE pTemplate = (LPCDLGTEMPLATE)GlobalAlloc(GPTR, tamanho);

    if (pTemplate == NULL) {
        return NULL;  // Falha na alocação
    }

    /* Preencher estrutura DLGTEMPLATE */
    DLGTEMPLATE* p = (DLGTEMPLATE*)pTemplate;

    /* Estilo da janela: popup modal com caption e sysmenu */
    p->style = DS_MODALFRAME | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU;
    p->dwExtendedStyle = 0;
    p->cdit = 0;            // Número de controles (será definido ao criar controles)
    p->x = 0;               // Posição X (DS_CENTER centraliza)
    p->y = 0;               // Posição Y
    p->cx = largura;        // Largura
    p->cy = altura;         // Altura

    return pTemplate;
}

/******************************************************************************
 * FUNÇÃO PÚBLICA: MostrarDialogoTexto
 *
 * Cria e exibe um diálogo modal para entrada de texto.
 * Bloqueia a janela pai até o usuário fechar o diálogo.
 *****************************************************************************/
BOOL MostrarDialogoTexto(HWND hwndParent, const char* titulo,
                         const char* mensagem, char* buffer, int tamanho) {
    /* Inicializar estrutura com dados do diálogo */
    DadosDialogoTexto dados;
    dados.titulo = titulo;
    dados.mensagem = mensagem;
    dados.texto[0] = '\0';  // Inicializar buffer vazio

    /* Resetar flag de resultado */
    g_dialogoResultadoOK = FALSE;

    /* Registrar classe de janela para o diálogo */
    /* Uma "classe" define propriedades compartilhadas por janelas do mesmo tipo */
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = DefDlgProc;  // Procedimento padrão de diálogo
    wc.hInstance = GetModuleHandle(NULL);  // Handle do executável atual
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);  // Cor de fundo branca
    wc.lpszClassName = "DialogoTextoClass";  // Nome único da classe

    /* Registrar classe (ignora se já existir) */
    RegisterClassEx(&wc);

    /* Criar janela do diálogo */
    HWND hwndDialogo = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,  // Estilos estendidos: frame de diálogo, sempre no topo
        "DialogoTextoClass",                   // Nome da classe
        titulo,                                // Título da janela
        DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU,  // Estilos da janela
        0, 0,                                  // Posição (será centralizada)
        420, 160,                              // Tamanho (largura x altura)
        hwndParent,                            // Janela pai
        NULL,                                  // Menu (nenhum)
        GetModuleHandle(NULL),                 // Handle do módulo
        NULL                                   // Parâmetros adicionais
    );

    if (!hwndDialogo) {
        return FALSE;  // Falha ao criar janela
    }

    /* Armazenar dados na janela */
    SetWindowLongPtr(hwndDialogo, GWLP_USERDATA, (LONG_PTR)&dados);

    /* Criar controles filhos dentro do diálogo */

    /* 1. Label (texto estático) com a mensagem */
    CreateWindowEx(0,
        "STATIC",                              // Classe: controle de texto estático
        mensagem,                              // Texto a exibir
        WS_CHILD | WS_VISIBLE | SS_LEFT,      // Estilos: filho, visível, alinhado à esquerda
        10, 10, 390, 35,                      // Posição e tamanho
        hwndDialogo,                           // Pai
        (HMENU)100,                            // ID
        NULL, NULL);

    /* 2. Campo de texto (EDIT) para entrada do usuário */
    CreateWindowEx(WS_EX_CLIENTEDGE,           // Borda 3D
        "EDIT",                                // Classe: campo de texto
        "",                                    // Texto inicial (vazio)
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,  // Estilos: borda, scroll horizontal automático
        10, 50, 390, 25,                      // Posição e tamanho
        hwndDialogo,                           // Pai
        (HMENU)ID_EDIT_DIALOGO,                // ID
        NULL, NULL);

    /* 3. Botão OK */
    CreateWindowEx(0,
        "BUTTON",                              // Classe: botão
        "OK",                                  // Texto do botão
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,  // Estilos: botão padrão (ativado com Enter)
        220, 85, 80, 30,                      // Posição e tamanho
        hwndDialogo,                           // Pai
        (HMENU)ID_BTN_OK,                      // ID
        NULL, NULL);

    /* 4. Botão Cancelar */
    CreateWindowEx(0,
        "BUTTON",
        "Cancelar",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        310, 85, 90, 30,
        hwndDialogo,
        (HMENU)ID_BTN_CANCELAR,
        NULL, NULL);

    /* Centralizar janela */
    RECT rc;
    GetWindowRect(hwndDialogo, &rc);
    int x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;
    SetWindowPos(hwndDialogo, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    /* Desabilitar janela pai (torná-la modal) */
    EnableWindow(hwndParent, FALSE);

    /* Exibir janela */
    ShowWindow(hwndDialogo, SW_SHOW);
    UpdateWindow(hwndDialogo);

    /* Subclassear janela para usar nosso procedimento */
    SetWindowLongPtr(hwndDialogo, GWLP_WNDPROC, (LONG_PTR)DialogoTextoProc);

    /* Simular WM_INITDIALOG */
    DialogoTextoProc(hwndDialogo, WM_INITDIALOG, 0, (LPARAM)&dados);

    /* Processar mensagens até diálogo fechar */
    /* Este loop roda até EndDialog ser chamado no procedimento */
    MSG msg;
    BOOL executando = TRUE;

    while (executando && GetMessage(&msg, NULL, 0, 0)) {
        /* Verificar se diálogo ainda existe */
        if (!IsWindow(hwndDialogo)) {
            executando = FALSE;
            break;
        }

        /* Processar mensagens do diálogo */
        if (!IsDialogMessage(hwndDialogo, &msg)) {
            TranslateMessage(&msg);  // Traduzir teclas
            DispatchMessage(&msg);    // Enviar para procedimento
        }
    }

    /* Reabilitar janela pai */
    EnableWindow(hwndParent, TRUE);
    SetForegroundWindow(hwndParent);

    /* Destruir janela do diálogo */
    DestroyWindow(hwndDialogo);

    /* Remover classe registrada */
    UnregisterClass("DialogoTextoClass", GetModuleHandle(NULL));

    /* Copiar resultado para buffer se confirmado */
    if (g_dialogoResultadoOK && buffer != NULL) {
        strncpy(buffer, dados.texto, tamanho - 1);
        buffer[tamanho - 1] = '\0';  // Garantir terminação
    }

    return g_dialogoResultadoOK;
}

/******************************************************************************
 * FUNÇÃO PÚBLICA: MostrarDialogoNumero
 *
 * Wrapper sobre MostrarDialogoTexto que converte a entrada para número.
 *****************************************************************************/
BOOL MostrarDialogoNumero(HWND hwndParent, const char* titulo,
                          const char* mensagem, double* valor) {
    /* Buffer para armazenar texto digitado */
    char buffer[64];

    /* Chamar diálogo de texto */
    if (MostrarDialogoTexto(hwndParent, titulo, mensagem, buffer, sizeof(buffer))) {
        /* Usuário confirmou - converter texto para número */
        /* atof = ASCII to Float (converte string para double) */
        *valor = atof(buffer);
        return TRUE;
    }

    /* Usuário cancelou */
    return FALSE;
}

/******************************************************************************
 * FUNÇÃO PÚBLICA: MostrarDialogoCategoria
 *
 * Cria e exibe um diálogo modal com listbox para seleção de categoria.
 *****************************************************************************/
BOOL MostrarDialogoCategoria(HWND hwndParent, void* lista_categorias,
                             char* categoria_selecionada) {
    /* Inicializar dados */
    DadosDialogoCategoria dados;
    dados.lista_categorias = lista_categorias;
    dados.categoria_selecionada[0] = '\0';

    g_dialogoResultadoOK = FALSE;

    /* Registrar classe */
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = DefDlgProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszClassName = "DialogoCategoriaClass";
    RegisterClassEx(&wc);

    /* Criar janela */
    HWND hwndDialogo = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        "DialogoCategoriaClass",
        "Selecionar Categoria",
        DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU,
        0, 0, 420, 380,
        hwndParent,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if (!hwndDialogo) {
        return FALSE;
    }

    SetWindowLongPtr(hwndDialogo, GWLP_USERDATA, (LONG_PTR)&dados);

    /* Criar controles */

    /* Label */
    CreateWindowEx(0,
        "STATIC",
        "Selecione uma categoria da lista:",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        10, 10, 390, 20,
        hwndDialogo,
        (HMENU)100,
        NULL, NULL);

    /* Listbox */
    CreateWindowEx(WS_EX_CLIENTEDGE,
        "LISTBOX",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
        10, 35, 390, 260,
        hwndDialogo,
        (HMENU)ID_LISTBOX_CATEGORIAS,
        NULL, NULL);

    /* Botão OK */
    CreateWindowEx(0,
        "BUTTON",
        "OK",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        220, 305, 80, 30,
        hwndDialogo,
        (HMENU)ID_BTN_OK,
        NULL, NULL);

    /* Botão Cancelar */
    CreateWindowEx(0,
        "BUTTON",
        "Cancelar",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        310, 305, 90, 30,
        hwndDialogo,
        (HMENU)ID_BTN_CANCELAR,
        NULL, NULL);

    /* Centralizar */
    RECT rc;
    GetWindowRect(hwndDialogo, &rc);
    int x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;
    SetWindowPos(hwndDialogo, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    /* Desabilitar pai */
    EnableWindow(hwndParent, FALSE);

    /* Exibir */
    ShowWindow(hwndDialogo, SW_SHOW);
    UpdateWindow(hwndDialogo);

    /* Subclassear */
    SetWindowLongPtr(hwndDialogo, GWLP_WNDPROC, (LONG_PTR)DialogoCategoriaProc);

    /* Inicializar */
    DialogoCategoriaProc(hwndDialogo, WM_INITDIALOG, 0, (LPARAM)&dados);

    /* Loop de mensagens */
    MSG msg;
    BOOL executando = TRUE;

    while (executando && GetMessage(&msg, NULL, 0, 0)) {
        if (!IsWindow(hwndDialogo)) {
            executando = FALSE;
            break;
        }

        if (!IsDialogMessage(hwndDialogo, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    /* Reabilitar pai */
    EnableWindow(hwndParent, TRUE);
    SetForegroundWindow(hwndParent);

    /* Limpar */
    DestroyWindow(hwndDialogo);
    UnregisterClass("DialogoCategoriaClass", GetModuleHandle(NULL));

    /* Copiar resultado */
    if (g_dialogoResultadoOK && categoria_selecionada != NULL) {
        strcpy(categoria_selecionada, dados.categoria_selecionada);
    }

    return g_dialogoResultadoOK;
}
