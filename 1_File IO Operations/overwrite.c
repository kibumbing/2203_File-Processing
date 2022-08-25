#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void main(int argc, char** argv)
{
	int fd;
	int offset = atoi(argv[1]);	

	fd = open(argv[3], O_RDWR);

	lseek(fd, offset, 0);
	write(fd, argv[2], strlen(argv[2]));
	
	close(fd);

}

