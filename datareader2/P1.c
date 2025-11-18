#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <json-c/json.h>

#define MAX_DESCRICAO 100
#define MAX_CATEGORIA 50

/* Estrutura para armazenar alimento em memoria (com ponteiros dinamicos) */
typedef struct {
    int numero;
    char* descricao;
    double umidade;
    int energia_kcal;
    double proteina;
    double carboidrato;
    char* categoria;
} Alimento;

/* Estrutura para armazenar alimento em arquivo binario (tamanho fixo) */
typedef struct {
    int numero;
    char descricao[MAX_DESCRICAO];
    double umidade;
    int energia_kcal;
    double proteina;
    double carboidrato;
    char categoria[MAX_CATEGORIA];
} AlimentoArquivo;

/* ===================================================================================
   FUNCAO PARA LER OS DADOS DO ARQUIVO .JSON ORIGINAL
   =================================================================================== */
Alimento** ler_alimentos_do_json(const char* arquivo_json, int* total_alimentos) {
    FILE* arquivo = fopen(arquivo_json, "r");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo dados.json");
        return NULL;
    }

    /* Le o conteudo inteiro do arquivo para a memoria */
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    char* buffer_json = (char*)malloc(tamanho + 1);
    if (buffer_json == NULL) {
        fprintf(stderr, "Falha ao alocar memoria para o buffer do JSON.\n");
        fclose(arquivo);
        return NULL;
    }

    fread(buffer_json, 1, tamanho, arquivo);
    fclose(arquivo);
    buffer_json[tamanho] = '\0';

    /* Usa a biblioteca 'json-c' para interpretar o texto do arquivo */
    json_object* json_completo = json_tokener_parse(buffer_json);
    if (json_completo == NULL) {
        fprintf(stderr, "Erro ao interpretar o conteudo JSON.\n");
        free(buffer_json);
        return NULL;
    }

    size_t n_alimentos = json_object_array_length(json_completo);
    *total_alimentos = n_alimentos;

    Alimento** array_alimentos = (Alimento**)malloc(n_alimentos * sizeof(Alimento*));
    if (array_alimentos == NULL) {
        fprintf(stderr, "Falha ao alocar memoria para o array de alimentos.\n");
        free(buffer_json);
        json_object_put(json_completo);
        return NULL;
    }

    /* Percorre cada objeto de alimento no JSON */
    for (int i = 0; i < n_alimentos; i++) {
        json_object* item = json_object_array_get_idx(json_completo, i);
        array_alimentos[i] = (Alimento*)malloc(sizeof(Alimento));

        /* Pega cada dado do objeto JSON */
        json_object *j_numero, *j_descricao, *j_umidade, *j_energia, *j_proteina, *j_carboidrato, *j_categoria;
        json_object_object_get_ex(item, "Numero", &j_numero);
        json_object_object_get_ex(item, "Descricao", &j_descricao);
        json_object_object_get_ex(item, "Umidade", &j_umidade);
        json_object_object_get_ex(item, "Energia_kcal", &j_energia);
        json_object_object_get_ex(item, "Proteina", &j_proteina);
        json_object_object_get_ex(item, "Carboidrato", &j_carboidrato);
        json_object_object_get_ex(item, "Categoria", &j_categoria);

        /* Preenche a estrutura com os dados */
        array_alimentos[i]->numero = json_object_get_int(j_numero);
        array_alimentos[i]->descricao = strdup(json_object_get_string(j_descricao));
        array_alimentos[i]->umidade = json_object_get_double(j_umidade);
        array_alimentos[i]->energia_kcal = json_object_get_int(j_energia);
        array_alimentos[i]->proteina = json_object_get_double(j_proteina);
        array_alimentos[i]->carboidrato = json_object_get_double(j_carboidrato);
        array_alimentos[i]->categoria = strdup(json_object_get_string(j_categoria));
    }

    json_object_put(json_completo);
    free(buffer_json);
    return array_alimentos;
}

/* ===================================================================================
   FUNCAO PARA SALVAR OS DADOS EM BINARIO
   =================================================================================== */
bool salvar_em_binario(Alimento** array_memoria, int total_alimentos, const char* arquivo_bin) {
    FILE* f_binario = fopen(arquivo_bin, "wb");
    if (f_binario == NULL) {
        perror("Nao foi possivel criar o arquivo binario");
        return false;
    }

    printf("Escrevendo dados em '%s'...\n", arquivo_bin);

    for (int i = 0; i < total_alimentos; i++) {
        AlimentoArquivo temp_bin;
        memset(&temp_bin, 0, sizeof(AlimentoArquivo));

        temp_bin.numero = array_memoria[i]->numero;
        temp_bin.umidade = array_memoria[i]->umidade;
        temp_bin.energia_kcal = array_memoria[i]->energia_kcal;
        temp_bin.proteina = array_memoria[i]->proteina;
        temp_bin.carboidrato = array_memoria[i]->carboidrato;

        strncpy(temp_bin.descricao, array_memoria[i]->descricao, MAX_DESCRICAO - 1);
        strncpy(temp_bin.categoria, array_memoria[i]->categoria, MAX_CATEGORIA - 1);

        fwrite(&temp_bin, sizeof(AlimentoArquivo), 1, f_binario);
    }

    fclose(f_binario);
    printf("Arquivo '%s' criado com sucesso.\n", arquivo_bin);
    return true;
}

/* ===================================================================================
   FUNCAO MAIN
   =================================================================================== */
int main() {
    int total_alimentos = 0;
    const char* json_filename = "dados.json";
    const char* bin_filename = "dados.bin";

    /* 1. LER OS DADOS DO ARQUIVO JSON */
    printf("=== PROGRAMA P1 - Conversao JSON para BINARIO ===\n\n");
    printf("Lendo dados de '%s'...\n", json_filename);
    Alimento** array_memoria = ler_alimentos_do_json(json_filename, &total_alimentos);

    if (array_memoria == NULL) {
        fprintf(stderr, "Falha ao ler o arquivo JSON. Encerrando.\n");
        exit(1);
    }
    printf("%d alimentos lidos com sucesso.\n\n", total_alimentos);

    /* 2. SALVAR OS DADOS NO ARQUIVO BINARIO */
    if (!salvar_em_binario(array_memoria, total_alimentos, bin_filename)) {
        fprintf(stderr, "Falha ao salvar o arquivo binario.\n");
    }

    /* 3. LIBERAR MEMORIA */
    printf("\nLimpando memoria...\n");
    for (int i = 0; i < total_alimentos; i++) {
        free(array_memoria[i]->descricao);
        free(array_memoria[i]->categoria);
        free(array_memoria[i]);
    }
    free(array_memoria);

    printf("Processo concluido com sucesso!\n");
    return 0;
}
