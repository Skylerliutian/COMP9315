# COMP9315 19T2 Final Exam Q2
# Join/Buffer-pool simulator

CC=gcc
CFLAGS=-Wall -Werror -g -std=c99

bnl : bnl.o bufpool.o
	$(CC) -o bnl bnl.o bufpool.o

bnl.o : bnl.c bufpool.h
bufpool.o : bufpool.c bufpool.h

clean :
	rm -f bnl *.o core
