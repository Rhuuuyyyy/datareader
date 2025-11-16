# 🍎 Sistema de Gerenciamento de Alimentos

**Trabalho de Programação Imperativa - RA2**
Prof. Alcides Calsavara

---

## ✨ AGORA COM 2 VERSÕES!

### 🖼️ **VERSÃO GUI** - Interface Gráfica (RECOMENDADA!)
**Arquivo:** `P2_GUI.exe`

![Interface Gráfica](https://img.shields.io/badge/Interface-Gr%C3%A1fica-blue?style=for-the-badge)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

#### 🎯 Características:
- ✅ **Interface Visual Bonita** - Janelas, botões e listas
- ✅ **Navegação por Mouse** - Clique nos botões, sem digitar!
- ✅ **Intuitivo** - Qualquer um consegue usar
- ✅ **Profissional** - Layout moderno com Win32 API
- ✅ **Mensagens Visuais** - Confirmações e alertas em janelas
- ✅ **Lista Organizada** - Resultados formatados e scrolláveis

#### 🚀 Como usar:
```bash
# Opção 1: Duplo clique
EXECUTAR_GUI.bat

# Opção 2: Direto
P2_GUI.exe
```

**Veja o guia completo:** [`GUIA_INTERFACE_GRAFICA.txt`](GUIA_INTERFACE_GRAFICA.txt)

---

### 📟 **VERSÃO CMD** - Linha de Comando
**Arquivo:** `P2.exe`

![Terminal](https://img.shields.io/badge/Terminal-CMD-black?style=for-the-badge)

#### 🎯 Características:
- ✅ **Clássico** - Interface de terminal tradicional
- ✅ **Leve** - Menor consumo de recursos
- ✅ **Texto** - Ideal para quem prefere comandos

#### 🚀 Como usar:
```bash
# Opção 1: Script automático
EXECUTAR_WINDOWS.bat

# Opção 2: Manual
P2.exe
```

**Veja o guia completo:** [`COMO_USAR_WINDOWS.txt`](COMO_USAR_WINDOWS.txt)

---

## 📊 Comparação das Versões

| Característica | P2_GUI.exe (GUI) | P2.exe (CMD) |
|----------------|------------------|--------------|
| Interface | 🖼️ Janela gráfica | 📟 Terminal preto |
| Uso | 🖱️ Cliques de mouse | ⌨️ Digitar números |
| Visual | ✨ Moderno e bonito | 📝 Texto simples |
| Facilidade | ⭐⭐⭐⭐⭐ Muito fácil | ⭐⭐⭐ Moderado |
| Tamanho | 276 KB | 407 KB |
| Recomendado | **✅ SIM** | Para quem prefere CMD |

---

## 🎨 Preview da Interface Gráfica

```
┌─────────────────────────────────────────────────────────────────────────┐
│ Sistema de Gerenciamento de Alimentos - Trabalho RA2               [_][□][X] │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  ┌──────────────────┐ ┌──────────────────┐ ┌──────────────────┐      │
│  │ 1. Listar        │ │ 4. Ordenar por   │ │ 7. Remover       │      │
│  │    Categorias    │ │    Proteína      │ │    Categoria     │      │
│  └──────────────────┘ └──────────────────┘ └──────────────────┘      │
│                                                                         │
│  ┌──────────────────┐ ┌──────────────────┐ ┌──────────────────┐      │
│  │ 2. Listar        │ │ 5. Filtrar       │ │ 8. Remover       │      │
│  │    Alimentos     │ │    Energia       │ │    Alimento      │      │
│  └──────────────────┘ └──────────────────┘ └──────────────────┘      │
│                                                                         │
│  ┌──────────────────┐ ┌──────────────────┐ ┌──────────────────┐      │
│  │ 3. Ordenar por   │ │ 6. Filtrar       │ │ 9. Sair          │      │
│  │    Energia       │ │    Proteína      │ │                  │      │
│  └──────────────────┘ └──────────────────┘ └──────────────────┘      │
│                                                                         │
│  ╔═══════════════════════════════════════════════════════════════╗    │
│  ║ RESULTADOS                                                  ▲ ║    │
│  ╠═══════════════════════════════════════════════════════════════╣    │
│  ║ === CATEGORIAS DE ALIMENTOS ===                            ║ ║    │
│  ║                                                             ║ ║    │
│  ║  1. Alimentos preparados                                   ║ ║    │
│  ║  2. Bebidas (alcoólicas e não alcoólicas)                  ║ ║    │
│  ║  3. Carnes e derivados                                     ║ ║    │
│  ║  4. Cereais e derivados                                    ║ ║    │
│  ║  5. Frutas e derivados                                     ║ ║    │
│  ║  ...                                                        ║ ║    │
│  ║                                                             ▼ ║    │
│  ╚═══════════════════════════════════════════════════════════════╝    │
│                                                                         │
│  Status: Categorias listadas com sucesso. ✓                           │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 📦 Arquivos Incluídos

### Executáveis:
- **P2_GUI.exe** (276 KB) - Versão com interface gráfica ⭐
- **P2.exe** (407 KB) - Versão linha de comando
- **dados.bin** (112 KB) - Banco de dados (597 alimentos)

### Scripts de Execução:
- **EXECUTAR_GUI.bat** - Inicia versão gráfica
- **EXECUTAR_WINDOWS.bat** - Inicia versão CMD

### Documentação:
- **GUIA_INTERFACE_GRAFICA.txt** - Manual da versão GUI
- **COMO_USAR_WINDOWS.txt** - Manual da versão CMD
- **README.md** - Documentação técnica completa

### Código Fonte:
- **P2_GUI.c** - Programa principal GUI
- **gui.h / gui.c** - Implementação da interface gráfica
- **P2.c** - Programa principal CMD
- **tipos.h** - Estruturas de dados
- **utils.h/c** - Utilitários
- **arvore.h/c** - Árvores binárias
- **categoria.h/c** - Gerenciamento de categorias
- **arquivo.h/c** - I/O de arquivos
- **menu.h/c** - Menu do CMD
- **Makefile** - Compilação

---

## 🚀 Início Rápido

### Versão Gráfica (Recomendada):
```bash
1. Faça download ou git pull do repositório
2. Dê duplo clique em: EXECUTAR_GUI.bat
3. Uma janela bonita abre automaticamente!
4. Clique nos botões para usar as funções
```

### Versão Terminal:
```bash
1. Dê duplo clique em: EXECUTAR_WINDOWS.bat
2. Siga o menu numerado
3. Digite o número da opção desejada
```

---

## 💻 Requisitos

- **Sistema Operacional:** Windows 7 ou superior (64-bit)
- **Instalações Necessárias:** ❌ NENHUMA!
- **Bibliotecas Extras:** ❌ NÃO PRECISA!
- **Espaço em Disco:** ~1 MB

### ✅ **100% Pronto para Usar!**

Não precisa instalar:
- ❌ MSYS2
- ❌ GCC
- ❌ Bibliotecas
- ❌ Compiladores
- ❌ Nada!

Apenas baixe e execute! 🎉

---

## 📚 Funcionalidades

Ambas as versões têm as mesmas 9 funções:

1. **Listar Categorias** - Mostra as 15 categorias
2. **Listar Alimentos** - Alimentos de uma categoria (alfabético)
3. **Ordenar por Energia** - Ordem decrescente usando árvore binária
4. **Ordenar por Proteína** - Ordem decrescente usando árvore binária
5. **Filtrar Energia** - Busca por intervalo de calorias
6. **Filtrar Proteína** - Busca por intervalo de proteínas
7. **Remover Categoria** - Remove categoria completa
8. **Remover Alimento** - Remove alimento específico
9. **Sair** - Salva alterações e encerra

---

## 🗂️ Dados

- **597 alimentos** da Tabela Brasileira de Composição de Alimentos
- **15 categorias** (Cereais, Carnes, Frutas, Verduras, etc.)
- **Informações por alimento:**
  - Número de identificação
  - Descrição completa
  - Energia (kcal)
  - Proteína (g)
  - Umidade (%)
  - Carboidrato (g)
  - Categoria

---

## 🛠️ Tecnologias Utilizadas

### Versão GUI:
- **Win32 API** - Interface gráfica nativa do Windows
- **C99** - Linguagem de programação
- **MinGW-w64** - Compilador cross-platform

### Estruturas de Dados:
- **Listas Ligadas** - Para categorias e alimentos (ordem alfabética)
- **Árvores Binárias** - Para indexação por energia e proteína
- **Enumerados** - Para representar categorias
- **Alocação Dinâmica** - malloc, calloc, free

---

## 📖 Mais Informações

- **Documentação Técnica:** [README.md](README.md)
- **Guia GUI:** [GUIA_INTERFACE_GRAFICA.txt](GUIA_INTERFACE_GRAFICA.txt)
- **Guia CMD:** [COMO_USAR_WINDOWS.txt](COMO_USAR_WINDOWS.txt)
- **Compilação:** [COMPILAR_WINDOWS.md](COMPILAR_WINDOWS.md)

---

## 🎓 Trabalho Acadêmico

Este projeto foi desenvolvido para a disciplina de **Programação Imperativa** e atende todos os requisitos especificados:

✅ Enumerado para categorias
✅ Listas ligadas com inserção ordenada
✅ Árvores binárias de indexação
✅ Código modularizado em .h e .c
✅ Gerenciamento adequado de memória
✅ Interface de usuário (CMD + GUI!)

---

## ⭐ Recomendação

**Use a versão GUI (P2_GUI.exe)!**

É muito mais fácil, bonita e intuitiva! 🎨✨

---

**Desenvolvido com 💙 para o curso de Programação Imperativa**
