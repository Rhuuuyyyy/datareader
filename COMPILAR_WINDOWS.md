# Compilar no Windows (.exe)

## Opção 1: MinGW (Recomendado)

### 1. Instalar MinGW-w64

Baixe e instale o MinGW-w64:
- https://www.mingw-w64.org/downloads/
- Ou use o instalador: https://github.com/niXman/mingw-builds-binaries/releases

### 2. Instalar biblioteca JSON-C para Windows

Baixe a biblioteca json-c compilada para Windows ou compile manualmente.

Alternativa mais fácil - usar MSYS2:
```bash
# Instalar MSYS2 de: https://www.msys2.org/
# No terminal MSYS2:
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-json-c
```

### 3. Compilar

No terminal do MinGW/MSYS2:

```bash
# Compilar P1
gcc -Wall -Wextra -std=c99 -o P1.exe P1.c -ljson-c

# Compilar P2
gcc -Wall -Wextra -std=c99 -c utils.c
gcc -Wall -Wextra -std=c99 -c arvore.c
gcc -Wall -Wextra -std=c99 -c categoria.c
gcc -Wall -Wextra -std=c99 -c arquivo.c
gcc -Wall -Wextra -std=c99 -c menu.c
gcc -Wall -Wextra -std=c99 -c P2.c
gcc -Wall -Wextra -std=c99 -o P2.exe P2.o utils.o arvore.o categoria.o arquivo.o menu.o
```

### 4. Executar

```bash
P1.exe
P2.exe
```

## Opção 2: Visual Studio

### 1. Instalar Visual Studio Community

Baixe de: https://visualstudio.microsoft.com/

### 2. Criar projeto C

- Novo Projeto → Aplicativo de Console C/C++
- Adicionar todos os arquivos .c e .h ao projeto

### 3. Configurar biblioteca JSON-C

Você precisará:
1. Baixar json-c para Windows
2. Configurar em: Propriedades → C/C++ → Adicional → Incluir Diretórios
3. Adicionar biblioteca em: Propriedades → Linker → Entrada

### 4. Compilar

Use o botão "Build" ou F7 no Visual Studio.

Os executáveis .exe estarão em `Debug/` ou `Release/`.

## Opção 3: WSL (Windows Subsystem for Linux)

Se você tem WSL instalado:

```bash
# No terminal WSL (Ubuntu)
sudo apt-get update
sudo apt-get install gcc libjson-c-dev make

# Navegar até o diretório do projeto
cd /mnt/c/caminho/para/o/projeto

# Compilar
make all

# Executar
./P1
./P2
```

## Opção 4: Cygwin

1. Instalar Cygwin de: https://www.cygwin.com/
2. Durante instalação, selecionar pacotes:
   - gcc-core
   - make
   - libjson-c-devel
3. Usar terminal Cygwin e comandos Linux normais

## Notas Importantes

- Os executáveis gerados no Linux não funcionam no Windows
- Você precisa recompilar no Windows para gerar .exe
- A forma mais fácil é usar MSYS2 ou WSL
- Certifique-se de ter a biblioteca json-c instalada

## Makefile para Windows (MinGW)

Você pode criar um `Makefile.win`:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LIBS = -ljson-c

all: P1.exe P2.exe

P1.exe: P1.c
	$(CC) $(CFLAGS) -o P1.exe P1.c $(LIBS)

P2.exe: P2.o utils.o arvore.o categoria.o arquivo.o menu.o
	$(CC) $(CFLAGS) -o P2.exe P2.o utils.o arvore.o categoria.o arquivo.o menu.o

P2.o: P2.c
	$(CC) $(CFLAGS) -c P2.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

arvore.o: arvore.c
	$(CC) $(CFLAGS) -c arvore.c

categoria.o: categoria.c
	$(CC) $(CFLAGS) -c categoria.c

arquivo.o: arquivo.c
	$(CC) $(CFLAGS) -c arquivo.c

menu.o: menu.c
	$(CC) $(CFLAGS) -c menu.c

clean:
	del *.o P1.exe P2.exe

.PHONY: all clean
```

Uso:
```bash
mingw32-make -f Makefile.win
```
