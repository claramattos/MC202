CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: rearranjar

rearranjar: rearranjar.c
	gcc rearranjar.c -g
	gcc $(CFLAGS) rearranjar.c -o rearranjar $(LDFLAGS)
	

clean:
	rm -f rearranjar
	rm a.exe
