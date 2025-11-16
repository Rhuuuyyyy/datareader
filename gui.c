/******************************************************************************
 * GUI.C - Interface Gráfica do Sistema de Gerenciamento de Alimentos
 *
 * Este arquivo implementa toda a interface gráfica do sistema usando Win32 API.
 * Contém as funções de criar controles, processar eventos de botões e
 * exibir dados na listbox.
 *
 * ESTRUTURA:
 * 1. Funções auxiliares (limpar listbox, adicionar itens, criar tabelas)
 * 2. Criação de controles (botões, listbox, labels)
 * 3. Handlers de cada uma das 9 funcionalidades
 * 4. Procedimento de janela (processa todos os eventos)
 * 5. Inicialização da GUI
 *
 * FUNCIONALIDADES IMPLEMENTADAS:
 * 1. Listar categorias alfabeticamente
 * 2. Listar alimentos de uma categoria
 * 3. Ordenar alimentos por energia (decrescente)
 * 4. Ordenar alimentos por proteina (decrescente)
 * 5. Filtrar alimentos por intervalo de energia
 * 6. Filtrar alimentos por intervalo de proteina
 * 7. Remover categoria completa
 * 8. Remover alimento específico
 * 9. Sair do sistema
 *****************************************************************************/

#include "gui.h"           // Definições de IDs, estruturas, protótipos
#include "categoria.h"     // Funções de manipulação de categorias
#include "arvore.h"        // Funções de árvore binária
#include "dialogo.h"       // Sistema de diálogos personalizados
#include <stdio.h>         // sprintf, printf
#include <stdlib.h>        // malloc, free
#include <string.h>        // strcpy, strcmp
#include <commctrl.h>      // Controles comuns do Windows

/******************************************************************************
 * VARIÁVEIS GLOBAIS PRIVADAS
 *
 * Estas variáveis são acessíveis apenas dentro deste arquivo (static).
 * Armazenam dados compartilhados entre funções.
 *****************************************************************************/

/* Ponteiro global para os dados da aplicação (lista de categorias, handles de janelas) */
static AppData* g_appData = NULL;

/* Fontes (tipos de letra) usadas na interface */
static HFONT g_hFontMono = NULL;    // Fonte monoespaçada (Consolas) para tabelas
static HFONT g_hFontNormal = NULL;  // Fonte normal (Segoe UI) para títulos

/******************************************************************************
 * FUNÇÕES AUXILIARES PARA MANIPULAÇÃO DA LISTBOX
 *
 * Estas funções encapsulam operações comuns na listbox (caixa de listagem)
 * onde os resultados são exibidos.
 *****************************************************************************/

/**
 * LimparListBox - Remove todos os itens da listbox
 *
 * @param hwndListBox: Handle (identificador) da listbox a limpar
 *
 * FUNCIONAMENTO:
 * - SendMessage envia uma mensagem para um controle Windows
 * - LB_RESETCONTENT é a mensagem que limpa todos os itens de uma listbox
 * - Retorna 0 se bem-sucedido
 */
void LimparListBox(HWND hwndListBox) {
    SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);
}

/**
 * AdicionarItemListBox - Adiciona uma linha de texto à listbox
 *
 * @param hwndListBox: Handle da listbox
 * @param texto: String a ser adicionada
 *
 * FUNCIONAMENTO:
 * - LB_ADDSTRING adiciona um item ao final da lista
 * - (LPARAM) faz cast do ponteiro de string para o tipo esperado
 * - O item é adicionado na próxima posição disponível
 */
void AdicionarItemListBox(HWND hwndListBox, const char* texto) {
    SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)texto);
}

/**
 * MostrarMensagem - Exibe uma caixa de diálogo simples com mensagem
 *
 * @param hwnd: Janela pai (a mensagem aparece sobre ela)
 * @param titulo: Título da janela de mensagem
 * @param mensagem: Texto da mensagem
 *
 * FUNCIONAMENTO:
 * - MessageBox é função padrão do Windows para mostrar mensagens
 * - MB_OK: apenas botão OK
 * - MB_ICONINFORMATION: ícone de informação (i azul)
 */
void MostrarMensagem(HWND hwnd, const char* titulo, const char* mensagem) {
    MessageBox(hwnd, mensagem, titulo, MB_OK | MB_ICONINFORMATION);
}

/**
 * AdicionarSeparador - Adiciona uma linha separadora feita de traços
 *
 * @param hwndListBox: Handle da listbox
 * @param tamanho: Número de traços (comprimento da linha)
 *
 * FUNCIONAMENTO:
 * 1. Cria um array de caracteres
 * 2. Preenche com '-' até o tamanho especificado
 * 3. Adiciona '\0' (terminador de string) no final
 * 4. Adiciona à listbox
 */
void AdicionarSeparador(HWND hwndListBox, int tamanho) {
    char separador[256];  // Buffer para armazenar a linha de traços
    int i;

    /* Loop para preencher buffer com traços */
    for (i = 0; i < tamanho && i < 255; i++) {
        separador[i] = '-';  // Cada posição recebe um traço
    }

    /* Adicionar terminador nulo (obrigatório em strings C) */
    separador[i] = '\0';

    /* Adicionar à listbox */
    AdicionarItemListBox(hwndListBox, separador);
}

/**
 * AdicionarCabecalhoTabela - Adiciona o cabeçalho padrão das tabelas de alimentos
 *
 * @param hwndListBox: Handle da listbox
 *
 * FUNCIONAMENTO:
 * Cria uma tabela bonita com:
 * - Linha separadora superior
 * - Cabeçalhos das colunas (Num, Descricao, Energia, Proteina)
 * - Linha separadora inferior
 *
 * FORMATO:
 * ------------------------------------------------------
 *  Num | Descricao                      | Energia  | Proteina
 * ------------------------------------------------------
 */
void AdicionarCabecalhoTabela(HWND hwndListBox) {
    /* Linha separadora superior (110 traços) */
    AdicionarSeparador(hwndListBox, 110);

    /* Cabeçalhos das colunas, formatados para alinhar */
    /* Espaços são cuidadosamente calculados para alinhamento com fonte monoespaçada */
    AdicionarItemListBox(hwndListBox, " Num | Descricao                                       | Energia  | Proteina ");

    /* Linha separadora inferior */
    AdicionarSeparador(hwndListBox, 110);
}

/******************************************************************************
 * CRIAÇÃO DOS CONTROLES DA INTERFACE
 *
 * Esta função cria todos os elementos visuais da janela:
 * - Títulos
 * - Botões das 9 funcionalidades
 * - Listbox para exibir resultados
 * - Barra de status
 *****************************************************************************/

/**
 * CriarControles - Cria todos os controles (botões, textos, listbox) da interface
 *
 * @param hwnd: Handle da janela principal onde os controles serão criados
 *
 * FUNCIONAMENTO:
 * 1. Cria fontes personalizadas (Segoe UI para títulos, Consolas para tabelas)
 * 2. Cria labels de título e subtítulo
 * 3. Cria 9 botões (2 linhas de botões)
 * 4. Cria grupo de resultados com listbox
 * 5. Cria barra de status
 *
 * LAYOUT DA INTERFACE:
 * +----------------------------------------------------------+
 * |       SISTEMA DE GERENCIAMENTO DE ALIMENTOS             |
 * |   Trabalho de Programação Imperativa - RA2              |
 * +----------------------------------------------------------+
 * | [1.Listar] [2.Listar] [3.Energia] [4.Proteina]          |
 * | [5.Filtro] [6.Filtro] [7.Remover] [8.Remover] [9.SAIR]  |
 * +----------------------------------------------------------+
 * | Resultados                                               |
 * | +------------------------------------------------------+ |
 * | |                                                      | |
 * | |              (Listbox com resultados)                | |
 * | |                                                      | |
 * | +------------------------------------------------------+ |
 * +----------------------------------------------------------+
 * | Status: Pronto. Sistema carregado...                     |
 * +----------------------------------------------------------+
 */
void CriarControles(HWND hwnd) {
    /*** CRIAÇÃO DE FONTES PERSONALIZADAS ***/

    /* Fonte normal (Segoe UI, negrito, 18px) para títulos */
    /* CreateFont cria uma fonte com parâmetros específicos */
    g_hFontNormal = CreateFont(
        18,                        // Altura da fonte em pixels
        0,                         // Largura (0 = automático baseado na altura)
        0,                         // Ângulo de escape (rotação)
        0,                         // Ângulo de orientação
        FW_BOLD,                   // Peso da fonte (FW_BOLD = negrito)
        FALSE,                     // Itálico? (FALSE = não)
        FALSE,                     // Sublinhado? (FALSE = não)
        FALSE,                     // Riscado? (FALSE = não)
        DEFAULT_CHARSET,           // Conjunto de caracteres
        OUT_DEFAULT_PRECIS,        // Precisão de saída
        CLIP_DEFAULT_PRECIS,       // Precisão de recorte
        DEFAULT_QUALITY,           // Qualidade
        DEFAULT_PITCH | FF_DONTCARE, // Pitch e família
        "Segoe UI"                 // Nome da fonte
    );

    /* Fonte monoespaçada (Consolas, 16px) para tabelas */
    /* Fonte monoespaçada = cada caractere tem mesma largura, perfeito para alinhar colunas */
    g_hFontMono = CreateFont(
        16,                        // Altura 16px
        0,                         // Largura automática
        0,                         // Sem rotação
        0,                         // Sem inclinação
        FW_NORMAL,                 // Peso normal (não negrito)
        FALSE, FALSE, FALSE,       // Sem itálico, sublinhado ou riscado
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN,   // FIXED_PITCH = monoespaçada, FF_MODERN = família moderna
        "Consolas"                 // Nome da fonte
    );

    /*** CRIAÇÃO DE CONTROLES DE INTERFACE ***/

    /* TÍTULO PRINCIPAL */
    /* CreateWindow cria um controle filho dentro da janela principal */
    HWND hwndTitulo = CreateWindow(
        "STATIC",                  // Classe: texto estático (não editável)
        "SISTEMA DE GERENCIAMENTO DE ALIMENTOS",  // Texto a exibir
        WS_VISIBLE | WS_CHILD | SS_CENTER,  // Estilos: visível, filho, centralizado
        10, 10,                    // Posição X, Y (pixels do canto superior esquerdo)
        980, 35,                   // Largura, Altura
        hwnd,                      // Janela pai
        NULL,                      // ID do menu (não usado para STATIC)
        NULL,                      // Handle da instância (NULL = usar padrão)
        NULL                       // Parâmetros adicionais
    );
    /* Aplicar fonte ao título */
    /* WM_SETFONT = mensagem para definir fonte de um controle */
    SendMessage(hwndTitulo, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);

    /* SUBTÍTULO */
    CreateWindow(
        "STATIC",
        "Trabalho de Programacao Imperativa - RA2",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        10, 45, 980, 20,
        hwnd, NULL, NULL, NULL
    );

    /*** PRIMEIRA LINHA DE BOTÕES (4 botões) ***/

    /* BOTÃO 1: Listar Categorias */
    CreateWindow(
        "BUTTON",                  // Classe: botão
        "1. Listar Categorias",    // Texto do botão
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Estilos: visível, filho, push button
        10, 75,                    // Posição X=10, Y=75
        240, 45,                   // Largura=240, Altura=45
        hwnd,                      // Janela pai
        (HMENU)ID_BTN_LISTAR_CATEGORIAS,  // ID do botão (usado para identificar cliques)
        NULL, NULL
    );

    /* BOTÃO 2: Listar Alimentos */
    CreateWindow(
        "BUTTON",
        "2. Listar Alimentos",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        260, 75,                   // X=260 (espaçado 250 + 10 de margem do anterior)
        240, 45,
        hwnd,
        (HMENU)ID_BTN_LISTAR_ALIMENTOS,
        NULL, NULL
    );

    /* BOTÃO 3: Ordenar por Energia */
    CreateWindow(
        "BUTTON",
        "3. Ordenar por Energia",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        510, 75,                   // X=510
        240, 45,
        hwnd,
        (HMENU)ID_BTN_ENERGIA_DESC,
        NULL, NULL
    );

    /* BOTÃO 4: Ordenar por Proteina */
    CreateWindow(
        "BUTTON",
        "4. Ordenar por Proteina",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        760, 75,                   // X=760
        240, 45,
        hwnd,
        (HMENU)ID_BTN_PROTEINA_DESC,
        NULL, NULL
    );

    /*** SEGUNDA LINHA DE BOTÕES (5 botões) ***/

    /* BOTÃO 5: Filtrar por Energia */
    CreateWindow(
        "BUTTON",
        "5. Filtrar Energia",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10, 130,                   // Y=130 (segunda linha)
        240, 45,
        hwnd,
        (HMENU)ID_BTN_INTERVALO_ENERGIA,
        NULL, NULL
    );

    /* BOTÃO 6: Filtrar por Proteina */
    CreateWindow(
        "BUTTON",
        "6. Filtrar Proteina",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        260, 130,
        240, 45,
        hwnd,
        (HMENU)ID_BTN_INTERVALO_PROTEINA,
        NULL, NULL
    );

    /* BOTÃO 7: Remover Categoria */
    CreateWindow(
        "BUTTON",
        "7. Remover Categoria",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        510, 130,
        240, 45,
        hwnd,
        (HMENU)ID_BTN_REMOVER_CATEGORIA,
        NULL, NULL
    );

    /* BOTÃO 8: Remover Alimento */
    CreateWindow(
        "BUTTON",
        "8. Remover Alimento",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        760, 130,
        150, 45,                   // Largura menor (150) para dar espaço ao botão Sair
        hwnd,
        (HMENU)ID_BTN_REMOVER_ALIMENTO,
        NULL, NULL
    );

    /* BOTÃO 9: SAIR */
    CreateWindow(
        "BUTTON",
        "9. SAIR",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        920, 130,                  // X=920 (mais à direita)
        80, 45,                    // Largura menor (80)
        hwnd,
        (HMENU)ID_BTN_SAIR,
        NULL, NULL
    );

    /*** GRUPO DE RESULTADOS ***/

    /* GroupBox (moldura visual com título "Resultados") */
    CreateWindow(
        "BUTTON",                  // BUTTON com BS_GROUPBOX cria uma moldura
        "Resultados",              // Texto da moldura
        WS_VISIBLE | WS_CHILD | BS_GROUPBOX,  // BS_GROUPBOX = estilo de grupo
        10, 185,                   // Posição Y=185 (abaixo dos botões)
        990, 430,                  // Largura=990, Altura=430 (caixa grande)
        hwnd, NULL, NULL, NULL
    );

    /* LISTBOX - Caixa de listagem para exibir resultados */
    /* Esta é a área onde todas as tabelas e listas serão exibidas */
    g_appData->hwndListBox = CreateWindowEx(
        WS_EX_CLIENTEDGE,          // Estilo estendido: borda 3D
        "LISTBOX",                 // Classe: caixa de listagem
        NULL,                      // Sem texto inicial
        WS_CHILD | WS_VISIBLE |    // Estilos: filho, visível
        WS_VSCROLL |               // Com barra de rolagem vertical
        WS_HSCROLL |               // Com barra de rolagem horizontal
        LBS_NOINTEGRALHEIGHT |     // Permite altura não-inteira de itens
        LBS_NOTIFY,                // Notifica pai quando seleção muda
        20, 205,                   // Posição X=20, Y=205 (dentro do grupo)
        970, 395,                  // Largura=970, Altura=395
        hwnd,                      // Janela pai
        (HMENU)ID_LISTBOX_RESULTADO,  // ID da listbox
        NULL, NULL
    );

    /* Aplicar fonte monoespaçada à listbox */
    /* Isso garante que tabelas fiquem perfeitamente alinhadas */
    SendMessage(g_appData->hwndListBox, WM_SETFONT, (WPARAM)g_hFontMono, TRUE);

    /*** BARRA DE STATUS ***/

    /* Texto estático no rodapé mostrando status do sistema */
    g_appData->hwndStatus = CreateWindowEx(
        WS_EX_STATICEDGE,          // Borda estática (leve relevo)
        "STATIC",
        "Pronto. Sistema carregado com 597 alimentos em 15 categorias.",
        WS_CHILD | WS_VISIBLE | SS_LEFT,  // Filho, visível, alinhado à esquerda
        10, 625,                   // Posição Y=625 (rodapé)
        990, 25,                   // Altura=25
        hwnd,
        (HMENU)ID_STATUSBAR,
        NULL, NULL
    );
}

/******************************************************************************
 * HANDLER 1: LISTAR CATEGORIAS
 *
 * Exibe todas as categorias em ordem alfabética.
 * Usa a lista encadeada ordenada de categorias.
 *****************************************************************************/

/**
 * OnListarCategorias - Handler do botão "1. Listar Categorias"
 *
 * @param app: Ponteiro para estrutura com dados da aplicação
 *
 * FUNCIONAMENTO:
 * 1. Limpa a listbox
 * 2. Verifica se há categorias
 * 3. Cria cabeçalho visual bonito
 * 4. Percorre lista encadeada de categorias
 * 5. Adiciona cada categoria numerada
 * 6. Mostra total de categorias
 * 7. Atualiza barra de status
 *
 * ESTRUTURA DE DADOS:
 * app->lista_categorias é uma lista encadeada ordenada alfabeticamente
 * Cada NoCategoria tem:
 * - nome: string com nome da categoria
 * - proximo: ponteiro para próxima categoria (NULL no fim)
 */
void OnListarCategorias(AppData* app) {
    /* Limpar listbox antes de adicionar novo conteúdo */
    LimparListBox(app->hwndListBox);

    /* Verificar se lista está vazia */
    if (app->lista_categorias == NULL) {
        AdicionarItemListBox(app->hwndListBox, "Nenhuma categoria encontrada.");
        return;  // Sair da função se não há dados
    }

    /* Adicionar cabeçalho visual bonito com bordas ASCII */
    AdicionarItemListBox(app->hwndListBox, "");  // Linha em branco para espaçamento
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "|              CATEGORIAS DE ALIMENTOS - ORDEM ALFABETICA                |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Variáveis para percorrer lista */
    int contador = 1;                         // Contador para numerar categorias (inicia em 1)
    NoCategoria* atual = app->lista_categorias;  // Ponteiro para navegar pela lista
    char buffer[512];                         // Buffer temporário para formatar strings

    /* Loop para percorrer toda a lista encadeada */
    /* Continua enquanto 'atual' não for NULL (fim da lista) */
    while (atual != NULL) {
        /* Formatar string: "  1.  Alimentos preparados" */
        /* %2d = inteiro com 2 dígitos (alinhado à direita) */
        /* %s = string */
        sprintf(buffer, "  %2d.  %s", contador, atual->nome);

        /* Adicionar à listbox */
        AdicionarItemListBox(app->hwndListBox, buffer);

        /* Incrementar contador para próxima categoria */
        contador++;

        /* Avançar para próximo nó da lista */
        /* atual->proximo aponta para o próximo NoCategoria */
        /* Se for o último, proximo = NULL e loop termina */
        atual = atual->proximo;
    }

    /* Adicionar rodapé com total */
    AdicionarItemListBox(app->hwndListBox, "");
    /* contador - 1 porque incrementamos uma vez a mais no loop */
    sprintf(buffer, "Total: %d categorias", contador - 1);
    AdicionarItemListBox(app->hwndListBox, buffer);

    /* Atualizar barra de status no rodapé da janela */
    SetWindowText(app->hwndStatus, "Categorias listadas com sucesso.");
}

/******************************************************************************
 * HANDLER 2: LISTAR ALIMENTOS DE UMA CATEGORIA
 *
 * Mostra diálogo para selecionar categoria, depois lista todos os alimentos
 * dessa categoria em ordem alfabética.
 *****************************************************************************/

/**
 * OnListarAlimentos - Handler do botão "2. Listar Alimentos"
 *
 * @param app: Ponteiro para dados da aplicação
 *
 * FUNCIONAMENTO:
 * 1. Abre diálogo para usuário selecionar categoria
 * 2. Busca a categoria selecionada na lista
 * 3. Percorre lista de alimentos da categoria
 * 4. Exibe em formato de tabela bonita
 *
 * ESTRUTURA DE DADOS:
 * NoCategoria contém:
 * - lista_alimentos: lista encadeada de NoAlimento
 * Cada NoAlimento contém:
 * - numero: número de identificação
 * - descricao: nome do alimento
 * - energia_kcal: energia em kcal
 * - proteina: proteina em gramas
 * - proximo: próximo alimento na lista
 */
void OnListarAlimentos(AppData* app) {
    char categoria[MAX_CATEGORIA];  // Buffer para armazenar nome da categoria selecionada

    /* Mostrar diálogo de seleção de categoria */
    /* Retorna FALSE se usuário cancelou */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;  // Usuário cancelou, sair da função
    }

    /* Buscar categoria na lista usando nome */
    /* buscar_categoria retorna ponteiro para NoCategoria ou NULL se não encontrou */
    NoCategoria* cat = buscar_categoria(app->lista_categorias, categoria);

    /* Limpar listbox para novo conteúdo */
    LimparListBox(app->hwndListBox);

    /* Verificar se categoria foi encontrada */
    if (cat == NULL) {
        AdicionarItemListBox(app->hwndListBox, "Categoria nao encontrada!");
        return;
    }

    /* Buffer para formatar strings */
    char buffer[512];

    /* Adicionar cabeçalho com nome da categoria */
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");

    /* Cabeçalho com nome da categoria centralizado */
    /* %-70s = string alinhada à esquerda com 70 caracteres */
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);

    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Adicionar cabeçalho da tabela de alimentos */
    AdicionarCabecalhoTabela(app->hwndListBox);

    /* Percorrer lista de alimentos da categoria */
    int total = 0;                          // Contador de alimentos
    NoAlimento* alim = cat->lista_alimentos;  // Ponteiro para navegar lista de alimentos

    while (alim != NULL) {
        /* Formatar linha da tabela com dados do alimento */
        /* %3d = número com 3 dígitos, alinhado à direita */
        /* %-50s = descrição com 50 caracteres, alinhada à esquerda */
        /* %4d = energia com 4 dígitos */
        /* %6.1f = proteina com 6 caracteres total, 1 casa decimal */
        sprintf(buffer, " %3d | %-50s | %4d kcal | %6.1f g",
                alim->numero,
                alim->descricao,
                alim->energia_kcal,
                alim->proteina);

        AdicionarItemListBox(app->hwndListBox, buffer);

        /* Avançar para próximo alimento */
        alim = alim->proximo;
        total++;  // Incrementar contador
    }

    /* Adicionar rodapé */
    AdicionarSeparador(app->hwndListBox, 110);
    sprintf(buffer, "Total: %d alimentos", total);
    AdicionarItemListBox(app->hwndListBox, buffer);

    /* Atualizar status */
    sprintf(buffer, "Alimentos da categoria '%s' listados (ordem alfabetica).", cat->nome);
    SetWindowText(app->hwndStatus, buffer);
}

/******************************************************************************
 * FUNÇÕES AUXILIARES RECURSIVAS PARA ÁRVORES BINÁRIAS
 *
 * Estas funções percorrem árvores binárias de busca de forma recursiva.
 * São usadas para ordenar alimentos por energia/proteina e para buscar
 * intervalos.
 *****************************************************************************/

/**
 * AdicionarNosArvoreDecrescente - Percorre árvore em ordem DECRESCENTE
 *
 * @param raiz: Ponteiro para nó raiz da árvore (ou subárvore)
 * @param hwndListBox: Handle da listbox onde adicionar resultados
 *
 * FUNCIONAMENTO (Percurso In-Order Reverso):
 * 1. Visitar subárvore DIREITA (valores maiores) - recursão
 * 2. Processar nó atual (adicionar à listbox)
 * 3. Visitar subárvore ESQUERDA (valores menores) - recursão
 *
 * ESTRUTURA DA ÁRVORE:
 * NoArvore contém:
 * - chave: valor numérico (energia ou proteina)
 * - alimento: ponteiro para NoAlimento
 * - esquerda: ponteiro para filho esquerdo (valores menores)
 * - direita: ponteiro para filho direito (valores maiores)
 *
 * EXEMPLO:
 *         50
 *        /  \
 *      30    70
 *     /  \   /  \
 *   20  40 60  80
 *
 * Ordem decrescente: 80, 70, 60, 50, 40, 30, 20
 * Percurso: direita -> raiz -> esquerda
 */
void AdicionarNosArvoreDecrescente(NoArvore* raiz, HWND hwndListBox) {
    /* Caso base: se raiz é NULL, árvore vazia, retornar */
    if (raiz == NULL) return;

    /* PASSO 1: Visitar subárvore direita (valores maiores que raiz) */
    /* Chamada recursiva: processa toda subárvore direita antes de continuar */
    AdicionarNosArvoreDecrescente(raiz->direita, hwndListBox);

    /* PASSO 2: Processar nó atual (raiz desta subárvore) */
    char buffer[512];

    /* Formatar dados do alimento associado a este nó */
    sprintf(buffer, " %3d | %-50s | %4d kcal | %6.1f g",
            raiz->alimento->numero,
            raiz->alimento->descricao,
            raiz->alimento->energia_kcal,
            raiz->alimento->proteina);

    /* Adicionar à listbox */
    AdicionarItemListBox(hwndListBox, buffer);

    /* PASSO 3: Visitar subárvore esquerda (valores menores que raiz) */
    /* Chamada recursiva: processa toda subárvore esquerda */
    AdicionarNosArvoreDecrescente(raiz->esquerda, hwndListBox);
}

/**
 * AdicionarNosIntervalo - Busca e adiciona nós dentro de um intervalo [min, max]
 *
 * @param raiz: Ponteiro para nó raiz da árvore
 * @param hwndListBox: Handle da listbox
 * @param min: Valor mínimo do intervalo (inclusivo)
 * @param max: Valor máximo do intervalo (inclusivo)
 *
 * FUNCIONAMENTO (Busca Otimizada):
 * - Usa propriedade da BST para podar ramos desnecessários
 * - Se chave > min: pode haver valores válidos à esquerda
 * - Se min <= chave <= max: nó atual está no intervalo
 * - Se chave < max: pode haver valores válidos à direita
 *
 * OTIMIZAÇÃO:
 * Não precisa visitar todos os nós, apenas os que podem estar no intervalo.
 *
 * EXEMPLO: Buscar intervalo [30, 60] na árvore:
 *         50
 *        /  \
 *      30    70
 *     /  \   /  \
 *   20  40 60  80
 *
 * Resultado: 30, 40, 50, 60
 * Não visita: 20 (< min), 70 e 80 (> max)
 */
void AdicionarNosIntervalo(NoArvore* raiz, HWND hwndListBox, double min, double max) {
    /* Caso base: árvore vazia */
    if (raiz == NULL) return;

    /* OTIMIZAÇÃO: Se chave > min, pode haver valores válidos à esquerda */
    /* Exemplo: raiz=50, min=30 → pode ter 30, 40 à esquerda */
    if (raiz->chave > min) {
        AdicionarNosIntervalo(raiz->esquerda, hwndListBox, min, max);
    }

    /* Verificar se nó atual está dentro do intervalo */
    if (raiz->chave >= min && raiz->chave <= max) {
        /* Nó está no intervalo, adicionar à lista */
        char buffer[512];
        sprintf(buffer, " %3d | %-50s | %4d kcal | %6.1f g",
                raiz->alimento->numero,
                raiz->alimento->descricao,
                raiz->alimento->energia_kcal,
                raiz->alimento->proteina);

        AdicionarItemListBox(hwndListBox, buffer);
    }

    /* OTIMIZAÇÃO: Se chave < max, pode haver valores válidos à direita */
    /* Exemplo: raiz=50, max=60 → pode ter 60 à direita */
    if (raiz->chave < max) {
        AdicionarNosIntervalo(raiz->direita, hwndListBox, min, max);
    }
}

/******************************************************************************
 * HANDLER 3: ORDENAR POR ENERGIA (DECRESCENTE)
 *
 * Usa árvore binária de busca para ordenar alimentos por energia.
 * A árvore já foi construída ao carregar os dados.
 *****************************************************************************/

/**
 * OnListarPorEnergia - Handler do botão "3. Ordenar por Energia"
 *
 * @param app: Ponteiro para dados da aplicação
 *
 * FUNCIONAMENTO:
 * 1. Usuário seleciona categoria via diálogo
 * 2. Busca categoria
 * 3. Percorre árvore binária de energia em ordem decrescente
 * 4. Exibe resultados em tabela
 *
 * ESTRUTURA DE DADOS:
 * NoCategoria contém:
 * - arvore_energia: árvore binária indexada por energia (kcal)
 * Cada nó da árvore aponta para um alimento
 * Arvore está ordenada: esquerda < raiz < direita
 * Percurso in-order reverso resulta em ordem decrescente
 */
void OnListarPorEnergia(AppData* app) {
    char categoria[MAX_CATEGORIA];

    /* Diálogo de seleção de categoria */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;  // Cancelado
    }

    /* Buscar categoria */
    NoCategoria* cat = buscar_categoria(app->lista_categorias, categoria);

    if (cat == NULL) {
        MostrarMensagem(app->hwndMain, "Erro", "Categoria nao encontrada!");
        return;
    }

    /* Limpar listbox */
    LimparListBox(app->hwndListBox);

    char buffer[512];

    /* Cabeçalho */
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);
    AdicionarItemListBox(app->hwndListBox, "|  ORDENADO POR: ENERGIA (DECRESCENTE) - Usando Arvore Binaria          |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Cabeçalho da tabela */
    AdicionarCabecalhoTabela(app->hwndListBox);

    /* Percorrer árvore em ordem decrescente */
    /* A árvore arvore_energia foi construída ao carregar dados */
    /* Cada nó tem chave = energia_kcal */
    if (cat->arvore_energia != NULL) {
        AdicionarNosArvoreDecrescente(cat->arvore_energia, app->hwndListBox);
    }

    /* Rodapé */
    AdicionarSeparador(app->hwndListBox, 110);

    /* Atualizar status */
    SetWindowText(app->hwndStatus, "Alimentos ordenados por energia (maior -> menor).");
}

/******************************************************************************
 * HANDLER 4: ORDENAR POR PROTEÍNA (DECRESCENTE)
 *
 * Funciona identicamente ao Handler 3, mas usa árvore de proteina.
 *****************************************************************************/

/**
 * OnListarPorProteina - Handler do botão "4. Ordenar por Proteina"
 *
 * @param app: Ponteiro para dados da aplicação
 *
 * FUNCIONAMENTO:
 * Idêntico a OnListarPorEnergia, mas usa arvore_proteina ao invés de arvore_energia.
 *
 * ESTRUTURA DE DADOS:
 * NoCategoria->arvore_proteina: BST indexada por proteina (g)
 */
void OnListarPorProteina(AppData* app) {
    char categoria[MAX_CATEGORIA];

    /* Diálogo de seleção */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;
    }

    /* Buscar categoria */
    NoCategoria* cat = buscar_categoria(app->lista_categorias, categoria);

    if (cat == NULL) {
        MostrarMensagem(app->hwndMain, "Erro", "Categoria nao encontrada!");
        return;
    }

    /* Limpar listbox */
    LimparListBox(app->hwndListBox);

    char buffer[512];

    /* Cabeçalho */
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);
    AdicionarItemListBox(app->hwndListBox, "|  ORDENADO POR: PROTEINA (DECRESCENTE) - Usando Arvore Binaria         |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Cabeçalho da tabela */
    AdicionarCabecalhoTabela(app->hwndListBox);

    /* Percorrer árvore de proteina em ordem decrescente */
    if (cat->arvore_proteina != NULL) {
        AdicionarNosArvoreDecrescente(cat->arvore_proteina, app->hwndListBox);
    }

    /* Rodapé */
    AdicionarSeparador(app->hwndListBox, 110);

    /* Atualizar status */
    SetWindowText(app->hwndStatus, "Alimentos ordenados por proteina (maior -> menor).");
}

/******************************************************************************
 * HANDLER 5: FILTRAR POR INTERVALO DE ENERGIA
 *
 * Permite usuário especificar energia mínima e máxima, depois exibe apenas
 * alimentos nesse intervalo.
 *****************************************************************************/

/**
 * OnIntervaloEnergia - Handler do botão "5. Filtrar Energia"
 *
 * @param app: Ponteiro para dados da aplicação
 *
 * FUNCIONAMENTO:
 * 1. Diálogo para selecionar categoria
 * 2. Diálogo para digitar energia mínima
 * 3. Diálogo para digitar energia máxima
 * 4. Busca na árvore binária valores no intervalo [min, max]
 * 5. Exibe resultados
 *
 * VANTAGEM DA ÁRVORE BINÁRIA:
 * Busca otimizada - não precisa percorrer todos os nós, apenas os relevantes.
 * Complexidade: O(log n + k) onde k = número de resultados
 */
void OnIntervaloEnergia(AppData* app) {
    char categoria[MAX_CATEGORIA];
    double min = 0.0;  // Energia mínima (kcal)
    double max = 0.0;  // Energia máxima (kcal)

    /* Passo 1: Selecionar categoria */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;  // Cancelado
    }

    /* Passo 2: Perguntar energia mínima */
    /* MostrarDialogoNumero retorna FALSE se cancelado */
    if (!MostrarDialogoNumero(app->hwndMain, "Energia Minima",
            "Digite o valor minimo de energia (kcal):", &min)) {
        return;  // Cancelado
    }

    /* Passo 3: Perguntar energia máxima */
    if (!MostrarDialogoNumero(app->hwndMain, "Energia Maxima",
            "Digite o valor maximo de energia (kcal):", &max)) {
        return;  // Cancelado
    }

    /* Buscar categoria */
    NoCategoria* cat = buscar_categoria(app->lista_categorias, categoria);

    if (cat == NULL) {
        MostrarMensagem(app->hwndMain, "Erro", "Categoria nao encontrada!");
        return;
    }

    /* Limpar listbox */
    LimparListBox(app->hwndListBox);

    char buffer[512];

    /* Cabeçalho com informações do filtro */
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);

    /* Linha com intervalo de energia */
    /* sprintf formata, depois ajustamos manualmente para fechar a borda */
    sprintf(buffer, "|  FILTRO: Energia entre %.0f e %.0f kcal                                      ", min, max);
    buffer[73] = '|';  // Forçar '|' na posição 73 para fechar borda
    buffer[74] = '\0'; // Terminador null
    AdicionarItemListBox(app->hwndListBox, buffer);

    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Cabeçalho da tabela */
    AdicionarCabecalhoTabela(app->hwndListBox);

    /* Buscar na árvore valores no intervalo [min, max] */
    /* Função recursiva otimizada que poda ramos desnecessários */
    if (cat->arvore_energia != NULL) {
        AdicionarNosIntervalo(cat->arvore_energia, app->hwndListBox, min, max);
    }

    /* Rodapé */
    AdicionarSeparador(app->hwndListBox, 110);

    /* Atualizar status */
    sprintf(buffer, "Filtro aplicado: energia entre %.0f e %.0f kcal.", min, max);
    SetWindowText(app->hwndStatus, buffer);
}

/******************************************************************************
 * HANDLER 6: FILTRAR POR INTERVALO DE PROTEÍNA
 *
 * Funciona identicamente ao Handler 5, mas para proteina.
 *****************************************************************************/

/**
 * OnIntervaloProteina - Handler do botão "6. Filtrar Proteina"
 *
 * @param app: Ponteiro para dados da aplicação
 *
 * FUNCIONAMENTO:
 * Idêntico a OnIntervaloEnergia, mas usa arvore_proteina.
 */
void OnIntervaloProteina(AppData* app) {
    char categoria[MAX_CATEGORIA];
    double min = 0.0;  // Proteina mínima (g)
    double max = 0.0;  // Proteina máxima (g)

    /* Selecionar categoria */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;
    }

    /* Perguntar proteina mínima */
    if (!MostrarDialogoNumero(app->hwndMain, "Proteina Minima",
            "Digite o valor minimo de proteina (g):", &min)) {
        return;
    }

    /* Perguntar proteina máxima */
    if (!MostrarDialogoNumero(app->hwndMain, "Proteina Maxima",
            "Digite o valor maximo de proteina (g):", &max)) {
        return;
    }

    /* Buscar categoria */
    NoCategoria* cat = buscar_categoria(app->lista_categorias, categoria);

    if (cat == NULL) {
        MostrarMensagem(app->hwndMain, "Erro", "Categoria nao encontrada!");
        return;
    }

    /* Limpar listbox */
    LimparListBox(app->hwndListBox);

    char buffer[512];

    /* Cabeçalho */
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);

    /* Linha com intervalo de proteina */
    sprintf(buffer, "|  FILTRO: Proteina entre %.1f e %.1f g                                       ", min, max);
    buffer[73] = '|';
    buffer[74] = '\0';
    AdicionarItemListBox(app->hwndListBox, buffer);

    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Cabeçalho da tabela */
    AdicionarCabecalhoTabela(app->hwndListBox);

    /* Buscar na árvore de proteina valores no intervalo */
    if (cat->arvore_proteina != NULL) {
        AdicionarNosIntervalo(cat->arvore_proteina, app->hwndListBox, min, max);
    }

    /* Rodapé */
    AdicionarSeparador(app->hwndListBox, 110);

    /* Atualizar status */
    sprintf(buffer, "Filtro aplicado: proteina entre %.1f e %.1f g.", min, max);
    SetWindowText(app->hwndStatus, buffer);
}

/******************************************************************************
 * HANDLER 7: REMOVER CATEGORIA
 *
 * Remove uma categoria inteira (e todos os seus alimentos) do sistema.
 * Pede confirmação antes de remover.
 *****************************************************************************/

/**
 * OnRemoverCategoria - Handler do botão "7. Remover Categoria"
 *
 * @param app: Ponteiro para dados da aplicação
 *
 * FUNCIONAMENTO:
 * 1. Diálogo para selecionar categoria
 * 2. MessageBox para confirmar remoção
 * 3. Chama função remover_categoria (de categoria.c)
 * 4. Marca dados como modificados
 * 5. Exibe mensagem de confirmação
 *
 * IMPORTANTE:
 * - Remove categoria e TODOS os alimentos dela
 * - Libera memória das árvores binárias e listas encadeadas
 * - Marca dados_modificados = true para salvar ao sair
 */
void OnRemoverCategoria(AppData* app) {
    char categoria[MAX_CATEGORIA];

    /* Selecionar categoria a remover */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;  // Cancelado
    }

    /* Mensagem de confirmação */
    char mensagem[512];
    sprintf(mensagem, "Deseja realmente remover a categoria:\n\n%s\n\nTodos os alimentos desta categoria serao removidos!", categoria);

    /* MessageBox com botões Sim/Não e ícone de aviso */
    /* MB_YESNO = botões Sim e Não */
    /* MB_ICONWARNING = ícone de aviso (triângulo amarelo) */
    /* Retorna IDYES se usuário clicou Sim */
    if (MessageBox(app->hwndMain, mensagem, "Confirmar Remocao",
            MB_YESNO | MB_ICONWARNING) != IDYES) {
        return;  // Usuário clicou Não, cancelar operação
    }

    /* Remover categoria da lista */
    /* remover_categoria retorna nova cabeça da lista (caso primeira categoria seja removida) */
    app->lista_categorias = remover_categoria(app->lista_categorias, categoria);

    /* Marcar dados como modificados */
    /* Isso fará o sistema salvar automaticamente ao sair */
    app->dados_modificados = true;

    /* Exibir mensagem de sucesso */
    LimparListBox(app->hwndListBox);
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "|                    CATEGORIA REMOVIDA COM SUCESSO                      |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    sprintf(mensagem, "Categoria removida: %s", categoria);
    AdicionarItemListBox(app->hwndListBox, mensagem);
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "As alteracoes serao salvas ao sair do programa.");

    /* Atualizar status */
    sprintf(mensagem, "Categoria '%s' removida com sucesso.", categoria);
    SetWindowText(app->hwndStatus, mensagem);
}

/******************************************************************************
 * HANDLER 8: REMOVER ALIMENTO
 *
 * Remove um alimento específico de uma categoria.
 * Pede confirmação antes de remover.
 *****************************************************************************/

/**
 * OnRemoverAlimento - Handler do botão "8. Remover Alimento"
 *
 * @param app: Ponteiro para dados da aplicação
 *
 * FUNCIONAMENTO:
 * 1. Diálogo para selecionar categoria
 * 2. Diálogo para digitar número do alimento
 * 3. Busca nome do alimento para exibir na confirmação
 * 4. MessageBox para confirmar remoção
 * 5. Chama função remover_alimento_de_categoria
 * 6. Atualiza árvores binárias automaticamente
 * 7. Marca dados como modificados
 *
 * IMPORTANTE:
 * - Remove alimento da lista encadeada
 * - Remove alimento das árvores de energia e proteina
 * - Marca dados_modificados = true para salvar ao sair
 */
void OnRemoverAlimento(AppData* app) {
    char categoria[MAX_CATEGORIA];
    double numero_double = 0.0;  // Usamos double pois diálogo retorna double

    /* Selecionar categoria */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;
    }

    /* Perguntar número do alimento */
    if (!MostrarDialogoNumero(app->hwndMain, "Numero do Alimento",
            "Digite o numero do alimento a remover:", &numero_double)) {
        return;
    }

    /* Converter double para int */
    int numero = (int)numero_double;

    /* Buscar categoria */
    NoCategoria* cat = buscar_categoria(app->lista_categorias, categoria);

    if (cat == NULL) {
        MostrarMensagem(app->hwndMain, "Erro", "Categoria nao encontrada!");
        return;
    }

    /* Buscar nome do alimento para mostrar na confirmação */
    /* Percorre lista de alimentos procurando pelo número */
    NoAlimento* alim = cat->lista_alimentos;
    char nome_alimento[MAX_DESCRICAO] = "Alimento nao encontrado";  // Valor padrão se não achar

    while (alim != NULL) {
        if (alim->numero == numero) {
            /* Encontrou! Copiar nome */
            strcpy(nome_alimento, alim->descricao);
            break;  // Sair do loop
        }
        alim = alim->proximo;  // Próximo alimento
    }

    /* Mensagem de confirmação com dados do alimento */
    char mensagem[512];
    sprintf(mensagem, "Deseja realmente remover o alimento?\n\nNumero: %d\nNome: %s\nCategoria: %s",
            numero, nome_alimento, categoria);

    /* Pedir confirmação */
    if (MessageBox(app->hwndMain, mensagem, "Confirmar Remocao",
            MB_YESNO | MB_ICONWARNING) != IDYES) {
        return;  // Cancelado
    }

    /* Remover alimento */
    /* Função remove da lista e das árvores automaticamente */
    remover_alimento_de_categoria(cat, numero);

    /* Marcar dados como modificados */
    app->dados_modificados = true;

    /* Exibir mensagem de sucesso */
    LimparListBox(app->hwndListBox);
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "|                    ALIMENTO REMOVIDO COM SUCESSO                       |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    sprintf(mensagem, "Alimento removido: %d - %s", numero, nome_alimento);
    AdicionarItemListBox(app->hwndListBox, mensagem);

    sprintf(mensagem, "Categoria: %s", categoria);
    AdicionarItemListBox(app->hwndListBox, mensagem);

    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "As arvores binarias foram atualizadas automaticamente.");
    AdicionarItemListBox(app->hwndListBox, "As alteracoes serao salvas ao sair do programa.");

    /* Atualizar status */
    sprintf(mensagem, "Alimento %d removido com sucesso.", numero);
    SetWindowText(app->hwndStatus, mensagem);
}

/******************************************************************************
 * PROCEDIMENTO DE JANELA (WINDOW PROCEDURE)
 *
 * Esta é a função mais importante do sistema GUI.
 * O Windows chama esta função sempre que um evento ocorre (clique, tecla, etc).
 *
 * É o "cérebro" da interface: recebe eventos e chama os handlers apropriados.
 *****************************************************************************/

/**
 * WndProc - Procedimento de janela (Window Procedure)
 *
 * @param hwnd: Handle da janela que recebeu a mensagem
 * @param msg: Tipo de mensagem (WM_CREATE, WM_COMMAND, WM_CLOSE, etc)
 * @param wParam: Parâmetro 1 (depende da mensagem)
 * @param lParam: Parâmetro 2 (depende da mensagem)
 *
 * @return: Valor de retorno (depende da mensagem)
 *
 * MENSAGENS PRINCIPAIS:
 * - WM_CREATE: Janela sendo criada (criar controles)
 * - WM_COMMAND: Botão clicado ou menu selecionado
 * - WM_CLOSE: Usuário clicou no X
 * - WM_DESTROY: Janela sendo destruída (limpar recursos)
 *
 * FUNCIONAMENTO:
 * 1. Switch/case baseado no tipo de mensagem
 * 2. Processar mensagem apropriadamente
 * 3. Retornar valor adequado
 * 4. Se não processar, chamar DefWindowProc (processamento padrão)
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        /*** WM_CREATE: Janela sendo criada ***/
        case WM_CREATE: {
            /* Esta mensagem é enviada quando CreateWindow é chamado */
            /* Momento ideal para criar todos os controles filhos */
            CriarControles(hwnd);
            break;
        }

        /*** WM_COMMAND: Comando (botão clicado, menu, etc) ***/
        case WM_COMMAND: {
            /* Esta mensagem é enviada quando:
             * - Usuário clica em botão
             * - Seleciona item de menu
             * - Tecla de atalho é pressionada
             *
             * LOWORD(wParam) = ID do controle que gerou o evento
             * HIWORD(wParam) = Código de notificação (BN_CLICKED para botões)
             */

            /* Switch baseado no ID do botão clicado */
            switch (LOWORD(wParam)) {
                case ID_BTN_LISTAR_CATEGORIAS:
                    /* Botão 1 clicado */
                    OnListarCategorias(g_appData);
                    break;

                case ID_BTN_LISTAR_ALIMENTOS:
                    /* Botão 2 clicado */
                    OnListarAlimentos(g_appData);
                    break;

                case ID_BTN_ENERGIA_DESC:
                    /* Botão 3 clicado */
                    OnListarPorEnergia(g_appData);
                    break;

                case ID_BTN_PROTEINA_DESC:
                    /* Botão 4 clicado */
                    OnListarPorProteina(g_appData);
                    break;

                case ID_BTN_INTERVALO_ENERGIA:
                    /* Botão 5 clicado */
                    OnIntervaloEnergia(g_appData);
                    break;

                case ID_BTN_INTERVALO_PROTEINA:
                    /* Botão 6 clicado */
                    OnIntervaloProteina(g_appData);
                    break;

                case ID_BTN_REMOVER_CATEGORIA:
                    /* Botão 7 clicado */
                    OnRemoverCategoria(g_appData);
                    break;

                case ID_BTN_REMOVER_ALIMENTO:
                    /* Botão 8 clicado */
                    OnRemoverAlimento(g_appData);
                    break;

                case ID_BTN_SAIR:
                    /* Botão 9 (SAIR) clicado */
                    /* Pedir confirmação antes de sair */
                    if (MessageBox(hwnd, "Deseja realmente sair do sistema?",
                        "Confirmar Saida", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        /* Usuário confirmou - enviar mensagem para encerrar aplicação */
                        /* PostQuitMessage faz GetMessage retornar 0, encerrando loop */
                        PostQuitMessage(0);
                    }
                    break;
            }
            break;
        }

        /*** WM_CLOSE: Usuário clicou no X da janela ***/
        case WM_CLOSE: {
            /* Pedir confirmação antes de fechar */
            if (MessageBox(hwnd, "Deseja realmente fechar o programa?",
                "Confirmar Fechamento", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                /* Usuário confirmou - destruir janela */
                /* Isso enviará WM_DESTROY */
                DestroyWindow(hwnd);
            }
            /* Se usuário cancelou, não fazer nada (janela permanece aberta) */
            break;
        }

        /*** WM_DESTROY: Janela sendo destruída ***/
        case WM_DESTROY: {
            /* Esta mensagem é enviada quando DestroyWindow é chamado */
            /* Momento para liberar recursos alocados */

            /* Deletar fontes se foram criadas */
            if (g_hFontMono) DeleteObject(g_hFontMono);
            if (g_hFontNormal) DeleteObject(g_hFontNormal);

            /* Enviar mensagem de quit para encerrar aplicação */
            PostQuitMessage(0);
            break;
        }

        /*** Mensagem não processada ***/
        default: {
            /* Para qualquer mensagem que não processamos, */
            /* chamar processamento padrão do Windows */
            /* DefWindowProc implementa comportamento padrão */
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

    /* Retornar 0 indica que processamos a mensagem */
    return 0;
}

/******************************************************************************
 * INICIALIZAÇÃO DA GUI
 *
 * Registra classe de janela e cria a janela principal.
 *****************************************************************************/

/**
 * InicializarGUI - Inicializa a interface gráfica
 *
 * @param hInstance: Handle da instância da aplicação (passado em WinMain)
 *
 * FUNCIONAMENTO:
 * 1. Registra "classe de janela" (define propriedades de janelas deste tipo)
 * 2. Cria janela principal
 * 3. Exibe janela
 *
 * CLASSE DE JANELA:
 * Define propriedades compartilhadas:
 * - Procedimento de janela (WndProc)
 * - Ícone
 * - Cursor
 * - Cor de fundo
 * - etc.
 */
void InicializarGUI(HINSTANCE hInstance) {
    /* Estrutura que define a classe de janela */
    WNDCLASSEX wc = {0};  // Inicializar com zeros

    /* Preencher estrutura com propriedades da classe */
    wc.cbSize = sizeof(WNDCLASSEX);        // Tamanho da estrutura
    wc.style = 0;                          // Estilos da classe (0 = padrão)
    wc.lpfnWndProc = WndProc;              // Ponteiro para procedimento de janela
    wc.cbClsExtra = 0;                     // Bytes extras por classe (não usado)
    wc.cbWndExtra = 0;                     // Bytes extras por janela (não usado)
    wc.hInstance = hInstance;              // Handle da instância
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);     // Ícone grande (padrão)
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Cursor (seta padrão)
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // Cor de fundo (branco)
    wc.lpszMenuName = NULL;                // Nome do menu (não temos menu)
    wc.lpszClassName = "SistemaAlimentosClass";     // Nome único da classe
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);   // Ícone pequeno (padrão)

    /* Registrar classe no Windows */
    /* Se falhar, exibir erro e retornar */
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Falha ao registrar classe da janela!", "Erro",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    /* Criar janela principal */
    g_appData->hwndMain = CreateWindowEx(
        0,                                  // Estilos estendidos (nenhum)
        "SistemaAlimentosClass",            // Nome da classe (registrada acima)
        "Sistema de Gerenciamento de Alimentos - Trabalho de Programacao Imperativa RA2",  // Título
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,  // Estilos da janela
        /* WS_OVERLAPPED = janela sobreposta (normal)
         * WS_CAPTION = tem barra de título
         * WS_SYSMENU = tem menu do sistema (ícone no canto)
         * WS_MINIMIZEBOX = tem botão minimizar
         * NÃO tem WS_MAXIMIZEBOX (sem botão maximizar - janela com tamanho fixo)
         * NÃO tem WS_THICKFRAME (sem borda redimensionável)
         */
        CW_USEDEFAULT, CW_USEDEFAULT,       // Posição X, Y (Windows escolhe)
        1030, 710,                          // Largura, Altura (tamanho fixo)
        NULL,                               // Janela pai (nenhuma - é janela principal)
        NULL,                               // Menu (nenhum)
        hInstance,                          // Handle da instância
        NULL                                // Parâmetros de criação (nenhum)
    );

    /* Verificar se janela foi criada com sucesso */
    if (g_appData->hwndMain == NULL) {
        MessageBox(NULL, "Falha ao criar janela!", "Erro",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    /* Exibir janela */
    /* SW_SHOW = mostrar janela no estado normal */
    ShowWindow(g_appData->hwndMain, SW_SHOW);

    /* Forçar redesenho da janela */
    /* Isso garante que a janela seja desenhada corretamente */
    UpdateWindow(g_appData->hwndMain);
}

/******************************************************************************
 * FUNÇÕES DE GERENCIAMENTO DE DADOS GLOBAIS
 *
 * Estas funções permitem compartilhar AppData entre módulos.
 *****************************************************************************/

/**
 * SetAppData - Define ponteiro global para AppData
 *
 * @param app: Ponteiro para estrutura AppData
 *
 * Permite que P2_GUI.c passe dados para gui.c
 */
void SetAppData(AppData* app) {
    g_appData = app;
}

/**
 * GetAppData - Obtém ponteiro global para AppData
 *
 * @return: Ponteiro para AppData
 *
 * Permite outros módulos acessarem dados se necessário
 */
AppData* GetAppData() {
    return g_appData;
}
