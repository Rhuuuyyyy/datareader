/******************************************************************************
 * P2_GUI.C - Programa Principal da Interface Gráfica
 *
 * Este é o ponto de entrada do programa P2_GUI.exe (versão com interface gráfica).
 * Responsável por:
 * 1. Carregar dados do arquivo binário (dados.bin)
 * 2. Inicializar a interface gráfica
 * 3. Executar o loop de mensagens do Windows
 * 4. Salvar alterações ao sair (se houve modificações)
 * 5. Liberar memória antes de encerrar
 *
 * ESTRUTURA DO PROGRAMA:
 * 1. Splash screen (mensagem de carregamento)
 * 2. Carregamento de dados do arquivo binário
 * 3. Inicialização da estrutura AppData
 * 4. Criação da janela e loop de mensagens
 * 5. Auto-save (se houver alterações)
 * 6. Liberação de memória
 *
 * ARQUIVOS RELACIONADOS:
 * - dados.bin: arquivo binário com 597 alimentos em 15 categorias
 * - arquivo.c: funções para carregar/salvar dados binários
 * - gui.c: implementação da interface gráfica
 * - categoria.c: funções de manipulação de listas encadeadas
 *****************************************************************************/

#include <windows.h>       // API do Windows (WinMain, MessageBox, etc)
#include <stdio.h>         // printf, sprintf (não usado em GUI mas incluído por compatibilidade)
#include "tipos.h"         // Definições de estruturas (NoCategoria, NoAlimento, AppData)
#include "arquivo.h"       // carregar_dados_binario, salvar_dados_binario
#include "categoria.h"     // liberar_categorias
#include "gui.h"           // SetAppData, InicializarGUI

/******************************************************************************
 * PROTÓTIPOS DE FUNÇÕES EXTERNAS
 *****************************************************************************/

/**
 * SetAppData - Definida em gui.c
 *
 * Define ponteiro global para AppData em gui.c, permitindo que as funções
 * de interface acessem os dados da aplicação.
 *
 * @param app: Ponteiro para estrutura AppData
 */
void SetAppData(AppData* app);

/******************************************************************************
 * WinMain - Ponto de Entrada do Programa Windows
 *
 * Esta é a função principal de programas Windows com interface gráfica.
 * Equivalente ao main() em programas de console.
 *
 * PARÂMETROS:
 * @param hInstance: Handle da instância atual do programa
 *                   Identifica unicamente esta execução do programa
 *                   Usado para criar janelas e carregar recursos
 *
 * @param hPrevInstance: Handle de instância prévia (obsoleto, sempre NULL)
 *                       Mantido por compatibilidade com Windows 16-bit
 *
 * @param lpCmdLine: Linha de comando (string) passada ao programa
 *                   Exemplo: se executar "P2_GUI.exe arquivo.txt"
 *                   lpCmdLine conterá "arquivo.txt"
 *
 * @param nCmdShow: Como a janela deve ser exibida inicialmente
 *                  SW_SHOW = normal, SW_MAXIMIZE = maximizada, etc.
 *
 * @return: Código de saída do programa (0 = sucesso, != 0 = erro)
 *
 * FLUXO DE EXECUÇÃO:
 * 1. Ignorar parâmetros não usados
 * 2. Mostrar splash screen (mensagem de carregamento)
 * 3. Carregar dados do arquivo binário
 * 4. Verificar se carregamento foi bem-sucedido
 * 5. Inicializar estrutura AppData
 * 6. Configurar dados globais para GUI
 * 7. Criar e exibir janela principal
 * 8. Loop de mensagens (aguarda eventos do usuário)
 * 9. Salvar dados se foram modificados
 * 10. Liberar memória
 * 11. Retornar código de saída
 *****************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

    /*** PASSO 1: EVITAR AVISOS DE COMPILAÇÃO PARA PARÂMETROS NÃO USADOS ***/

    /* Marcar parâmetros como intencionalmente não usados */
    /* (void) converte para void, dizendo ao compilador que é intencional */
    (void)hPrevInstance;  // Sempre NULL em Win32
    (void)lpCmdLine;      // Não usamos argumentos de linha de comando
    (void)nCmdShow;       // Usamos SW_SHOW diretamente em gui.c

    /*** PASSO 2: DEFINIR NOME DO ARQUIVO DE DADOS ***/

    /* Caminho do arquivo binário com dados dos alimentos */
    /* Arquivo deve estar na mesma pasta que P2_GUI.exe */
    const char* arquivo_bin = "dados.bin";

    /*** PASSO 3: SPLASH SCREEN (TELA DE CARREGAMENTO) ***/

    /* Exibir MessageBox informando que dados estão sendo carregados */
    /* MB_OK = apenas botão OK */
    /* MB_ICONINFORMATION = ícone de informação (i azul) */
    /* NULL = sem janela pai (MessageBox é independente) */
    MessageBox(NULL,
        "Sistema de Gerenciamento de Alimentos\n\n"        // Título do sistema
        "Trabalho de Programacao Imperativa - RA2\n"       // Identificação acadêmica
        "Carregando dados...",                             // Status
        "Iniciando",                                       // Título da janela
        MB_OK | MB_ICONINFORMATION);

    /*** PASSO 4: CARREGAR DADOS DO ARQUIVO BINÁRIO ***/

    /* Chamar função que:
     * 1. Abre dados.bin para leitura
     * 2. Lê estruturas AlimentoArquivo uma a uma
     * 3. Cria lista encadeada de categorias (ordenada alfabeticamente)
     * 4. Para cada categoria, cria:
     *    - Lista encadeada de alimentos (ordenada alfabeticamente)
     *    - Árvore binária indexada por energia
     *    - Árvore binária indexada por proteína
     * 5. Retorna ponteiro para primeira categoria ou NULL se falhar
     */
    NoCategoria* lista_categorias = carregar_dados_binario(arquivo_bin);

    /*** PASSO 5: VERIFICAR SE CARREGAMENTO FOI BEM-SUCEDIDO ***/

    if (lista_categorias == NULL) {
        /* Falha ao carregar dados - possíveis causas:
         * - Arquivo dados.bin não encontrado
         * - Arquivo corrompido
         * - Sem permissões de leitura
         * - Falta de memória
         */

        /* Exibir mensagem de erro ao usuário */
        /* MB_ICONERROR = ícone vermelho com X */
        MessageBox(NULL,
            "Falha ao carregar dados.bin!\n\n"
            "Certifique-se de que o arquivo dados.bin\n"
            "esta na mesma pasta que P2_GUI.exe",
            "Erro",                                        // Título
            MB_OK | MB_ICONERROR);

        /* Retornar 1 indicando erro */
        /* Em Windows, código != 0 indica falha */
        return 1;
    }

    /*** PASSO 6: INICIALIZAR ESTRUTURA DE DADOS DA APLICAÇÃO ***/

    /* AppData é estrutura que agrupa todos os dados da aplicação:
     * - lista_categorias: ponteiro para lista encadeada de categorias
     * - hwndMain: handle da janela principal (será preenchido em gui.c)
     * - hwndListBox: handle da listbox de resultados (será preenchido em gui.c)
     * - hwndStatus: handle da barra de status (será preenchido em gui.c)
     * - dados_modificados: flag booleana (false inicialmente)
     */

    /* Inicializar estrutura com zeros */
    /* {0} é sintaxe C para inicializar todos os campos com 0/NULL/false */
    AppData appData = {0};

    /* Preencher campo lista_categorias com dados carregados */
    appData.lista_categorias = lista_categorias;

    /* Marcar dados como não modificados inicialmente */
    /* Se usuário remover categoria/alimento, isso mudará para true */
    appData.dados_modificados = false;

    /*** PASSO 7: CONFIGURAR DADOS GLOBAIS PARA GUI ***/

    /* gui.c precisa acessar appData para exibir e modificar dados */
    /* SetAppData define ponteiro global g_appData em gui.c */
    /* Passamos ENDEREÇO de appData (&appData) */
    /* IMPORTANTE: appData está na stack, mas persistirá até fim de WinMain */
    SetAppData(&appData);

    /*** PASSO 8: INICIALIZAR INTERFACE GRÁFICA ***/

    /* InicializarGUI (definida em gui.c) faz:
     * 1. Registra classe de janela ("SistemaAlimentosClass")
     * 2. Cria janela principal com CreateWindowEx
     * 3. Preenche appData.hwndMain com handle da janela
     * 4. Exibe janela com ShowWindow
     */
    InicializarGUI(hInstance);

    /*** PASSO 9: LOOP DE MENSAGENS ***/

    /* Em programas Windows, eventos (cliques, teclas, etc) são enviados como
     * "mensagens" para uma fila.
     *
     * O loop de mensagens:
     * 1. Pega próxima mensagem da fila (GetMessage)
     * 2. Traduz teclas virtuais (TranslateMessage)
     * 3. Envia para procedimento de janela (DispatchMessage)
     * 4. Repete até receber WM_QUIT
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
     * 0, 0 = não filtrar por tipo de mensagem (pegar todas)
     */
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        /* TranslateMessage: Converte teclas virtuais em caracteres */
        /* Exemplo: WM_KEYDOWN -> WM_CHAR */
        /* Necessário para entrada de texto funcionar corretamente */
        TranslateMessage(&msg);

        /* DispatchMessage: Envia mensagem para procedimento de janela */
        /* Chama WndProc (definido em gui.c) com a mensagem */
        /* WndProc decide o que fazer baseado no tipo de mensagem */
        DispatchMessage(&msg);
    }
    /* Loop termina quando PostQuitMessage(0) é chamado */
    /* Isso acontece quando usuário fecha a janela ou clica em Sair */

    /*** PASSO 10: AUTO-SAVE (SALVAR ALTERAÇÕES SE NECESSÁRIO) ***/

    /* Verificar se dados foram modificados durante execução */
    /* dados_modificados é setado para true quando:
     * - Categoria é removida (OnRemoverCategoria em gui.c)
     * - Alimento é removido (OnRemoverAlimento em gui.c)
     */
    if (appData.dados_modificados) {
        /* Dados foram modificados - salvar automaticamente */

        /* Informar usuário que salvamento está em progresso */
        MessageBox(NULL,
            "Salvando alteracoes no arquivo dados.bin...",
            "Salvando",
            MB_OK | MB_ICONINFORMATION);

        /* Tentar salvar dados */
        /* salvar_dados_binario (definida em arquivo.c) faz:
         * 1. Percorre lista de categorias
         * 2. Para cada categoria, percorre lista de alimentos
         * 3. Converte NoAlimento para AlimentoArquivo (formato fixo)
         * 4. Escreve no arquivo binário
         * 5. Retorna true se sucesso, false se erro
         */
        if (salvar_dados_binario(lista_categorias, arquivo_bin)) {
            /* Salvamento bem-sucedido */
            MessageBox(NULL,
                "Arquivo atualizado com sucesso!",
                "Sucesso",
                MB_OK | MB_ICONINFORMATION);
        } else {
            /* Erro ao salvar - possíveis causas:
             * - Sem permissões de escrita
             * - Disco cheio
             * - Arquivo bloqueado por outro programa
             */
            MessageBox(NULL,
                "Erro ao salvar arquivo!",
                "Erro",
                MB_OK | MB_ICONERROR);
        }
    }
    /* Se dados NÃO foram modificados, não salva (otimização) */

    /*** PASSO 11: LIBERAR MEMÓRIA ***/

    /* Antes de encerrar programa, CRUCIAL liberar toda memória alocada */
    /* para evitar memory leaks (vazamento de memória) */

    /* liberar_categorias (definida em categoria.c) faz recursivamente:
     * 1. Para cada categoria:
     *    a. Libera árvore de energia (chamando liberar_arvore)
     *    b. Libera árvore de proteína (chamando liberar_arvore)
     *    c. Libera lista de alimentos (chamando liberar_lista_alimentos)
     *    d. Libera o nó de categoria
     * 2. Retorna NULL
     *
     * IMPORTANTE: Após liberar, lista_categorias aponta para memória
     * inválida, mas não importa porque programa está encerrando
     */
    liberar_categorias(lista_categorias);

    /*** PASSO 12: RETORNAR CÓDIGO DE SAÍDA ***/

    /* msg.wParam contém código de saída passado em PostQuitMessage */
    /* Normalmente 0 (sucesso) */
    /* Converter WPARAM (tipo do Windows) para int (tipo padrão C) */
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
 * carregar_dados_binario() lê arquivo
 *     |
 *     v
 * Cria estruturas em memória:
 * - Lista encadeada de categorias (NoCategoria)
 * - Para cada categoria:
 *   * Lista encadeada de alimentos (NoAlimento)
 *   * Árvore binária por energia (NoArvore)
 *   * Árvore binária por proteína (NoArvore)
 *     |
 *     v
 * AppData agrupa tudo e passa para GUI
 *     |
 *     v
 * GUI exibe dados e permite modificações
 * (listar, ordenar, filtrar, remover)
 *     |
 *     v
 * Se houve modificações (dados_modificados = true):
 *     |
 *     v
 * salvar_dados_binario() grava de volta em dados.bin
 *     |
 *     v
 * liberar_categorias() libera toda memória
 *     |
 *     v
 * Programa encerra
 *
 *****************************************************************************/
