#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void main(int argc, char** argv)
{
	int fd1, fd2;
	int size = 10;
	char buffer[size];
	int length;

	fd1 = open(argv[1], O_RDWR);
	fd2 = open(argv[2], O_RDWR|O_CREAT, 0775);

	while((length = read(fd1, &buffer, size)) != 0)
	{
		write(fd2, &buffer, length);
	}	

	close(fd1);
	close(fd2);
}

