# Compilador e Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -lm

# Diretórios
SRC_DIR = src
INC_DIR = include
TEST_DIR = testes

# Arquivos fonte do projeto principal (todos os .c da pasta src)
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

# Executável principal
MAIN_EXEC = main.exe

# Testes: encontra todos os .c dentro de testes/ e gera os executáveis em testes/
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_EXECS = $(TEST_SRCS:.c=.exe)

# Regra principal (compila o programa principal e os testes)
all: $(MAIN_EXEC) tests

# Regra para compilar o executável principal
$(MAIN_EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Regra para compilar arquivos de teste
$(TEST_DIR)/%.exe: $(TEST_DIR)/%.c $(OBJ_FILES)
	$(CC) $(CFLAGS) $< $(OBJ_FILES) -o $@ $(LDFLAGS)

# Compilar objetos da pasta src/
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra conveniente para compilar apenas os testes
tests: $(TEST_EXECS)

# Limpeza padrão (arquivos compilados e saídas)
clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(MAIN_EXEC)
	rm -f $(TEST_DIR)/*.exe
	rm -f results/*

# Novo procedimento para remover apenas os arquivos .gitkeep das pastas
rmkeeps:
	rm -f .gitkeep docs/.gitkeep include/.gitkeep results/.gitkeep src/.gitkeep testes/.gitkeep

.PHONY: all tests clean rmkeeps