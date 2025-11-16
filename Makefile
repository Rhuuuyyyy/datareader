# Makefile para compilar os programas P1 e P2

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LIBS = -ljson-c

# Alvos principais
all: P1 P2

# Programa P1 (conversão JSON para binário)
P1: P1.o
	$(CC) $(CFLAGS) -o P1 P1.o $(LIBS)

P1.o: P1.c
	$(CC) $(CFLAGS) -c P1.c

# Programa P2 (sistema de gerenciamento)
P2: P2.o utils.o arvore.o categoria.o arquivo.o menu.o
	$(CC) $(CFLAGS) -o P2 P2.o utils.o arvore.o categoria.o arquivo.o menu.o

P2.o: P2.c tipos.h arquivo.h categoria.h menu.h
	$(CC) $(CFLAGS) -c P2.c

utils.o: utils.c utils.h tipos.h
	$(CC) $(CFLAGS) -c utils.c

arvore.o: arvore.c arvore.h tipos.h
	$(CC) $(CFLAGS) -c arvore.c

categoria.o: categoria.c categoria.h tipos.h arvore.h utils.h
	$(CC) $(CFLAGS) -c categoria.c

arquivo.o: arquivo.c arquivo.h tipos.h categoria.h utils.h
	$(CC) $(CFLAGS) -c arquivo.c

menu.o: menu.c menu.h tipos.h categoria.h arvore.h
	$(CC) $(CFLAGS) -c menu.c

# Limpar arquivos compilados
clean:
	rm -f *.o P1 P2

# Executar P1
run-p1: P1
	./P1

# Executar P2
run-p2: P2
	./P2

# Executar tudo (P1 e depois P2)
run: P1 P2
	./P1
	./P2

.PHONY: all clean run-p1 run-p2 run
