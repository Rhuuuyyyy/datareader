# Sistema de Gerenciamento de Alimentos
Sistema em C para analisar dados nutricionais de 597 alimentos em 15 categorias.

## Arquivos Principais

### Executaveis
- **P1.c** - Conversor de JSON para binario (gera dados.bin)
- **P2.c** - Interface CLI com menu de texto
- **P2_GUI.c** - Interface grafica Win32 (entrada principal)
- **P2.exe** - Executavel da interface CLI
- **P2_GUI.exe** - Executavel da interface grafica

### Dados
- **dados.bin** - Arquivo binario com 597 alimentos e 15 categorias

### Modulos Core
- **tipos.h** - Estruturas de dados (Alimento, Categoria, No, etc)
- **categoria.c/h** - Gerenciamento de categorias e alimentos (listas encadeadas)
- **arvore.c/h** - Arvores binarias de busca para ordenacao
- **arquivo.c/h** - Leitura e escrita de arquivos binarios

### Interface
- **gui.c/h** - 9 funcoes de analise da interface grafica
- **dialogo.c/h** - Sistema de dialogos modais Win32
- **menu.c/h** - Sistema de menus para interface CLI

### Utilitarios
- **utils.c/h** - Funcoes auxiliares (comparacao, impressao, etc)

## Compilacao
```bash
# Windows (MinGW):
gcc -o P2_GUI.exe P2_GUI.c gui.c dialogo.c categoria.c arvore.c arquivo.c utils.c -lcomctl32 -lgdi32 -mwindows -O2

# Linux (cross-compile):
x86_64-w64-mingw32-gcc -o P2_GUI.exe P2_GUI.c gui.c dialogo.c categoria.c arvore.c arquivo.c utils.c -lcomctl32 -lgdi32 -mwindows -O2
```

## Funcionalidades (GUI)
1. Listar todas as categorias
2. Listar alimentos de uma categoria (ordem alfabetica)
3. Ordenar alimentos por energia
4. Ordenar alimentos por proteina
5. Filtrar alimentos por energia minima
6. Filtrar alimentos por proteina minima
7. Buscar alimento por nome
8. Estatisticas nutricionais de uma categoria
9. Comparar duas categorias
