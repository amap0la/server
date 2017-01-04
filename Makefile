CC?=gcc
CFLAGS=-Werror -Wextra -Wall -pedantic -std=c99
SRC_SERVER=src/server.c src/daemonize.c
SRC_CLIENT=src/client.c
OBJ_SERVER= ${SRC_SERVER:.c=.o}
OBJ_CLIENT= ${SRC_CLIENT:.c=.o}

all: client server

client: $(OBJ_CLIENT)
	$(CC) $(LFLAGS) $(CFLAGS) $^ -o $@

server: $(OBJ_SERVER)
	$(CC) $(LFLAGS) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o client server

.PHONY: clean
