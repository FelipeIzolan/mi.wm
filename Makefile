CC = gcc
CFLAGS = -o build/mi -O3
LIBS = -lxcb

DIR = build

all: $(DIR) BUILD

$(DIR):
	mkdir -p $(DIR)

BUILD:
	$(CC) wm.c $(CFLAGS) $(LIBS)

clean:
	rm $(DIR)/mi
