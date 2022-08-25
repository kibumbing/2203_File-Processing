#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int  main(int argc, char** argv)
{
	int fd;
	int offset = atoi(argv[1]);	
	int size = strlen(argv[2]);

	fd = open(argv[3], O_RDWR);
	
	int end =  lseek(fd, -1, 2);
	if(offset >= end)
	{
		return -1;
	}

	int backSize = end - offset;
	char buffer[backSize];

	lseek(fd, offset+1, 0);
	read(fd, &buffer, backSize);
	lseek(fd, offset+size+1, 0);
	write(fd, &buffer, backSize);
	lseek(fd, offset+1, 0);
	write(fd, argv[2], size);
	
	close(fd);
	return 0;
}
