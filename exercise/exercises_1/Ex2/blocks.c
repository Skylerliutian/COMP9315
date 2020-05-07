#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void giveUp(char *);

int main(int argc, char **argv)
{
	char   *buf;
	int     inf;  // file descriptor
	int     bufsize;
	ssize_t nread;
	int     nblocks = 0, nbytes = 0;

	// process command line args

	if (argc < 3) giveUp("Insufficient args");
	bufsize = atoi(argv[1]);
	if (bufsize < 100) giveUp("Invalid buffer size");
	buf = malloc(bufsize * sizeof(char));
	if (buf == NULL) giveUp("Can't create buffer");

    // open file

	if ((inf = open(argv[2], O_RDONLY)) < 0)
		giveUp("Can't read file");

	// read file and count blocks/bytes

	while ((nread = read(inf,buf,bufsize)) != 0) {
		nblocks++;
		nbytes += nread;
		printf("%ld bytes read in current block\n",nread);
	}

	// display results

	printf("Read %d blocks and %d bytes\n",nblocks,nbytes);
	
	exit(EXIT_SUCCESS);
}

void giveUp(char *msg)
{
	fprintf(stderr, "Error: %s\n",msg);
	fprintf(stderr, "Usage: ./blocks BlockSize InputFile\n");
	exit(EXIT_FAILURE);
}
