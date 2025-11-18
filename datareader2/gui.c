/******************************************************************************
 * GUI.C - Interface Grafica do Sistema de Gerenciamento de Alimentos
 *
 * Este arquivo implementa toda a interface grafica do sistema usando Win32 API.
 * Contem as funcoes de criar controles, processar eventos de botoes e
 * exibir dados na listbox.
 *
 * ESTRUTURA:
 * 1. Funcoes auxiliares (limpar listbox, adicionar itens, criar tabelas)
 * 2. Criacao de controles (botoes, listbox, labels)
 * 3. Handlers de cada uma das 9 funcionalidades
 * 4. Procedimento de janela (processa todos os eventos)
 * 5. Inicializacao da GUI
 *
 * FUNCIONALIDADES IMPLEMENTADAS:
 * 1. Listar categorias alfabeticamente
 * 2. Listar alimentos de uma categoria
 * 3. Ordenar alimentos por energia (decrescente)
 * 4. Ordenar alimentos por proteina (decrescente)
 * 5. Filtrar alimentos por intervalo de energia
 * 6. Filtrar alimentos por intervalo de proteina
 * 7. Remover categoria completa
 * 8. Remover alimento especifico
 * 9. Sair do sistema
 *****************************************************************************/

#include "gui.h"           // Definicoes de IDs, estruturas, prototipos
#include "categoria.h"     // Funcoes de manipulacao de categorias
#include "arvore.h"        // Funcoes de arvore binaria
#include "dialogo.h"       // Sistema de dialogos personalizados
#include <stdio.h>         // sprintf, printf
#include <stdlib.h>        // malloc, free
#include <string.h>        // strcpy, strcmp
#include <commctrl.h>      // Controles comuns do Windows

/******************************************************************************
 * VARIAVEIS GLOBAIS PRIVADAS
 *
 * Estas variaveis sao acessiveis apenas dentro deste arquivo (static).
 * Armazenam dados compartilhados entre funcoes.
 *****************************************************************************/

/* Ponteiro global para os dados da aplicacao (lista de categorias, handles de janelas) */
static AppData* g_appData = NULL;

/* Fontes (tipos de letra) usadas na interface */
static HFONT g_hFontMono = NULL;    // Fonte monoespacada (Consolas) para tabelas
static HFONT g_hFontNormal = NULL;  // Fonte normal (Segoe UI) para titulos

/******************************************************************************
 * FUNCOES AUXILIARES PARA MANIPULACAO DA LISTBOX
 *
 * Estas funcoes encapsulam operacoes comuns na listbox (caixa de listagem)
 * onde os resultados sao exibidos.
 *****************************************************************************/

/**
 * LimparListBox - Remove todos os itens da listbox
 *
 * @param hwndListBox: Handle (identificador) da listbox a limpar
 *
 * FUNCIONAMENTO:
 * - SendMessage envia uma mensagem para um controle Windows
 * - LB_RESETCONTENT e a mensagem que limpa todos os itens de uma listbox
 * - Retorna 0 se bem-sucedido
 */
void LimparListBox(HWND hwndListBox) {
    SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);
}

/**
 * AdicionarItemListBox - Adiciona uma linha de texto a listbox
 *
 * @param hwndListBox: Handle da listbox
 * @param texto: String a ser adicionada
 *
 * FUNCIONAMENTO:
 * - LB_ADDSTRING adiciona um item ao final da lista
 * - (LPARAM) faz cast do ponteiro de string para o tipo esperado
 * - O item e adicionado na proxima posicao disponivel
 */
void AdicionarItemListBox(HWND hwndListBox, const char* texto) {
    SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)texto);
}

/**
 * MostrarMensagem - Exibe uma caixa de dialogo simples com mensagem
 *
 * @param hwnd: Janela pai (a mensagem aparece sobre ela)
 * @param titulo: Titulo da janela de mensagem
 * @param mensagem: Texto da mensagem
 *
 * FUNCIONAMENTO:
 * - MessageBox e funcao padrao do Windows para mostrar mensagens
 * - MB_OK: apenas botao OK
 * - MB_ICONINFORMATION: icone de informacao (i azul)
 */
void MostrarMensagem(HWND hwnd, const char* titulo, const char* mensagem) {
    MessageBox(hwnd, mensagem, titulo, MB_OK | MB_ICONINFORMATION);
}

/**
 * AdicionarSeparador - Adiciona uma linha separadora feita de tracos
 *
 * @param hwndListBox: Handle da listbox
 * @param tamanho: Numero de tracos (comprimento da linha)
 *
 * FUNCIONAMENTO:
 * 1. Cria um array de caracteres
 * 2. Preenche com '-' ate o tamanho especificado
 * 3. Adiciona '\0' (terminador de string) no final
 * 4. Adiciona a listbox
 */
void AdicionarSeparador(HWND hwndListBox, int tamanho) {
    char separador[256];  // Buffer para armazenar a linha de tracos
    int i;

    /* Loop para preencher buffer com tracos */
    for (i = 0; i < tamanho && i < 255; i++) {
        separador[i] = '-';  // Cada posicao recebe um traco
    }

    /* Adicionar terminador nulo (obrigatorio em strings C) */
    separador[i] = '\0';

    /* Adicionar a listbox */
    AdicionarItemListBox(hwndListBox, separador);
}

/**
 * AdicionarCabecalhoTabela - Adiciona o cabecalho padrao das tabelas de alimentos
 *
 * @param hwndListBox: Handle da listbox
 *
 * FUNCIONAMENTO:
 * Cria uma tabela bonita com:
 * - Linha separadora superior
 * - Cabecalhos das colunas (Num, Descricao, Energia, Proteina)
 * - Linha separadora inferior
 *
 * FORMATO:
 * ------------------------------------------------------
 *  Num | Descricao                      | Energia  | Proteina
 * ------------------------------------------------------
 */
void AdicionarCabecalhoTabela(HWND hwndListBox) {
    /* Linha separadora superior (110 tracos) */
    AdicionarSeparador(hwndListBox, 110);

    /* Cabecalhos das colunas, formatados para alinhar */
    /* Espacos sao cuidadosamente calculados para alinhamento com fonte monoespacada */
    AdicionarItemListBox(hwndListBox, " Num | Descricao                                       | Energia  | Proteina ");

    /* Linha separadora inferior */
    AdicionarSeparador(hwndListBox, 110);
}

/******************************************************************************
 * CRIACAO DOS CONTROLES DA INTERFACE
 *
 * Esta funcao cria todos os elementos visuais da janela:
 * - Titulos
 * - Botoes das 9 funcionalidades
 * - Listbox para exibir resultados
 * - Barra de status
 *****************************************************************************/

/**
 * CriarControles - Cria todos os controles (botoes, textos, listbox) da interface
 *
 * @param hwnd: Handle da janela principal onde os controles serao criados
 *
 * FUNCIONAMENTO:
 * 1. Cria fontes personalizadas (Segoe UI para titulos, Consolas para tabelas)
 * 2. Cria labels de titulo e subtitulo
 * 3. Cria 9 botoes (2 linhas de botoes)
 * 4. Cria grupo de resultados com listbox
 * 5. Cria barra de status
 *
 * LAYOUT DA INTERFACE:
 * +----------------------------------------------------------+
 * |       SISTEMA DE GERENCIAMENTO DE ALIMENTOS             |
 * |   Trabalho de Programacao Imperativa - RA2              |
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
    /*** CRIACAO DE FONTES PERSONALIZADAS ***/

    /* Fonte normal (Segoe UI, negrito, 18px) para titulos */
    /* CreateFont cria uma fonte com parametros especificos */
    g_hFontNormal = CreateFont(
        18,                        // Altura da fonte em pixels
        0,                         // Largura (0 = automatico baseado na altura)
        0,                         // Angulo de escape (rotacao)
        0,                         // Angulo de orientacao
        FW_BOLD,                   // Peso da fonte (FW_BOLD = negrito)
        FALSE,                     // Italico? (FALSE = nao)
        FALSE,                     // Sublinhado? (FALSE = nao)
        FALSE,                     // Riscado? (FALSE = nao)
        DEFAULT_CHARSET,           // Conjunto de caracteres
        OUT_DEFAULT_PRECIS,        // Precisao de saida
        CLIP_DEFAULT_PRECIS,       // Precisao de recorte
        DEFAULT_QUALITY,           // Qualidade
        DEFAULT_PITCH | FF_DONTCARE, // Pitch e familia
        "Segoe UI"                 // Nome da fonte
    );

    /* Fonte monoespacada (Consolas, 16px) para tabelas */
    /* Fonte monoespacada = cada caractere tem mesma largura, perfeito para alinhar colunas */
    g_hFontMono = CreateFont(
        16,                        // Altura 16px
        0,                         // Largura automatica
        0,                         // Sem rotacao
        0,                         // Sem inclinacao
        FW_NORMAL,                 // Peso normal (nao negrito)
        FALSE, FALSE, FALSE,       // Sem italico, sublinhado ou riscado
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN,   // FIXED_PITCH = monoespacada, FF_MODERN = familia moderna
        "Consolas"                 // Nome da fonte
    );

    /*** CRIACAO DE CONTROLES DE INTERFACE ***/

    /* TITULO PRINCIPAL */
    /* CreateWindow cria um controle filho dentro da janela principal */
    HWND hwndTitulo = CreateWindow(
        "STATIC",                  // Classe: texto estatico (nao editavel)
        "SISTEMA DE GERENCIAMENTO DE ALIMENTOS",  // Texto a exibir
        WS_VISIBLE | WS_CHILD | SS_CENTER,  // Estilos: visivel, filho, centralizado
        10, 10,                    // Posicao X, Y (pixels do canto superior esquerdo)
        980, 35,                   // Largura, Altura
        hwnd,                      // Janela pai
        NULL,                      // ID do menu (nao usado para STATIC)
        NULL,                      // Handle da instancia (NULL = usar padrao)
        NULL                       // Parametros adicionais
    );
    /* Aplicar fonte ao titulo */
    /* WM_SETFONT = mensagem para definir fonte de um controle */
    SendMessage(hwndTitulo, WM_SETFONT, (WPARAM)g_hFontNormal, TRUE);

    /* SUBTITULO */
    CreateWindow(
        "STATIC",
        "Trabalho de Programacao Imperativa - RA2",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        10, 45, 980, 20,
        hwnd, NULL, NULL, NULL
    );

    /*** PRIMEIRA LINHA DE BOTOES (4 botoes) ***/

    /* BOTAO 1: Listar Categorias */
    CreateWindow(
        "BUTTON",                  // Classe: botao
        "1. Listar Categorias",    // Texto do botao
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Estilos: visivel, filho, push button
        10, 75,                    // Posicao X=10, Y=75
        240, 45,                   // Largura=240, Altura=45
        hwnd,                      // Janela pai
        (HMENU)ID_BTN_LISTAR_CATEGORIAS,  // ID do botao (usado para identificar cliques)
        NULL, NULL
    );

    /* BOTAO 2: Listar Alimentos */
    CreateWindow(
        "BUTTON",
        "2. Listar Alimentos",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        260, 75,                   // X=260 (espacado 250 + 10 de margem do anterior)
        240, 45,
        hwnd,
        (HMENU)ID_BTN_LISTAR_ALIMENTOS,
        NULL, NULL
    );

    /* BOTAO 3: Ordenar por Energia */
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

    /* BOTAO 4: Ordenar por Proteina */
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

    /*** SEGUNDA LINHA DE BOTOES (5 botoes) ***/

    /* BOTAO 5: Filtrar por Energia */
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

    /* BOTAO 6: Filtrar por Proteina */
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

    /* BOTAO 7: Remover Categoria */
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

    /* BOTAO 8: Remover Alimento */
    CreateWindow(
        "BUTTON",
        "8. Remover Alimento",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        760, 130,
        150, 45,                   // Largura menor (150) para dar espaco ao botao Sair
        hwnd,
        (HMENU)ID_BTN_REMOVER_ALIMENTO,
        NULL, NULL
    );

    /* BOTAO 9: SAIR */
    CreateWindow(
        "BUTTON",
        "9. SAIR",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        920, 130,                  // X=920 (mais a direita)
        80, 45,                    // Largura menor (80)
        hwnd,
        (HMENU)ID_BTN_SAIR,
        NULL, NULL
    );

    /*** GRUPO DE RESULTADOS ***/

    /* GroupBox (moldura visual com titulo "Resultados") */
    CreateWindow(
        "BUTTON",                  // BUTTON com BS_GROUPBOX cria uma moldura
        "Resultados",              // Texto da moldura
        WS_VISIBLE | WS_CHILD | BS_GROUPBOX,  // BS_GROUPBOX = estilo de grupo
        10, 185,                   // Posicao Y=185 (abaixo dos botoes)
        990, 430,                  // Largura=990, Altura=430 (caixa grande)
        hwnd, NULL, NULL, NULL
    );

    /* LISTBOX - Caixa de listagem para exibir resultados */
    /* Esta e a area onde todas as tabelas e listas serao exibidas */
    g_appData->hwndListBox = CreateWindowEx(
        WS_EX_CLIENTEDGE,          // Estilo estendido: borda 3D
        "LISTBOX",                 // Classe: caixa de listagem
        NULL,                      // Sem texto inicial
        WS_CHILD | WS_VISIBLE |    // Estilos: filho, visivel
        WS_VSCROLL |               // Com barra de rolagem vertical
        WS_HSCROLL |               // Com barra de rolagem horizontal
        LBS_NOINTEGRALHEIGHT |     // Permite altura nao-inteira de itens
        LBS_NOTIFY,                // Notifica pai quando selecao muda
        20, 205,                   // Posicao X=20, Y=205 (dentro do grupo)
        970, 395,                  // Largura=970, Altura=395
        hwnd,                      // Janela pai
        (HMENU)ID_LISTBOX_RESULTADO,  // ID da listbox
        NULL, NULL
    );

    /* Aplicar fonte monoespacada a listbox */
    /* Isso garante que tabelas fiquem perfeitamente alinhadas */
    SendMessage(g_appData->hwndListBox, WM_SETFONT, (WPARAM)g_hFontMono, TRUE);

    /*** BARRA DE STATUS ***/

    /* Texto estatico no rodape mostrando status do sistema */
    g_appData->hwndStatus = CreateWindowEx(
        WS_EX_STATICEDGE,          // Borda estatica (leve relevo)
        "STATIC",
        "Pronto. Sistema carregado com 597 alimentos em 15 categorias.",
        WS_CHILD | WS_VISIBLE | SS_LEFT,  // Filho, visivel, alinhado a esquerda
        10, 625,                   // Posicao Y=625 (rodape)
        990, 25,                   // Altura=25
        hwnd,
        (HMENU)ID_STATUSBAR,
        NULL, NULL
    );
}

/******************************************************************************
 * HANDLER 1: LISTAR CATEGORIAS
 *
 * Exibe todas as categorias em ordem alfabetica.
 * Usa a lista encadeada ordenada de categorias.
 *****************************************************************************/

/**
 * OnListarCategorias - Handler do botao "1. Listar Categorias"
 *
 * @param app: Ponteiro para estrutura com dados da aplicacao
 *
 * FUNCIONAMENTO:
 * 1. Limpa a listbox
 * 2. Verifica se ha categorias
 * 3. Cria cabecalho visual bonito
 * 4. Percorre lista encadeada de categorias
 * 5. Adiciona cada categoria numerada
 * 6. Mostra total de categorias
 * 7. Atualiza barra de status
 *
 * ESTRUTURA DE DADOS:
 * app->lista_categorias e uma lista encadeada ordenada alfabeticamente
 * Cada NoCategoria tem:
 * - nome: string com nome da categoria
 * - proximo: ponteiro para proxima categoria (NULL no fim)
 */
void OnListarCategorias(AppData* app) {
    /* Limpar listbox antes de adicionar novo conteudo */
    LimparListBox(app->hwndListBox);

    /* Verificar se lista esta vazia */
    if (app->lista_categorias == NULL) {
        AdicionarItemListBox(app->hwndListBox, "Nenhuma categoria encontrada.");
        return;  // Sair da funcao se nao ha dados
    }

    /* Adicionar cabecalho visual bonito com bordas ASCII */
    AdicionarItemListBox(app->hwndListBox, "");  // Linha em branco para espacamento
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "|              CATEGORIAS DE ALIMENTOS - ORDEM ALFABETICA                |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Variaveis para percorrer lista */
    int contador = 1;                         // Contador para numerar categorias (inicia em 1)
    NoCategoria* atual = app->lista_categorias;  // Ponteiro para navegar pela lista
    char buffer[512];                         // Buffer temporario para formatar strings

    /* Loop para percorrer toda a lista encadeada */
    /* Continua enquanto 'atual' nao for NULL (fim da lista) */
    while (atual != NULL) {
        /* Formatar string: "  1.  Alimentos preparados" */
        /* %2d = inteiro com 2 digitos (alinhado a direita) */
        /* %s = string */
        sprintf(buffer, "  %2d.  %s", contador, atual->nome);

        /* Adicionar a listbox */
        AdicionarItemListBox(app->hwndListBox, buffer);

        /* Incrementar contador para proxima categoria */
        contador++;

        /* Avancar para proximo no da lista */
        /* atual->proximo aponta para o proximo NoCategoria */
        /* Se for o ultimo, proximo = NULL e loop termina */
        atual = atual->proximo;
    }

    /* Adicionar rodape com total */
    AdicionarItemListBox(app->hwndListBox, "");
    /* contador - 1 porque incrementamos uma vez a mais no loop */
    sprintf(buffer, "Total: %d categorias", contador - 1);
    AdicionarItemListBox(app->hwndListBox, buffer);

    /* Atualizar barra de status no rodape da janela */
    SetWindowText(app->hwndStatus, "Categorias listadas com sucesso.");
}

/******************************************************************************
 * HANDLER 2: LISTAR ALIMENTOS DE UMA CATEGORIA
 *
 * Mostra dialogo para selecionar categoria, depois lista todos os alimentos
 * dessa categoria em ordem alfabetica.
 *****************************************************************************/

/**
 * OnListarAlimentos - Handler do botao "2. Listar Alimentos"
 *
 * @param app: Ponteiro para dados da aplicacao
 *
 * FUNCIONAMENTO:
 * 1. Abre dialogo para usuario selecionar categoria
 * 2. Busca a categoria selecionada na lista
 * 3. Percorre lista de alimentos da categoria
 * 4. Exibe em formato de tabela bonita
 *
 * ESTRUTURA DE DADOS:
 * NoCategoria contem:
 * - lista_alimentos: lista encadeada de NoAlimento
 * Cada NoAlimento contem:
 * - numero: numero de identificacao
 * - descricao: nome do alimento
 * - energia_kcal: energia em kcal
 * - proteina: proteina em gramas
 * - proximo: proximo alimento na lista
 */
void OnListarAlimentos(AppData* app) {
    char categoria[MAX_CATEGORIA];  // Buffer para armazenar nome da categoria selecionada

    /* Mostrar dialogo de selecao de categoria */
    /* Retorna FALSE se usuario cancelou */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;  // Usuario cancelou, sair da funcao
    }

    /* Buscar categoria na lista usando nome */
    /* buscar_categoria retorna ponteiro para NoCategoria ou NULL se nao encontrou */
    NoCategoria* cat = buscar_categoria(app->lista_categorias, categoria);

    /* Limpar listbox para novo conteudo */
    LimparListBox(app->hwndListBox);

    /* Verificar se categoria foi encontrada */
    if (cat == NULL) {
        AdicionarItemListBox(app->hwndListBox, "Categoria nao encontrada!");
        return;
    }

    /* Buffer para formatar strings */
    char buffer[512];

    /* Adicionar cabecalho com nome da categoria */
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");

    /* Cabecalho com nome da categoria centralizado */
    /* %-70s = string alinhada a esquerda com 70 caracteres */
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);

    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Adicionar cabecalho da tabela de alimentos */
    AdicionarCabecalhoTabela(app->hwndListBox);

    /* Percorrer lista de alimentos da categoria */
    int total = 0;                          // Contador de alimentos
    NoAlimento* alim = cat->lista_alimentos;  // Ponteiro para navegar lista de alimentos

    while (alim != NULL) {
        /* Formatar linha da tabela com dados do alimento */
        /* %3d = numero com 3 digitos, alinhado a direita */
        /* %-50s = descricao com 50 caracteres, alinhada a esquerda */
        /* %4d = energia com 4 digitos */
        /* %6.1f = proteina com 6 caracteres total, 1 casa decimal */
        sprintf(buffer, " %3d | %-50s | %4d kcal | %6.1f g",
                alim->numero,
                alim->descricao,
                alim->energia_kcal,
                alim->proteina);

        AdicionarItemListBox(app->hwndListBox, buffer);

        /* Avancar para proximo alimento */
        alim = alim->proximo;
        total++;  // Incrementar contador
    }

    /* Adicionar rodape */
    AdicionarSeparador(app->hwndListBox, 110);
    sprintf(buffer, "Total: %d alimentos", total);
    AdicionarItemListBox(app->hwndListBox, buffer);

    /* Atualizar status */
    sprintf(buffer, "Alimentos da categoria '%s' listados (ordem alfabetica).", cat->nome);
    SetWindowText(app->hwndStatus, buffer);
}

/******************************************************************************
 * FUNCOES AUXILIARES RECURSIVAS PARA ARVORES BINARIAS
 *
 * Estas funcoes percorrem arvores binarias de busca de forma recursiva.
 * Sao usadas para ordenar alimentos por energia/proteina e para buscar
 * intervalos.
 *****************************************************************************/

/**
 * AdicionarNosArvoreDecrescente - Percorre arvore em ordem DECRESCENTE
 *
 * @param raiz: Ponteiro para no raiz da arvore (ou subarvore)
 * @param hwndListBox: Handle da listbox onde adicionar resultados
 *
 * FUNCIONAMENTO (Percurso In-Order Reverso):
 * 1. Visitar subarvore DIREITA (valores maiores) - recursao
 * 2. Processar no atual (adicionar a listbox)
 * 3. Visitar subarvore ESQUERDA (valores menores) - recursao
 *
 * ESTRUTURA DA ARVORE:
 * NoArvore contem:
 * - chave: valor numerico (energia ou proteina)
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
    /* Caso base: se raiz e NULL, arvore vazia, retornar */
    if (raiz == NULL) return;

    /* PASSO 1: Visitar subarvore direita (valores maiores que raiz) */
    /* Chamada recursiva: processa toda subarvore direita antes de continuar */
    AdicionarNosArvoreDecrescente(raiz->direita, hwndListBox);

    /* PASSO 2: Processar no atual (raiz desta subarvore) */
    char buffer[512];

    /* Formatar dados do alimento associado a este no */
    sprintf(buffer, " %3d | %-50s | %4d kcal | %6.1f g",
            raiz->alimento->numero,
            raiz->alimento->descricao,
            raiz->alimento->energia_kcal,
            raiz->alimento->proteina);

    /* Adicionar a listbox */
    AdicionarItemListBox(hwndListBox, buffer);

    /* PASSO 3: Visitar subarvore esquerda (valores menores que raiz) */
    /* Chamada recursiva: processa toda subarvore esquerda */
    AdicionarNosArvoreDecrescente(raiz->esquerda, hwndListBox);
}

/**
 * AdicionarNosIntervalo - Busca e adiciona nos dentro de um intervalo [min, max]
 *
 * @param raiz: Ponteiro para no raiz da arvore
 * @param hwndListBox: Handle da listbox
 * @param min: Valor minimo do intervalo (inclusivo)
 * @param max: Valor maximo do intervalo (inclusivo)
 *
 * FUNCIONAMENTO (Busca Otimizada):
 * - Usa propriedade da BST para podar ramos desnecessarios
 * - Se chave > min: pode haver valores validos a esquerda
 * - Se min <= chave <= max: no atual esta no intervalo
 * - Se chave < max: pode haver valores validos a direita
 *
 * OTIMIZACAO:
 * Nao precisa visitar todos os nos, apenas os que podem estar no intervalo.
 *
 * EXEMPLO: Buscar intervalo [30, 60] na arvore:
 *         50
 *        /  \
 *      30    70
 *     /  \   /  \
 *   20  40 60  80
 *
 * Resultado: 30, 40, 50, 60
 * Nao visita: 20 (< min), 70 e 80 (> max)
 */
void AdicionarNosIntervalo(NoArvore* raiz, HWND hwndListBox, double min, double max) {
    /* Caso base: arvore vazia */
    if (raiz == NULL) return;

    /* OTIMIZACAO: Se chave > min, pode haver valores validos a esquerda */
    /* Exemplo: raiz=50, min=30 → pode ter 30, 40 a esquerda */
    if (raiz->chave > min) {
        AdicionarNosIntervalo(raiz->esquerda, hwndListBox, min, max);
    }

    /* Verificar se no atual esta dentro do intervalo */
    if (raiz->chave >= min && raiz->chave <= max) {
        /* No esta no intervalo, adicionar a lista */
        char buffer[512];
        sprintf(buffer, " %3d | %-50s | %4d kcal | %6.1f g",
                raiz->alimento->numero,
                raiz->alimento->descricao,
                raiz->alimento->energia_kcal,
                raiz->alimento->proteina);

        AdicionarItemListBox(hwndListBox, buffer);
    }

    /* OTIMIZACAO: Se chave < max, pode haver valores validos a direita */
    /* Exemplo: raiz=50, max=60 → pode ter 60 a direita */
    if (raiz->chave < max) {
        AdicionarNosIntervalo(raiz->direita, hwndListBox, min, max);
    }
}

/******************************************************************************
 * HANDLER 3: ORDENAR POR ENERGIA (DECRESCENTE)
 *
 * Usa arvore binaria de busca para ordenar alimentos por energia.
 * A arvore ja foi construida ao carregar os dados.
 *****************************************************************************/

/**
 * OnListarPorEnergia - Handler do botao "3. Ordenar por Energia"
 *
 * @param app: Ponteiro para dados da aplicacao
 *
 * FUNCIONAMENTO:
 * 1. Usuario seleciona categoria via dialogo
 * 2. Busca categoria
 * 3. Percorre arvore binaria de energia em ordem decrescente
 * 4. Exibe resultados em tabela
 *
 * ESTRUTURA DE DADOS:
 * NoCategoria contem:
 * - arvore_energia: arvore binaria indexada por energia (kcal)
 * Cada no da arvore aponta para um alimento
 * Arvore esta ordenada: esquerda < raiz < direita
 * Percurso in-order reverso resulta em ordem decrescente
 */
void OnListarPorEnergia(AppData* app) {
    char categoria[MAX_CATEGORIA];

    /* Dialogo de selecao de categoria */
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

    /* Cabecalho */
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);
    AdicionarItemListBox(app->hwndListBox, "|  ORDENADO POR: ENERGIA (DECRESCENTE) - Usando Arvore Binaria          |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Cabecalho da tabela */
    AdicionarCabecalhoTabela(app->hwndListBox);

    /* Percorrer arvore em ordem decrescente */
    /* A arvore arvore_energia foi construida ao carregar dados */
    /* Cada no tem chave = energia_kcal */
    if (cat->arvore_energia != NULL) {
        AdicionarNosArvoreDecrescente(cat->arvore_energia, app->hwndListBox);
    }

    /* Rodape */
    AdicionarSeparador(app->hwndListBox, 110);

    /* Atualizar status */
    SetWindowText(app->hwndStatus, "Alimentos ordenados por energia (maior -> menor).");
}

/******************************************************************************
 * HANDLER 4: ORDENAR POR PROTEINA (DECRESCENTE)
 *
 * Funciona identicamente ao Handler 3, mas usa arvore de proteina.
 *****************************************************************************/

/**
 * OnListarPorProteina - Handler do botao "4. Ordenar por Proteina"
 *
 * @param app: Ponteiro para dados da aplicacao
 *
 * FUNCIONAMENTO:
 * Identico a OnListarPorEnergia, mas usa arvore_proteina ao inves de arvore_energia.
 *
 * ESTRUTURA DE DADOS:
 * NoCategoria->arvore_proteina: BST indexada por proteina (g)
 */
void OnListarPorProteina(AppData* app) {
    char categoria[MAX_CATEGORIA];

    /* Dialogo de selecao */
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

    /* Cabecalho */
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);
    AdicionarItemListBox(app->hwndListBox, "|  ORDENADO POR: PROTEINA (DECRESCENTE) - Usando Arvore Binaria         |");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Cabecalho da tabela */
    AdicionarCabecalhoTabela(app->hwndListBox);

    /* Percorrer arvore de proteina em ordem decrescente */
    if (cat->arvore_proteina != NULL) {
        AdicionarNosArvoreDecrescente(cat->arvore_proteina, app->hwndListBox);
    }

    /* Rodape */
    AdicionarSeparador(app->hwndListBox, 110);

    /* Atualizar status */
    SetWindowText(app->hwndStatus, "Alimentos ordenados por proteina (maior -> menor).");
}

/******************************************************************************
 * HANDLER 5: FILTRAR POR INTERVALO DE ENERGIA
 *
 * Permite usuario especificar energia minima e maxima, depois exibe apenas
 * alimentos nesse intervalo.
 *****************************************************************************/

/**
 * OnIntervaloEnergia - Handler do botao "5. Filtrar Energia"
 *
 * @param app: Ponteiro para dados da aplicacao
 *
 * FUNCIONAMENTO:
 * 1. Dialogo para selecionar categoria
 * 2. Dialogo para digitar energia minima
 * 3. Dialogo para digitar energia maxima
 * 4. Busca na arvore binaria valores no intervalo [min, max]
 * 5. Exibe resultados
 *
 * VANTAGEM DA ARVORE BINARIA:
 * Busca otimizada - nao precisa percorrer todos os nos, apenas os relevantes.
 * Complexidade: O(log n + k) onde k = numero de resultados
 */
void OnIntervaloEnergia(AppData* app) {
    char categoria[MAX_CATEGORIA];
    double min = 0.0;  // Energia minima (kcal)
    double max = 0.0;  // Energia maxima (kcal)

    /* Passo 1: Selecionar categoria */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;  // Cancelado
    }

    /* Passo 2: Perguntar energia minima */
    /* MostrarDialogoNumero retorna FALSE se cancelado */
    if (!MostrarDialogoNumero(app->hwndMain, "Energia Minima",
            "Digite o valor minimo de energia (kcal):", &min)) {
        return;  // Cancelado
    }

    /* Passo 3: Perguntar energia maxima */
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

    /* Cabecalho com informacoes do filtro */
    AdicionarItemListBox(app->hwndListBox, "");
    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    sprintf(buffer, "|  %-70s|", cat->nome);
    AdicionarItemListBox(app->hwndListBox, buffer);

    /* Linha com intervalo de energia */
    /* sprintf formata, depois ajustamos manualmente para fechar a borda */
    sprintf(buffer, "|  FILTRO: Energia entre %.0f e %.0f kcal                                      ", min, max);
    buffer[73] = '|';  // Forcar '|' na posicao 73 para fechar borda
    buffer[74] = '\0'; // Terminador null
    AdicionarItemListBox(app->hwndListBox, buffer);

    AdicionarItemListBox(app->hwndListBox, "+========================================================================+");
    AdicionarItemListBox(app->hwndListBox, "");

    /* Cabecalho da tabela */
    AdicionarCabecalhoTabela(app->hwndListBox);

    /* Buscar na arvore valores no intervalo [min, max] */
    /* Funcao recursiva otimizada que poda ramos desnecessarios */
    if (cat->arvore_energia != NULL) {
        AdicionarNosIntervalo(cat->arvore_energia, app->hwndListBox, min, max);
    }

    /* Rodape */
    AdicionarSeparador(app->hwndListBox, 110);

    /* Atualizar status */
    sprintf(buffer, "Filtro aplicado: energia entre %.0f e %.0f kcal.", min, max);
    SetWindowText(app->hwndStatus, buffer);
}

/******************************************************************************
 * HANDLER 6: FILTRAR POR INTERVALO DE PROTEINA
 *
 * Funciona identicamente ao Handler 5, mas para proteina.
 *****************************************************************************/

/**
 * OnIntervaloProteina - Handler do botao "6. Filtrar Proteina"
 *
 * @param app: Ponteiro para dados da aplicacao
 *
 * FUNCIONAMENTO:
 * Identico a OnIntervaloEnergia, mas usa arvore_proteina.
 */
void OnIntervaloProteina(AppData* app) {
    char categoria[MAX_CATEGORIA];
    double min = 0.0;  // Proteina minima (g)
    double max = 0.0;  // Proteina maxima (g)

    /* Selecionar categoria */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;
    }

    /* Perguntar proteina minima */
    if (!MostrarDialogoNumero(app->hwndMain, "Proteina Minima",
            "Digite o valor minimo de proteina (g):", &min)) {
        return;
    }

    /* Perguntar proteina maxima */
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

    /* Cabecalho */
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

    /* Cabecalho da tabela */
    AdicionarCabecalhoTabela(app->hwndListBox);

    /* Buscar na arvore de proteina valores no intervalo */
    if (cat->arvore_proteina != NULL) {
        AdicionarNosIntervalo(cat->arvore_proteina, app->hwndListBox, min, max);
    }

    /* Rodape */
    AdicionarSeparador(app->hwndListBox, 110);

    /* Atualizar status */
    sprintf(buffer, "Filtro aplicado: proteina entre %.1f e %.1f g.", min, max);
    SetWindowText(app->hwndStatus, buffer);
}

/******************************************************************************
 * HANDLER 7: REMOVER CATEGORIA
 *
 * Remove uma categoria inteira (e todos os seus alimentos) do sistema.
 * Pede confirmacao antes de remover.
 *****************************************************************************/

/**
 * OnRemoverCategoria - Handler do botao "7. Remover Categoria"
 *
 * @param app: Ponteiro para dados da aplicacao
 *
 * FUNCIONAMENTO:
 * 1. Dialogo para selecionar categoria
 * 2. MessageBox para confirmar remocao
 * 3. Chama funcao remover_categoria (de categoria.c)
 * 4. Marca dados como modificados
 * 5. Exibe mensagem de confirmacao
 *
 * IMPORTANTE:
 * - Remove categoria e TODOS os alimentos dela
 * - Libera memoria das arvores binarias e listas encadeadas
 * - Marca dados_modificados = true para salvar ao sair
 */
void OnRemoverCategoria(AppData* app) {
    char categoria[MAX_CATEGORIA];

    /* Selecionar categoria a remover */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;  // Cancelado
    }

    /* Mensagem de confirmacao */
    char mensagem[512];
    sprintf(mensagem, "Deseja realmente remover a categoria:\n\n%s\n\nTodos os alimentos desta categoria serao removidos!", categoria);

    /* MessageBox com botoes Sim/Nao e icone de aviso */
    /* MB_YESNO = botoes Sim e Nao */
    /* MB_ICONWARNING = icone de aviso (triangulo amarelo) */
    /* Retorna IDYES se usuario clicou Sim */
    if (MessageBox(app->hwndMain, mensagem, "Confirmar Remocao",
            MB_YESNO | MB_ICONWARNING) != IDYES) {
        return;  // Usuario clicou Nao, cancelar operacao
    }

    /* Remover categoria da lista */
    /* remover_categoria retorna nova cabeca da lista (caso primeira categoria seja removida) */
    app->lista_categorias = remover_categoria(app->lista_categorias, categoria);

    /* Marcar dados como modificados */
    /* Isso fara o sistema salvar automaticamente ao sair */
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
 * Remove um alimento especifico de uma categoria.
 * Pede confirmacao antes de remover.
 *****************************************************************************/

/**
 * OnRemoverAlimento - Handler do botao "8. Remover Alimento"
 *
 * @param app: Ponteiro para dados da aplicacao
 *
 * FUNCIONAMENTO:
 * 1. Dialogo para selecionar categoria
 * 2. Dialogo para digitar numero do alimento
 * 3. Busca nome do alimento para exibir na confirmacao
 * 4. MessageBox para confirmar remocao
 * 5. Chama funcao remover_alimento_de_categoria
 * 6. Atualiza arvores binarias automaticamente
 * 7. Marca dados como modificados
 *
 * IMPORTANTE:
 * - Remove alimento da lista encadeada
 * - Remove alimento das arvores de energia e proteina
 * - Marca dados_modificados = true para salvar ao sair
 */
void OnRemoverAlimento(AppData* app) {
    char categoria[MAX_CATEGORIA];
    double numero_double = 0.0;  // Usamos double pois dialogo retorna double

    /* Selecionar categoria */
    if (!MostrarDialogoCategoria(app->hwndMain, app->lista_categorias, categoria)) {
        return;
    }

    /* Perguntar numero do alimento */
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

    /* Buscar nome do alimento para mostrar na confirmacao */
    /* Percorre lista de alimentos procurando pelo numero */
    NoAlimento* alim = cat->lista_alimentos;
    char nome_alimento[MAX_DESCRICAO] = "Alimento nao encontrado";  // Valor padrao se nao achar

    while (alim != NULL) {
        if (alim->numero == numero) {
            /* Encontrou! Copiar nome */
            strcpy(nome_alimento, alim->descricao);
            break;  // Sair do loop
        }
        alim = alim->proximo;  // Proximo alimento
    }

    /* Mensagem de confirmacao com dados do alimento */
    char mensagem[512];
    sprintf(mensagem, "Deseja realmente remover o alimento?\n\nNumero: %d\nNome: %s\nCategoria: %s",
            numero, nome_alimento, categoria);

    /* Pedir confirmacao */
    if (MessageBox(app->hwndMain, mensagem, "Confirmar Remocao",
            MB_YESNO | MB_ICONWARNING) != IDYES) {
        return;  // Cancelado
    }

    /* Remover alimento */
    /* Funcao remove da lista e das arvores automaticamente */
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
 * Esta e a funcao mais importante do sistema GUI.
 * O Windows chama esta funcao sempre que um evento ocorre (clique, tecla, etc).
 *
 * E o "cerebro" da interface: recebe eventos e chama os handlers apropriados.
 *****************************************************************************/

/**
 * WndProc - Procedimento de janela (Window Procedure)
 *
 * @param hwnd: Handle da janela que recebeu a mensagem
 * @param msg: Tipo de mensagem (WM_CREATE, WM_COMMAND, WM_CLOSE, etc)
 * @param wParam: Parametro 1 (depende da mensagem)
 * @param lParam: Parametro 2 (depende da mensagem)
 *
 * @return: Valor de retorno (depende da mensagem)
 *
 * MENSAGENS PRINCIPAIS:
 * - WM_CREATE: Janela sendo criada (criar controles)
 * - WM_COMMAND: Botao clicado ou menu selecionado
 * - WM_CLOSE: Usuario clicou no X
 * - WM_DESTROY: Janela sendo destruida (limpar recursos)
 *
 * FUNCIONAMENTO:
 * 1. Switch/case baseado no tipo de mensagem
 * 2. Processar mensagem apropriadamente
 * 3. Retornar valor adequado
 * 4. Se nao processar, chamar DefWindowProc (processamento padrao)
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        /*** WM_CREATE: Janela sendo criada ***/
        case WM_CREATE: {
            /* Esta mensagem e enviada quando CreateWindow e chamado */
            /* Momento ideal para criar todos os controles filhos */
            CriarControles(hwnd);
            break;
        }

        /*** WM_COMMAND: Comando (botao clicado, menu, etc) ***/
        case WM_COMMAND: {
            /* Esta mensagem e enviada quando:
             * - Usuario clica em botao
             * - Seleciona item de menu
             * - Tecla de atalho e pressionada
             *
             * LOWORD(wParam) = ID do controle que gerou o evento
             * HIWORD(wParam) = Codigo de notificacao (BN_CLICKED para botoes)
             */

            /* Switch baseado no ID do botao clicado */
            switch (LOWORD(wParam)) {
                case ID_BTN_LISTAR_CATEGORIAS:
                    /* Botao 1 clicado */
                    OnListarCategorias(g_appData);
                    break;

                case ID_BTN_LISTAR_ALIMENTOS:
                    /* Botao 2 clicado */
                    OnListarAlimentos(g_appData);
                    break;

                case ID_BTN_ENERGIA_DESC:
                    /* Botao 3 clicado */
                    OnListarPorEnergia(g_appData);
                    break;

                case ID_BTN_PROTEINA_DESC:
                    /* Botao 4 clicado */
                    OnListarPorProteina(g_appData);
                    break;

                case ID_BTN_INTERVALO_ENERGIA:
                    /* Botao 5 clicado */
                    OnIntervaloEnergia(g_appData);
                    break;

                case ID_BTN_INTERVALO_PROTEINA:
                    /* Botao 6 clicado */
                    OnIntervaloProteina(g_appData);
                    break;

                case ID_BTN_REMOVER_CATEGORIA:
                    /* Botao 7 clicado */
                    OnRemoverCategoria(g_appData);
                    break;

                case ID_BTN_REMOVER_ALIMENTO:
                    /* Botao 8 clicado */
                    OnRemoverAlimento(g_appData);
                    break;

                case ID_BTN_SAIR:
                    /* Botao 9 (SAIR) clicado */
                    /* Pedir confirmacao antes de sair */
                    if (MessageBox(hwnd, "Deseja realmente sair do sistema?",
                        "Confirmar Saida", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        /* Usuario confirmou - enviar mensagem para encerrar aplicacao */
                        /* PostQuitMessage faz GetMessage retornar 0, encerrando loop */
                        PostQuitMessage(0);
                    }
                    break;
            }
            break;
        }

        /*** WM_CLOSE: Usuario clicou no X da janela ***/
        case WM_CLOSE: {
            /* Pedir confirmacao antes de fechar */
            if (MessageBox(hwnd, "Deseja realmente fechar o programa?",
                "Confirmar Fechamento", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                /* Usuario confirmou - destruir janela */
                /* Isso enviara WM_DESTROY */
                DestroyWindow(hwnd);
            }
            /* Se usuario cancelou, nao fazer nada (janela permanece aberta) */
            break;
        }

        /*** WM_DESTROY: Janela sendo destruida ***/
        case WM_DESTROY: {
            /* Esta mensagem e enviada quando DestroyWindow e chamado */
            /* Momento para liberar recursos alocados */

            /* Deletar fontes se foram criadas */
            if (g_hFontMono) DeleteObject(g_hFontMono);
            if (g_hFontNormal) DeleteObject(g_hFontNormal);

            /* Enviar mensagem de quit para encerrar aplicacao */
            PostQuitMessage(0);
            break;
        }

        /*** Mensagem nao processada ***/
        default: {
            /* Para qualquer mensagem que nao processamos, */
            /* chamar processamento padrao do Windows */
            /* DefWindowProc implementa comportamento padrao */
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

    /* Retornar 0 indica que processamos a mensagem */
    return 0;
}

/******************************************************************************
 * INICIALIZACAO DA GUI
 *
 * Registra classe de janela e cria a janela principal.
 *****************************************************************************/

/**
 * InicializarGUI - Inicializa a interface grafica
 *
 * @param hInstance: Handle da instancia da aplicacao (passado em WinMain)
 *
 * FUNCIONAMENTO:
 * 1. Registra "classe de janela" (define propriedades de janelas deste tipo)
 * 2. Cria janela principal
 * 3. Exibe janela
 *
 * CLASSE DE JANELA:
 * Define propriedades compartilhadas:
 * - Procedimento de janela (WndProc)
 * - Icone
 * - Cursor
 * - Cor de fundo
 * - etc.
 */
void InicializarGUI(HINSTANCE hInstance) {
    /* Estrutura que define a classe de janela */
    WNDCLASSEX wc = {0};  // Inicializar com zeros

    /* Preencher estrutura com propriedades da classe */
    wc.cbSize = sizeof(WNDCLASSEX);        // Tamanho da estrutura
    wc.style = 0;                          // Estilos da classe (0 = padrao)
    wc.lpfnWndProc = WndProc;              // Ponteiro para procedimento de janela
    wc.cbClsExtra = 0;                     // Bytes extras por classe (nao usado)
    wc.cbWndExtra = 0;                     // Bytes extras por janela (nao usado)
    wc.hInstance = hInstance;              // Handle da instancia
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);     // Icone grande (padrao)
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Cursor (seta padrao)
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // Cor de fundo (branco)
    wc.lpszMenuName = NULL;                // Nome do menu (nao temos menu)
    wc.lpszClassName = "SistemaAlimentosClass";     // Nome unico da classe
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);   // Icone pequeno (padrao)

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
        "Sistema de Gerenciamento de Alimentos - Trabalho de Programacao Imperativa RA2",  // Titulo
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,  // Estilos da janela
        /* WS_OVERLAPPED = janela sobreposta (normal)
         * WS_CAPTION = tem barra de titulo
         * WS_SYSMENU = tem menu do sistema (icone no canto)
         * WS_MINIMIZEBOX = tem botao minimizar
         * NAO tem WS_MAXIMIZEBOX (sem botao maximizar - janela com tamanho fixo)
         * NAO tem WS_THICKFRAME (sem borda redimensionavel)
         */
        CW_USEDEFAULT, CW_USEDEFAULT,       // Posicao X, Y (Windows escolhe)
        1030, 710,                          // Largura, Altura (tamanho fixo)
        NULL,                               // Janela pai (nenhuma - e janela principal)
        NULL,                               // Menu (nenhum)
        hInstance,                          // Handle da instancia
        NULL                                // Parametros de criacao (nenhum)
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

    /* Forcar redesenho da janela */
    /* Isso garante que a janela seja desenhada corretamente */
    UpdateWindow(g_appData->hwndMain);
}

/******************************************************************************
 * FUNCOES DE GERENCIAMENTO DE DADOS GLOBAIS
 *
 * Estas funcoes permitem compartilhar AppData entre modulos.
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
 * GetAppData - Obtem ponteiro global para AppData
 *
 * @return: Ponteiro para AppData
 *
 * Permite outros modulos acessarem dados se necessario
 */
AppData* GetAppData() {
    return g_appData;
}
