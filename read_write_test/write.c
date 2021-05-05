#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	//open test.txt and  creat test.txt
	int fd = open("test.txt", O_RDWR|O_CREAT, 0664);
	if(fd <0)
	{
		perror("open");
		goto out;
	}

	//write data to test.txt
	unsigned char write_buf[]="asdfghjkl;1234567890";
	int ret=0;
	for(int i=0; i<100;i++)
	{
		ret = write(fd, write_buf, sizeof(write_buf));
		if(ret == -1)
		{
			perror("write");
			goto out;
		}
	}
	
	//close test.txt
out:
	close(fd);
	return 0;
}


