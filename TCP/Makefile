CC      := gcc
CSTD    := -std=gnu11
CWARN   := -Wall -Wextra
CFLAGS  := $(CSTD) $(CWARN)

all: serveur_tcp_forum client_tcp_forum

serveur_tcp_forum: serveur_tcp_forum.c user.c
	$(CC) $(CFLAGS) $^ -o $@

client_tcp_forum: client_tcp_forum.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f serveur_tcp_forum client_tcp_forum
.PHONY: all clean


