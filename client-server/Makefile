CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS =

# Diretórios dos arquivos fonte e objetos
SRC_DIR = src
OBJ_DIR = obj

# Lista de fontes para o servidor e cliente
SERVER_SOURCES = $(SRC_DIR)/server.c $(SRC_DIR)/game.c
CLIENT_SOURCES = $(SRC_DIR)/client.c $(SRC_DIR)/game.c

# Objetos correspondentes
SERVER_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/server_%.o, $(SERVER_SOURCES))
CLIENT_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/client_%.o, $(CLIENT_SOURCES))

# Nome dos executáveis
SERVER_EXECUTABLE = server
CLIENT_EXECUTABLE = client

all: $(SERVER_EXECUTABLE) $(CLIENT_EXECUTABLE)

$(SERVER_EXECUTABLE): $(SERVER_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(CLIENT_EXECUTABLE): $(CLIENT_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/server_%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/client_%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(SERVER_EXECUTABLE) $(CLIENT_EXECUTABLE)
	rm -rf $(OBJ_DIR)
