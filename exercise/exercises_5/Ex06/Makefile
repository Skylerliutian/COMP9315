CC=gcc
CFLAGS=-Wall -Werror

all: s h

s : s.o showBits.o bits.o
	gcc -o s s.o showBits.o bits.o

h : h.o hash.o showBits.o bits.o
	gcc -o h h.o hash.o showBits.o bits.o

clean:
	rm -f s h *.o core
