#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{
	int fd;
	int offset = atoi(argv[1]);
	int size = atoi(argv[2]);

	fd = open(argv[3], O_RDWR);
	int end = lseek(fd, -1, 2);

	if(size > 0)
	{
		int frontBufSize = offset+1;
		char frontbuffer[frontBufSize];

		int backBufSize = end - offset - size;
		char backbuffer[backBufSize];

		lseek(fd, 0, 0);
		read(fd, &frontbuffer, frontBufSize);
		lseek(fd, offset+size+1, 0);
		read(fd, &backbuffer, backBufSize);
		
		close(fd);
		fd = open(argv[3], O_RDWR|O_TRUNC);

		write(fd, &frontbuffer, frontBufSize);
		write(fd, &backbuffer, backBufSize);
	}
	else if(size < 0)
	{
		size = -size;
		if(size < offset)
		{
			int frontBufSize = offset - size;
			char frontbuffer[frontBufSize];
			int backBufSize = end - offset + 1;
			char backbuffer[backBufSize];

			lseek(fd, 0, 0);
			read(fd, &frontbuffer, frontBufSize);
			lseek(fd, offset, 0);
			read(fd, &backbuffer, backBufSize);
		
			close(fd);
			fd = open(argv[3], O_RDWR|O_TRUNC);

			write(fd, &frontbuffer, frontBufSize);
			write(fd, &backbuffer, backBufSize);
		}
		else
		{
			int backBufSize = end - offset + 1;
			char backbuffer[backBufSize];

			lseek(fd, offset, 0);
			read(fd, &backbuffer, backBufSize);
		
			close(fd);
			fd = open(argv[3], O_RDWR|O_TRUNC);

			write(fd, &backbuffer, backBufSize);
		}
	}
	close(fd);
	return 0;
}
