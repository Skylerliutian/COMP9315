CC=gcc
CFLAGS=-gdwarf-2

jsim : joinsim.o bufpool.o
	$(CC) $(CFLAGS) -o jsim joinsim.o bufpool.o

joinsim.o : joinsim.c bufpool.h
bufpool.o : bufpool.c bufpool.h

clean :
	rm -f jsim *.o core
