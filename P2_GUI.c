/******************************************************************************
 * P2_GUI.C - Programa Principal da Interface Grafica
 *
 * Este e o ponto de entrada do programa P2_GUI.exe (versao com interface grafica).
 * Responsavel por:
 * 1. Carregar dados do arquivo binario (dados.bin)
 * 2. Inicializar a interface grafica
 * 3. Executar o loop de mensagens do Windows
 * 4. Salvar alteracoes ao sair (se houve modificacoes)
 * 5. Liberar memoria antes de encerrar
 *
 * ESTRUTURA DO PROGRAMA:
 * 1. Splash screen (mensagem de carregamento)
 * 2. Carregamento de dados do arquivo binario
 * 3. Inicializacao da estrutura AppData
 * 4. Criacao da janela e loop de mensagens
 * 5. Auto-save (se houver alteracoes)
 * 6. Liberacao de memoria
 *
 * ARQUIVOS RELACIONADOS:
 * - dados.bin: arquivo binario com 597 alimentos em 15 categorias
 * - arquivo.c: funcoes para carregar/salvar dados binarios
 * - gui.c: implementacao da interface grafica
 * - categoria.c: funcoes de manipulacao de listas encadeadas
 *****************************************************************************/

#include <windows.h>       // API do Windows (WinMain, MessageBox, etc)
#include <stdio.h>         // printf, sprintf (nao usado em GUI mas incluido por compatibilidade)
#include "tipos.h"         // Definicoes de estruturas (NoCategoria, NoAlimento, AppData)
#include "arquivo.h"       // carregar_dados_binario, salvar_dados_binario
#include "categoria.h"     // liberar_categorias
#include "gui.h"           // SetAppData, InicializarGUI

/******************************************************************************
 * PROTOTIPOS DE FUNCOES EXTERNAS
 *****************************************************************************/

/**
 * SetAppData - Definida em gui.c
 *
 * Define ponteiro global para AppData em gui.c, permitindo que as funcoes
 * de interface acessem os dados da aplicacao.
 *
 * @param app: Ponteiro para estrutura AppData
 */
void SetAppData(AppData* app);

/******************************************************************************
 * WinMain - Ponto de Entrada do Programa Windows
 *
 * Esta e a funcao principal de programas Windows com interface grafica.
 * Equivalente ao main() em programas de console.
 *
 * PARAMETROS:
 * @param hInstance: Handle da instancia atual do programa
 *                   Identifica unicamente esta execucao do programa
 *                   Usado para criar janelas e carregar recursos
 *
 * @param hPrevInstance: Handle de instancia previa (obsoleto, sempre NULL)
 *                       Mantido por compatibilidade com Windows 16-bit
 *
 * @param lpCmdLine: Linha de comando (string) passada ao programa
 *                   Exemplo: se executar "P2_GUI.exe arquivo.txt"
 *                   lpCmdLine contera "arquivo.txt"
 *
 * @param nCmdShow: Como a janela deve ser exibida inicialmente
 *                  SW_SHOW = normal, SW_MAXIMIZE = maximizada, etc.
 *
 * @return: Codigo de saida do programa (0 = sucesso, != 0 = erro)
 *
 * FLUXO DE EXECUCAO:
 * 1. Ignorar parametros nao usados
 * 2. Mostrar splash screen (mensagem de carregamento)
 * 3. Carregar dados do arquivo binario
 * 4. Verificar se carregamento foi bem-sucedido
 * 5. Inicializar estrutura AppData
 * 6. Configurar dados globais para GUI
 * 7. Criar e exibir janela principal
 * 8. Loop de mensagens (aguarda eventos do usuario)
 * 9. Salvar dados se foram modificados
 * 10. Liberar memoria
 * 11. Retornar codigo de saida
 *****************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

    /*** PASSO 1: EVITAR AVISOS DE COMPILACAO PARA PARAMETROS NAO USADOS ***/

    /* Marcar parametros como intencionalmente nao usados */
    /* (void) converte para void, dizendo ao compilador que e intencional */
    (void)hPrevInstance;  // Sempre NULL em Win32
    (void)lpCmdLine;      // Nao usamos argumentos de linha de comando
    (void)nCmdShow;       // Usamos SW_SHOW diretamente em gui.c

    /*** PASSO 2: DEFINIR NOME DO ARQUIVO DE DADOS ***/

    /* Caminho do arquivo binario com dados dos alimentos */
    /* Arquivo deve estar na mesma pasta que P2_GUI.exe */
    const char* arquivo_bin = "dados.bin";

    /*** PASSO 3: SPLASH SCREEN (TELA DE CARREGAMENTO) ***/

    /* Exibir MessageBox informando que dados estao sendo carregados */
    /* MB_OK = apenas botao OK */
    /* MB_ICONINFORMATION = icone de informacao (i azul) */
    /* NULL = sem janela pai (MessageBox e independente) */
    MessageBox(NULL,
        "Sistema de Gerenciamento de Alimentos\n\n"        // Titulo do sistema
        "Trabalho de Programacao Imperativa - RA2\n"       // Identificacao academica
        "Carregando dados...",                             // Status
        "Iniciando",                                       // Titulo da janela
        MB_OK | MB_ICONINFORMATION);

    /*** PASSO 4: CARREGAR DADOS DO ARQUIVO BINARIO ***/

    /* Chamar funcao que:
     * 1. Abre dados.bin para leitura
     * 2. Le estruturas AlimentoArquivo uma a uma
     * 3. Cria lista encadeada de categorias (ordenada alfabeticamente)
     * 4. Para cada categoria, cria:
     *    - Lista encadeada de alimentos (ordenada alfabeticamente)
     *    - Arvore binaria indexada por energia
     *    - Arvore binaria indexada por proteina
     * 5. Retorna ponteiro para primeira categoria ou NULL se falhar
     */
    NoCategoria* lista_categorias = carregar_dados_binario(arquivo_bin);

    /*** PASSO 5: VERIFICAR SE CARREGAMENTO FOI BEM-SUCEDIDO ***/

    if (lista_categorias == NULL) {
        /* Falha ao carregar dados - possiveis causas:
         * - Arquivo dados.bin nao encontrado
         * - Arquivo corrompido
         * - Sem permissoes de leitura
         * - Falta de memoria
         */

        /* Exibir mensagem de erro ao usuario */
        /* MB_ICONERROR = icone vermelho com X */
        MessageBox(NULL,
            "Falha ao carregar dados.bin!\n\n"
            "Certifique-se de que o arquivo dados.bin\n"
            "esta na mesma pasta que P2_GUI.exe",
            "Erro",                                        // Titulo
            MB_OK | MB_ICONERROR);

        /* Retornar 1 indicando erro */
        /* Em Windows, codigo != 0 indica falha */
        return 1;
    }

    /*** PASSO 6: INICIALIZAR ESTRUTURA DE DADOS DA APLICACAO ***/

    /* AppData e estrutura que agrupa todos os dados da aplicacao:
     * - lista_categorias: ponteiro para lista encadeada de categorias
     * - hwndMain: handle da janela principal (sera preenchido em gui.c)
     * - hwndListBox: handle da listbox de resultados (sera preenchido em gui.c)
     * - hwndStatus: handle da barra de status (sera preenchido em gui.c)
     * - dados_modificados: flag booleana (false inicialmente)
     */

    /* Inicializar estrutura com zeros */
    /* {0} e sintaxe C para inicializar todos os campos com 0/NULL/false */
    AppData appData = {0};

    /* Preencher campo lista_categorias com dados carregados */
    appData.lista_categorias = lista_categorias;

    /* Marcar dados como nao modificados inicialmente */
    /* Se usuario remover categoria/alimento, isso mudara para true */
    appData.dados_modificados = false;

    /*** PASSO 7: CONFIGURAR DADOS GLOBAIS PARA GUI ***/

    /* gui.c precisa acessar appData para exibir e modificar dados */
    /* SetAppData define ponteiro global g_appData em gui.c */
    /* Passamos ENDERECO de appData (&appData) */
    /* IMPORTANTE: appData esta na stack, mas persistira ate fim de WinMain */
    SetAppData(&appData);

    /*** PASSO 8: INICIALIZAR INTERFACE GRAFICA ***/

    /* InicializarGUI (definida em gui.c) faz:
     * 1. Registra classe de janela ("SistemaAlimentosClass")
     * 2. Cria janela principal com CreateWindowEx
     * 3. Preenche appData.hwndMain com handle da janela
     * 4. Exibe janela com ShowWindow
     */
    InicializarGUI(hInstance);

    /*** PASSO 9: LOOP DE MENSAGENS ***/

    /* Em programas Windows, eventos (cliques, teclas, etc) sao enviados como
     * "mensagens" para uma fila.
     *
     * O loop de mensagens:
     * 1. Pega proxima mensagem da fila (GetMessage)
     * 2. Traduz teclas virtuais (TranslateMessage)
     * 3. Envia para procedimento de janela (DispatchMessage)
     * 4. Repete ate receber WM_QUIT
     */

    /* Estrutura para armazenar mensagem atual */
    MSG msg;

    /* Loop principal do programa */
    /* GetMessage retorna:
     * - > 0 se recebeu mensagem normal (continuar loop)
     * - 0 se recebeu WM_QUIT (sair do loop)
     * - < 0 se erro (sair do loop)
     *
     * NULL = pegar mensagens de qualquer janela do programa
     * 0, 0 = nao filtrar por tipo de mensagem (pegar todas)
     */
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        /* TranslateMessage: Converte teclas virtuais em caracteres */
        /* Exemplo: WM_KEYDOWN -> WM_CHAR */
        /* Necessario para entrada de texto funcionar corretamente */
        TranslateMessage(&msg);

        /* DispatchMessage: Envia mensagem para procedimento de janela */
        /* Chama WndProc (definido em gui.c) com a mensagem */
        /* WndProc decide o que fazer baseado no tipo de mensagem */
        DispatchMessage(&msg);
    }
    /* Loop termina quando PostQuitMessage(0) e chamado */
    /* Isso acontece quando usuario fecha a janela ou clica em Sair */

    /*** PASSO 10: AUTO-SAVE (SALVAR ALTERACOES SE NECESSARIO) ***/

    /* Verificar se dados foram modificados durante execucao */
    /* dados_modificados e setado para true quando:
     * - Categoria e removida (OnRemoverCategoria em gui.c)
     * - Alimento e removido (OnRemoverAlimento em gui.c)
     */
    if (appData.dados_modificados) {
        /* Dados foram modificados - salvar automaticamente */

        /* Informar usuario que salvamento esta em progresso */
        MessageBox(NULL,
            "Salvando alteracoes no arquivo dados.bin...",
            "Salvando",
            MB_OK | MB_ICONINFORMATION);

        /* Tentar salvar dados */
        /* salvar_dados_binario (definida em arquivo.c) faz:
         * 1. Percorre lista de categorias
         * 2. Para cada categoria, percorre lista de alimentos
         * 3. Converte NoAlimento para AlimentoArquivo (formato fixo)
         * 4. Escreve no arquivo binario
         * 5. Retorna true se sucesso, false se erro
         */
        if (salvar_dados_binario(lista_categorias, arquivo_bin)) {
            /* Salvamento bem-sucedido */
            MessageBox(NULL,
                "Arquivo atualizado com sucesso!",
                "Sucesso",
                MB_OK | MB_ICONINFORMATION);
        } else {
            /* Erro ao salvar - possiveis causas:
             * - Sem permissoes de escrita
             * - Disco cheio
             * - Arquivo bloqueado por outro programa
             */
            MessageBox(NULL,
                "Erro ao salvar arquivo!",
                "Erro",
                MB_OK | MB_ICONERROR);
        }
    }
    /* Se dados NAO foram modificados, nao salva (otimizacao) */

    /*** PASSO 11: LIBERAR MEMORIA ***/

    /* Antes de encerrar programa, CRUCIAL liberar toda memoria alocada */
    /* para evitar memory leaks (vazamento de memoria) */

    /* liberar_categorias (definida em categoria.c) faz recursivamente:
     * 1. Para cada categoria:
     *    a. Libera arvore de energia (chamando liberar_arvore)
     *    b. Libera arvore de proteina (chamando liberar_arvore)
     *    c. Libera lista de alimentos (chamando liberar_lista_alimentos)
     *    d. Libera o no de categoria
     * 2. Retorna NULL
     *
     * IMPORTANTE: Apos liberar, lista_categorias aponta para memoria
     * invalida, mas nao importa porque programa esta encerrando
     */
    liberar_categorias(lista_categorias);

    /*** PASSO 12: RETORNAR CODIGO DE SAIDA ***/

    /* msg.wParam contem codigo de saida passado em PostQuitMessage */
    /* Normalmente 0 (sucesso) */
    /* Converter WPARAM (tipo do Windows) para int (tipo padrao C) */
    return (int)msg.wParam;
}

/******************************************************************************
 * FIM DO PROGRAMA PRINCIPAL
 *
 * FLUXO COMPLETO DE DADOS:
 *
 * dados.bin (arquivo em disco)
 *     |
 *     v
 * carregar_dados_binario() le arquivo
 *     |
 *     v
 * Cria estruturas em memoria:
 * - Lista encadeada de categorias (NoCategoria)
 * - Para cada categoria:
 *   * Lista encadeada de alimentos (NoAlimento)
 *   * Arvore binaria por energia (NoArvore)
 *   * Arvore binaria por proteina (NoArvore)
 *     |
 *     v
 * AppData agrupa tudo e passa para GUI
 *     |
 *     v
 * GUI exibe dados e permite modificacoes
 * (listar, ordenar, filtrar, remover)
 *     |
 *     v
 * Se houve modificacoes (dados_modificados = true):
 *     |
 *     v
 * salvar_dados_binario() grava de volta em dados.bin
 *     |
 *     v
 * liberar_categorias() libera toda memoria
 *     |
 *     v
 * Programa encerra
 *
 *****************************************************************************/
