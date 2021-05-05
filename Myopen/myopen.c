#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	int fd;
	fd = open("hello.c",O_RDWR|O_TRUNC);
	printf("fd = %d\n",fd);
	if(fd < 0)
	{
		perror("open file");
		exit(1);
	}

	int ret = close(fd);
	printf("ret = %d",ret);
	if(ret == -1)
	{
		perror("close file");
		exit(1);
	}
	return 0;
}
