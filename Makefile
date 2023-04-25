CFLAGS := -g -O0 -I.
CC := gcc

all: coro arg_test arg_test_x86

arg_test_x86: arg_test_x86.c
	$(CC) $(CFLAGS) $^ -o $@ -m32

arg_test: arg_test.c
	$(CC) $(CFLAGS) $^ -o $@

coro: coro.c
	$(CC) $(CFLAGS) $^ -o $@

coro.S:coro.c
	$(CC) $(CFLAGS) -S $^ -o $@

PHONY: clean
clean:
	rm -f coro coro.S