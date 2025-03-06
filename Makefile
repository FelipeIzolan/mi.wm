CC = gcc
CFLAGS = -O3
LIBS = -lxcb -lxcb-keysyms

DIR = build

all: mi

mi:
	$(CC) wm.c -o $@ bitmap.c $(CFLAGS) $(LIBS)

install: all
	cp -f mi /usr/bin

uninstall:
	rm -f /usr/bin/mi

clean:
	rm -f mi
