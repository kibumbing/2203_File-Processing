#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct Student{
	char data[200];
};


int main(int argc, char **argv)
{
	if(argc < 3){
		printf("Error!\n");
		return -1;
	}

	int count = atoi(argv[1]);
	int fd = open(argv[2], O_RDWR);
	if(fd < 0)
	{
		printf("Error! There's no file.\n");
		return -2;
	}

	struct Student *new_data = (struct Student*)calloc(count, sizeof(struct Student));

	for(int i=0; i<count; i++){
		strcpy(new_data[i].data, "student_data");
	}

	for(int i=0; i<count; i++){
		write(fd, &new_data[i], 200);
	}

	close(fd);
	free(new_data);

	return 0;
}
