# COMP9315 19T2 Final Exam ... Q1
# Multi-attribute hash values

CC=gcc
CFLAGS=-Wall -Werror -g -std=c99
LIBS=bits.o chvec.o hash.o tuple.o

mah: mah.o $(LIBS)
	$(CC) -o mah mah.o $(LIBS)

mah.o: mah.c bits.h chvec.h hash.h tuple.h

bits.o: bits.c defs.h bits.h
chvec.o: chvec.c defs.h chvec.h
hash.o: hash.c defs.h hash.h bits.h
tuple.o: tuple.c defs.h tuple.h chvec.h hash.h bits.h

clean :
	rm -f mah *.o core
