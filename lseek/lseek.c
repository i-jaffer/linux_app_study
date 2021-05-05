#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd = open("test.txt", O_WRONLY|O_CREAT,0664);
	if(fd == -1)
	{
		perror("open");
		goto out;
	}
	int len=0;
	len = lseek(fd, 0, SEEK_END);
	printf("test.txt len %d\n",len);
	
	len = lseek(fd, 2000, SEEK_END);
	printf("test.txt len %d\n",len);

	int ret = write(fd,"a",1);
	if(ret == -1)
	{
		perror("write");
		goto out;
	}
	

out:
	close(fd);
	return 0;
}


