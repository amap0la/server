CC=gcc
CFLAGS=-Werror -Wextra -Wall -pedantic -std=c99
SRC=src/devidd_ctl.c
OBJ= ${SRC:.c=.o}
PROJECT=devidd_ctl

all: $(PROJECT)

$(PROJECT): $(OBJ)
	$(CC) $(LFLAGS) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJ) $(PROJECT)

.PHONY: clean
