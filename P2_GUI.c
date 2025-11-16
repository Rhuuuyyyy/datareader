#include <windows.h>
#include <stdio.h>
#include "tipos.h"
#include "arquivo.h"
#include "categoria.h"
#include "gui.h"

/* Declaração externa para SetAppData */
void SetAppData(AppData* app);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    const char* arquivo_bin = "dados.bin";

    /* Mostrar splash screen */
    MessageBox(NULL,
        "Sistema de Gerenciamento de Alimentos\n\n"
        "Trabalho de Programação Imperativa - RA2\n"
        "Carregando dados...",
        "Iniciando",
        MB_OK | MB_ICONINFORMATION);

    /* Carregar dados */
    NoCategoria* lista_categorias = carregar_dados_binario(arquivo_bin);

    if (lista_categorias == NULL) {
        MessageBox(NULL,
            "Falha ao carregar dados.bin!\n\n"
            "Certifique-se de que o arquivo dados.bin\n"
            "está na mesma pasta que P2_GUI.exe",
            "Erro",
            MB_OK | MB_ICONERROR);
        return 1;
    }

    /* Inicializar estrutura de dados da aplicação */
    AppData appData = {0};
    appData.lista_categorias = lista_categorias;
    appData.dados_modificados = false;

    /* Configurar dados globais */
    SetAppData(&appData);

    /* Inicializar interface gráfica */
    InicializarGUI(hInstance);

    /* Loop de mensagens */
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    /* Liberar memória */
    liberar_categorias(lista_categorias);

    return (int)msg.wParam;
}
