#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	//open file
	int fd1 = open("test.txt", O_RDONLY);
	if(fd1 == -1)
	{
		perror("open test.txt");
		goto out;
	}
	int fd2 = open("test2.txt", O_WRONLY|O_CREAT, 0664);
	if(fd2 == -1)
	{
		perror("open test2.txt");
		goto out;
	}

	//read data for test.txt and write data to test2.txt
	unsigned char buf[100]={0};
	int read_count=0;
	int ret=0;	
	do
	{
		read_count = read(fd1,buf,sizeof(buf));
		printf("read_count = %d\n",read_count);
		if(read_count == -1)
		{
			perror("read test.txt");
			goto out;
		}
		ret = write(fd2, buf, read_count);
		if(ret == -1)
		{
			perror("write test2.txt");
			goto out;
		}
	}while(read_count>0);

	//close file
out:
	close(fd1);
	close(fd2);
	return 0;
}



