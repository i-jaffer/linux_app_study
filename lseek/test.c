#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd = open("test.txt", O_WRONLY);
	if(fd == -1)
	{
		perror("open");
		goto out;
	}
	int len=0;
	len = lseek(fd, 0, SEEK_CUR);
	printf("fd now:%d\n",len);

	len = lseek(fd, 200, SEEK_END);
	printf("fd mov:%d\n",len);

	close(fd);
	fd = open("test.txt", O_WRONLY);
	if(fd == -1)
	{
		perror("open");
		goto out;
	}
	len = lseek(fd, 0, SEEK_CUR);
	printf("again fd now:%d\n",len);

out:
	close(fd);
	return 0;
}
