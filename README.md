# Sistema de Gerenciamento de Alimentos

Trabalho de Programação Imperativa - RA2
Prof. Alcides Calsavara

## Descrição

Sistema completo para gerenciamento da Tabela Brasileira de Composição de Alimentos, implementado em linguagem C com estruturas de dados avançadas (listas ligadas e árvores binárias).

## Estrutura do Projeto

### Programas Principais

- **P1**: Converte dados de JSON para formato binário
- **P2**: Sistema interativo de gerenciamento com menu de 9 opções

### Arquivos de Código

```
P1.c                 - Programa principal de conversão JSON → Binário
P2.c                 - Programa principal do sistema de gerenciamento

tipos.h              - Definições de tipos, structs e enumerados
utils.h / utils.c    - Funções utilitárias de conversão
arvore.h / arvore.c  - Implementação de árvores binárias de indexação
categoria.h / categoria.c - Gerenciamento de categorias e listas ligadas
arquivo.h / arquivo.c     - Leitura/escrita de arquivos binários
menu.h / menu.c      - Interface de menu e operações do usuário

Makefile             - Compilação automatizada
```

## Características Implementadas

### Estruturas de Dados

1. **Enumerado de Categorias**: 15 categorias de alimentos conforme especificação
2. **Listas Ligadas Ordenadas**:
   - Lista de categorias em ordem alfabética
   - Lista de alimentos por categoria em ordem alfabética
3. **Árvores Binárias de Indexação**:
   - Árvore para indexação por energia (kcal)
   - Árvore para indexação por proteína (g)

### Funcionalidades do P2

1. Listar todas as categorias
2. Listar alimentos de uma categoria
3. Listar alimentos por energia (decrescente) - usa árvore binária
4. Listar alimentos por proteína (decrescente) - usa árvore binária
5. Listar alimentos por intervalo de energia - usa árvore binária
6. Listar alimentos por intervalo de proteína - usa árvore binária
7. Remover categoria
8. Remover alimento (atualiza árvores automaticamente)
9. Sair (salva alterações se houve remoções)

## Compilação

### Pré-requisitos

```bash
# Instalar biblioteca JSON-C
sudo apt-get install libjson-c-dev
```

### Compilar todos os programas

```bash
make all
```

### Compilar individualmente

```bash
make P1    # Compilar apenas P1
make P2    # Compilar apenas P2
```

### Limpar arquivos compilados

```bash
make clean
```

## Execução

### 1. Gerar arquivo binário (executar P1)

```bash
./P1
```

Este programa:
- Lê o arquivo `dados.json` (597 alimentos)
- Converte para formato binário
- Salva em `dados.bin`

### 2. Executar sistema de gerenciamento (P2)

```bash
./P2
```

### Execução completa automatizada

```bash
make run
```

## Exemplos de Uso

### Listar alimentos por energia

```
Escolha uma opcao: 3
Digite o nome da categoria: Cereais e derivados
```

Resultado: Alimentos ordenados por energia em ordem decrescente usando árvore binária.

### Buscar alimentos por intervalo de proteína

```
Escolha uma opcao: 6
Digite o nome da categoria: Carnes e derivados
Digite o valor minimo de proteina (g): 20.0
Digite o valor maximo de proteina (g): 30.0
```

Resultado: Alimentos com proteína entre 20g e 30g usando busca em árvore binária.

### Remover alimento

```
Escolha uma opcao: 8
Digite o nome da categoria do alimento: Cereais e derivados
Digite o numero do alimento a remover: 5
```

As árvores binárias são automaticamente reconstruídas após a remoção.

## Requisitos Atendidos

- ✅ Categorias implementadas com enumerado
- ✅ Código estruturado em funções
- ✅ Organização em arquivos .h e .c
- ✅ P1.c e P2.c contêm apenas a função main
- ✅ Alocação dinâmica de memória (malloc/calloc/free)
- ✅ Toda memória alocada é liberada antes do término
- ✅ Ordenação alfabética garantida durante inserção
- ✅ Árvores binárias com chave e ponteiro para alimento
- ✅ Sem uso de break/continue em loops
- ✅ Preferencialmente um return por função
- ✅ Identificadores significativos
- ✅ Código bem documentado

## Dados

- **Total de alimentos**: 597
- **Categorias**: 15
- **Campos por alimento**:
  - Número
  - Descrição
  - Umidade (%)
  - Energia (kcal)
  - Proteína (g)
  - Carboidrato (g)
  - Categoria

## Autor

Trabalho desenvolvido para a disciplina de Programação Imperativa
