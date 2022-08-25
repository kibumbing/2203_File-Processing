#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void main(int argc, char** argv)
{
	int fd;
	int offset = atoi(argv[1]);
	int size = atoi(argv[2]);
	char c;

	fd = open(argv[3], O_RDONLY);

	if(size > 0)
	{
		lseek(fd, offset+1, 0);
	}
	else if(size < 0)
	{
		size = -size;
		if(size < offset)
		{
			lseek(fd, offset-size, 0);
		}
		else
		{
			size = offset;
		}
	}

	for(int i = 0; i<size; i++)
	{
		if(read(fd, &c, 1) != 0)
		{
			write(1, &c, 1);
		}

	}
	close(fd);
}
