#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SUFFLE_NUM 10000 
#define DATA_SIZE 200

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);

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
	
	int num_of_records = lseek(fd, 0, 2) / DATA_SIZE;
	int *read_order_list = (int*)malloc(sizeof(int) * num_of_records);
	char buffer[DATA_SIZE];

	GenRecordSequence(read_order_list, num_of_records);
	
	struct timeval start, end;
	gettimeofday(&start, NULL);

	lseek(fd, 0, 0);
	int pre_record = -1;
	for(int i=0; i<num_of_records; i++)
	{
		int point = read_order_list[i] - pre_record - 1;
		lseek(fd, 200 * point, 1);
		read(fd, &buffer, DATA_SIZE);
		pre_record = read_order_list[i];
	}
	
	gettimeofday(&end,NULL);
	printf("%ld usec\n", end.tv_usec - start.tv_usec);
	
	close(fd);
	free(read_order_list);
	return 0;
}

void GenRecordSequence(int *list, int n)
{
	int i, j, k;

	srand((unsigned int)time(0));

	for(i=0; i<n; i++)
	{
		list[i] = i;
	}
	
	for(i=0; i<SUFFLE_NUM; i++)
	{
		j = rand() % n;
		k = rand() % n;
		swap(&list[j], &list[k]);
	}

	return;
}

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;

	return;
}
