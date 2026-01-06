CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SERVER = serveur_tcp
CLIENT = client_tcp

SERVER_SRC = serveur_tcp.c user.c
CLIENT_SRC = client_tcp.c

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER)

$(CLIENT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT)

clean:
	rm -f $(SERVER) $(CLIENT)

