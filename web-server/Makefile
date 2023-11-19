CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS =

# Diretórios dos arquivos fonte e objetos
SRC_DIR = src
OBJ_DIR = obj

# Lista de fontes para o servidor e cliente
SERVER_SOURCES = $(SRC_DIR)/server.c $(SRC_DIR)/web.c $(SRC_DIR)/file.c $(SRC_DIR)/client.c $(SRC_DIR)/threadtask.c

# Objetos correspondentes
SERVER_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/server_%.o, $(SERVER_SOURCES))

# Nome dos executáveis
SERVER_EXECUTABLE = server

all: $(SERVER_EXECUTABLE)

$(SERVER_EXECUTABLE): $(SERVER_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/server_%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(SERVER_EXECUTABLE)
	rm -rf $(OBJ_DIR)
