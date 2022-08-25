#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void main(int argc, char** argv)
{
	int fd1, fd2, fd3;
	char c;

	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_RDONLY);
	fd3 = open(argv[3], O_RDWR|O_CREAT, 0775);

	while(read(fd1, &c, 1) != 0)
	{
		write(fd3, &c, 1);
	}
	while(read(fd2, &c, 1) != 0)
	{
		write(fd3, &c, 1);
	}
	close(fd1);
	close(fd2);
	close(fd3);
}
