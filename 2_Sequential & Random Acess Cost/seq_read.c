#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define DATA_SIZE 200

int main(int argc, char **argv)
{
	if(argc > 2){
		printf("Error!\n");
		return -1;
	}

	int fd= open(argv[1], O_RDWR);
	if(fd < 0){
		printf("Error! There's no file.\n");
		return -2;
	}
	
	char buffer[DATA_SIZE];
	
	struct timeval start, end;
	gettimeofday(&start, NULL);

	lseek(fd, 0, 0);
	while(read(fd, &buffer, DATA_SIZE) != 0){}

	gettimeofday(&end,NULL);
	printf("%ld usec\n", end.tv_usec - start.tv_usec);
	
	close(fd);
	return 0;
}
