# Makefile for COMP9315 20T1 Sample Exam Q1

CC=gcc
CFLAGS=-Wall -Werror -g

buckets: buckets.o bits.o

buckets.o: buckets.c bits.h
bits.o: bits.c bits.h

clean:
	rm -f buckets *.o
