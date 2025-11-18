# DataReader - Sistema de Gerenciamento de Dados Alimentares

Sistema completo para processamento e analise de dados nutricionais baseado na Tabela TACO (Tabela Brasileira de Composicao de Alimentos), contendo 597 alimentos distribuidos em 15 categorias.

## Sobre o Projeto

Este projeto foi desenvolvido como trabalho academico para a PUC-PR (Pontificia Universidade Catolica do Parana), dividido em dois modulos complementares:

- **datareader1**: Programacao Imperativa - Selecao e analise de alimentos com saida CSV
- **datareader2**: Estruturas de Dados - Implementacao avancada com arvores binarias, listas encadeadas e interface grafica

## Funcionalidades

### Modulo 1 (datareader1)
- Selecao aleatoria de 100 alimentos com restricoes (minimo 10 categorias)
- Exportacao para formato CSV
- Menu interativo com 10 opcoes de consulta
- Ordenacao por energia, proteina, carboidratos e umidade
- Calculo de razoes energia/proteina e energia/carboidrato

### Modulo 2 (datareader2)
- Conversao de JSON para formato binario otimizado
- Interface CLI com menu completo
- Interface GUI nativa Windows (Win32 API)
- Arvores binarias de busca para ordenacao eficiente
- Listas encadeadas para gerenciamento de categorias
- Consultas por intervalo de valores
- Persistencia de dados com opcao de salvamento

## Tecnologias Utilizadas

- **Linguagem**: C (C99/C11)
- **Bibliotecas**:
  - json-c (parsing JSON)
  - Win32 API (interface grafica)
  - POSIX C (funcoes padrao)
- **Compilador**: GCC / MinGW
- **Build**: GNU Make

## Estrutura do Projeto

```
datareader/
├── datareader1/                 # Modulo 1 - Programacao Imperativa
│   ├── main.c                   # Ponto de entrada
│   ├── funcoes_usuario.c/h      # Funcoes de consulta
│   ├── dados.json               # Base de dados (597 alimentos)
│   ├── alimentos_selecionados.csv
│   └── Makefile
│
├── datareader2/                 # Modulo 2 - Estruturas de Dados
│   ├── P1.c                     # Conversor JSON -> Binario
│   ├── P2.c                     # Interface CLI
│   ├── P2_GUI.c                 # Interface GUI
│   ├── tipos.h                  # Definicoes de tipos
│   ├── categoria.c/h            # Gerenciamento de categorias
│   ├── arvore.c/h               # Arvores binarias de busca
│   ├── arquivo.c/h              # Operacoes de arquivo
│   ├── menu.c/h                 # Sistema de menu CLI
│   ├── gui.c/h                  # Componentes GUI
│   ├── dialogo.c/h              # Sistema de dialogos
│   ├── utils.c/h                # Funcoes utilitarias
│   ├── dados.json               # Base de dados
│   ├── dados.bin                # Base binaria compilada
│   └── Makefile
│
└── README.md
```

## Pre-requisitos

### Linux
```bash
sudo apt-get install build-essential libjson-c-dev
```

### Windows
- MinGW-w64 com suporte a json-c
- Windows SDK (para compilacao GUI)

## Instalacao e Execucao

### Modulo 1 (datareader1)

```bash
cd datareader1
make
./programa
```

### Modulo 2 (datareader2)

**Passo 1: Converter JSON para Binario**
```bash
cd datareader2
make P1
./P1
```

**Passo 2a: Interface CLI**
```bash
make P2
./P2
```

**Passo 2b: Interface GUI (Windows)**
```bash
# Usando batch script
EXECUTAR_GUI.bat

# Ou compilacao manual
gcc -o P2_GUI.exe P2_GUI.c gui.c dialogo.c categoria.c arvore.c arquivo.c utils.c -lcomctl32 -lgdi32 -mwindows
P2_GUI.exe
```

**Usando Makefile**
```bash
make all      # Compila P1 e P2
make run      # Executa P1 e depois P2
make clean    # Limpa arquivos compilados
```

## Menu de Opcoes

### Modulo 1
| Opcao | Descricao |
|-------|-----------|
| a | Listar todas as categorias |
| b | Listar alimentos por categoria (ordem alfabetica) |
| c | Listar alimentos por categoria (energia decrescente) |
| d | N alimentos com maior umidade por categoria |
| e | N alimentos com maior energia por categoria |
| f | N alimentos com maior proteina por categoria |
| g | N alimentos com maior carboidrato |
| h | N alimentos com maior razao energia/proteina |
| i | N alimentos com maior razao energia/carboidrato |
| j | Sair |

### Modulo 2
| Opcao | Descricao |
|-------|-----------|
| 1 | Listar todas as categorias |
| 2 | Listar alimentos de uma categoria (alfabetico) |
| 3 | Ordenar por energia (decrescente) |
| 4 | Ordenar por proteina (decrescente) |
| 5 | Filtrar por energia minima |
| 6 | Filtrar por proteina minima |
| 7 | Remover categoria |
| 8 | Remover alimento |
| 9 | Sair (com opcao de salvar) |

## Categorias de Alimentos

O sistema trabalha com 15 categorias da Tabela TACO:

1. Alimentos Preparados
2. Bebidas
3. Carnes e Derivados
4. Cereais e Derivados
5. Frutas e Derivados
6. Gorduras e Oleos
7. Leguminosas e Derivados
8. Leite e Derivados
9. Miscelaneas
10. Nozes e Sementes
11. Outros Alimentos Industrializados
12. Ovos e Derivados
13. Pescados e Frutos do Mar
14. Produtos Acucarados
15. Verduras/Hortalicas e Derivados

## Estruturas de Dados Implementadas

- **Structs**: Representacao de alimentos com campos nutricionais
- **Enums**: Tipagem das 15 categorias alimentares
- **Listas Encadeadas**: Gerenciamento de alimentos por categoria
- **Arvores Binarias de Busca**: Indexacao por energia e proteina
- **Arquivos Binarios**: Persistencia eficiente de dados

## Formatos de Dados

### Entrada
- `dados.json`: 597 alimentos em formato JSON

### Saida
- `alimentos_selecionados.csv`: 100 alimentos selecionados (Modulo 1)
- `dados.bin`: Base de dados binaria otimizada (Modulo 2)

### Campos do Alimento
- Numero (ID)
- Descricao
- Umidade (%)
- Energia (kcal)
- Proteina (g)
- Carboidrato (g)
- Categoria

## Compilacao Cruzada (Linux para Windows)

```bash
x86_64-w64-mingw32-gcc -o P2_GUI.exe P2_GUI.c gui.c dialogo.c categoria.c arvore.c arquivo.c utils.c -lcomctl32 -lgdi32 -mwindows -O2
```

## Contexto Academico

Projeto desenvolvido para disciplinas de:
- Programacao Imperativa
- Estruturas de Dados e Algoritmos

**Instituicao**: PUC-PR (Pontificia Universidade Catolica do Parana)

## Licenca

Projeto academico - uso educacional.
