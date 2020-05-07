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

	inf = open(argv[2], O_RDONLY);
	if (inf < 0) giveUp("Can't open file");

	// read file

	while ((nread = read(inf, buf, bufsize)) > 0) {
		nblocks++;
		printf("Block %d contained %d bytes\n",nblocks,(int)nread);
		nbytes += nread;
	}
	close(inf);

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
