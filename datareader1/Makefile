# Nome do compilador
CC = gcc

# Flags de compilação:
# -Wall: Habilita todos os avisos (warnings) do compilador, o que é uma boa prática.
# -g: Inclui informações de depuração no executável, necessário para usar o debugger.
CFLAGS = -Wall -g

# Flags do linker:
# -ljson-c: Informa ao linker para incluir a biblioteca "json-c".
LDFLAGS = -static -ljson-c

# Nome do arquivo executável que será gerado
EXECUTABLE = programa

# Lista de todos os arquivos-fonte (.c)
SOURCES = main.c funcoes_usuario.c

# Gera a lista de arquivos objeto (.o) a partir dos arquivos-fonte
OBJECTS = $(SOURCES:.c=.o)

# Regra principal: é executada quando você digita "make"
all: $(EXECUTABLE)

# Regra para criar o executável:
# Depende de todos os arquivos objeto.
# Executa o comando de linkagem para juntar os objetos e bibliotecas.
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

# Regra para compilar cada arquivo .c em um arquivo .o:
# Transforma um .c em um .o correspondente.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados (executável e objetos)
# Útil para forçar uma recompilação completa.
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) *.exe