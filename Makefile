CFLAGS := -g -O0 -I.
CC := gcc

all: coro

coro: coro.c
	$(CC) $(CFLAGS) $^ -o $@

coro.S:coro.c
	$(CC) $(CFLAGS) -S $^ -o $@

PHONY: clean
clean:
	rm -f coro coro.S