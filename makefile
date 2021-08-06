CC=gcc
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=
EXEC=localisation.exe
SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin

all: $(EXEC)

hello: hello.o main.o
    $(CC) -o $@ $^ $(LDFLAGS)

main.o: hello.h

%.o: %.c
    $(CC) -o $@ -c $< $(CFLAGS)

clean:
    rm -rf *.o

mrproper: clean
    rm -rf $(EXEC)